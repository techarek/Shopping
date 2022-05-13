#include "Arduino.h"
#include "Server_Requests.h"

char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response

void add_item(char * username, char * item, int selection, char * UPC, GPS * gps) {
  strcpy(request_buffer,"");
  strcpy(response_buffer,"");
  // Sends a POST request to add an item to the user's cart
  char body[300]; //for body;
  sprintf(body, "user=%s&item=%s&quantity=%d&UPC=%s,lat=%f,lon=%f", username, item, selection, UPC, gps->lat_deg + gps->lat_dm, gps->lon_deg + gps->lon_dm); //generate body, posting to User, 1 step
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer, "POST http://608dev-2.net/sandbox/sc/team114/final_project/shopping_list.py HTTP/1.1\r\n");
  strcat(request_buffer, "Host: 608dev-2.net\r\n");
  strcat(request_buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
  sprintf(request_buffer + strlen(request_buffer), "Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
  strcat(request_buffer, "\r\n"); //new line from header to body
  strcat(request_buffer, body); //body
  strcat(request_buffer, "\r\n"); //header
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
}

void get_items(char list[][MAX_OPTION_LEN], char UPC_list[][15], int *  list_counts, int list_len, char * username) {
  strcpy(request_buffer,"");
  strcpy(response_buffer,"");
  //  Sends a GET request to get the next item
  for(int i = 0; i < list_len; i++){
    strcpy(list[i],"");
    strcpy(UPC_list[i],"");
    list_counts[i] = 0;
  }
  
  sprintf(request_buffer, "GET http://608dev-2.net/sandbox/sc/team114/final_project/shopping_list.py?user=%s HTTP/1.1\r\n",username);
  strcat(request_buffer, "Host: 608dev-2.net\r\n");
  strcat(request_buffer, "\r\n");
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);

  char temp[OUT_BUFFER_SIZE];
  strcpy(temp,response_buffer);
  char * ptr = strchr(temp,';');
  if(ptr==NULL) return;
  
  list_counts[0] = atoi(strtok(response_buffer,"&"));
  char * upc = strtok(NULL,"&");
  if(strcmp(upc,"Null")==0)
      strcpy(UPC_list[0], "");
  else
      strcpy(UPC_list[0],upc);
  strcpy(list[0],strtok(NULL,";"));
  
  int i=1;
  while(1){
    ptr = strchr(ptr + 1,';'); //may need to be changed if strtok is interfering
    if(ptr==NULL) break; 
    list_counts[i] = atoi(strtok(NULL,"&"));
    char * upc = strtok(NULL,"&");
    if(strcmp(upc,"Null")==0)
        strcpy(UPC_list[i], "");
    else
        strcpy(UPC_list[i],upc);
    strcpy(list[i],strtok(NULL,";"));
    i++;
  }
}


void get_stores(GPS * gps, char * stores) {
  strcpy(request_buffer,"");
  strcpy(response_buffer,"");
  if(gps->valid){
    sprintf(request_buffer, "GET http://608dev-2.net/sandbox/sc/team114/final_project/find_stores.py?goal=%s&lat=%f&lon=%f HTTP/1.1\r\n", "close", gps->lat_deg + gps->lat_dm, gps->lon_deg + gps->lon_dm);
  }
  else{
    sprintf(request_buffer, "GET http://608dev-2.net/sandbox/sc/team114/final_project/find_stores.py?goal=%s&lat=%f&lon=%f HTTP/1.1\r\n", "cheap", 0.0, 0.0);
  }
  strcat(request_buffer, "Host: 608dev-2.net\r\n");
  strcat(request_buffer, "\r\n");
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
  strcpy(stores,response_buffer);
}


void get_product_description(char * output, char * UPC){
  strcpy(request_buffer,"");
  strcpy(response_buffer,"");
  sprintf(request_buffer,"GET http://608dev-2.net/sandbox/sc/team114/final_project/process_image/Process_Image.py?num=0&path=None HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev-2.net\r\n");
  strcat(request_buffer,"\r\n"); //new line from header to body
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  strcpy(response_buffer,&response_buffer[2]);
  strcpy(response_buffer,strtok(response_buffer,"'"));
  
  if(strchr(response_buffer,';')!=NULL){
    strcpy(output,strtok(response_buffer,";"));
    strcpy(UPC,strtok(NULL," "));
  }
  else
    strcpy(output,response_buffer);
    
  strcpy(response_buffer,"");
  strcpy(request_buffer,"");
}
