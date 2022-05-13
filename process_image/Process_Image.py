import cv2
import base64
import numpy as np
import json
import sys
sys.path.append('__HOME__/final_project')
import productInfo2 as prod_info
sys.path.append('__HOME__/final_project/process_image')
import barcode_reader as bcr
import Find_Barcode
import google_image

working_directory = "__HOME__/final_project/process_image"

def request_handler(request):
	if request["method"] == "GET":
		try:
			num_images = int(request['values']['num'])
			if num_images == 1:
				path_suffix = request['values']['path']
				image_paths = [working_directory + "/" + path_suffix]
			elif num_images > 1:
				path_suffixes = request['values']["path"].split(",")
				image_paths = [working_directory + "/" + path for path in path_suffixes]
			else:
				image_paths = ["__HOME__/final_project/.gitignore/tmp.jpg"]
		except:
			return "INVALID REQUEST FORMATTING; Include 'path' and 'num'"

		return [process_image(path) for path in image_paths]

def process_image64(img_64):
	img_arr = np.frombuffer(base64.b64decode(img_64),np.uint8)
	img = cv2.imdecode(img_arr,cv2.IMREAD_COLOR)
	if Find_Barcode.thing(img,"X"):
		barcode = bcr.isolate_barcode(img) #img needs to be RGB
		UPC = bcr.get_UPC(img)
		if len(UPC) == 12 and -1 not in UPC:
			return UPC

	return google_image.get_label(img_64)

def process_image(img_path):
	img = bcr.get_image(img_path)
	if Find_Barcode.thing(img,"X"):
		try:
			barcode = bcr.isolate_barcode(img) #img needs to be RGB
			UPC = bcr.get_UPC(barcode)
			if len(UPC) == 12 and -1 not in UPC:
				UPC_text = "".join(list(map(str,UPC)))
				name = prod_info.getProductName(UPC_text)
				name = name.replace("'","")
				return name + ";" + UPC_text
		except:
			print("Couldn't find UPC")
	return google_image.get_label(img_path)


#if __name__ == "__main__":
	#print(process_image("tmp.jpg"))
	# img = bcr.get_image("tmp.jpg")
	# barcode = bcr.isolate_barcode(img)
	# bcr.show_image(barcode)
	# print(bcr.get_UPC(barcode))

	#print(prod_info.getProductName("705911703599"))
	
