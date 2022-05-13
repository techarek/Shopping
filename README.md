<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" href="https://stackedit.io/style.css" />
</head>

<body class="stackedit">
  <div class="stackedit__html"><h1 id="team-114-final-report">6.08 Team 114 Final Report</h1>
<h1 id="table-of-contents">Table of Contents</h1>
<ol>
<li><a href="#project-idea">Project Idea</a></li>
<li><a href="#synopsis">Synopsis</a></li>
<li><a href="#overview">Overview</a><br>
3.1. <a href="#video-demo">Video Demo</a><br>
3.2. <a href="#system-diagrams">System diagrams</a></li>
<li><a href="#parts-list">Parts List</a></li>
<li><a href="#documentation">Documentation</a><br>
5.1. <a href="#database">Database</a><br>
5.2. <a href="#server-file-descriptions">Server File Descriptions</a><br>
5.3. <a href="#arduino-feature-descriptions">Arduino Feature Descriptions</a></li>
<li><a href="#milestone-videos">Milestone Videos</a></li>
</ol>
<h2 id="project-idea">Project Idea</h2>
<p>Have you ever seen an item at someone’s house that you are really interested in, but you’re not sure where to buy it from - especially at the best price or closest location? Or with things around your own house, if you run out of something and want to purchase more but don’t remember where you got it? Our project aims to fix that problem through adding a camera to our ESP setup to scan an item’s barcode or use image recognition APIs to identify the object. We would then return the closest or cheapest place to buy the item, depending on the user’s preference.</p>
<h2 id="synopsis">Synopsis</h2>
<p>For our project we created a device that allows users to create and maintain a shopping list by taking photos of items and scanning barcodes. We integrated the ArduCAM camera to both display images on the TFT screen and allow the user to take photos of images. These images are then sent to the server where they are processed and an item description is returned to the user to confirm. The user can then view their current shopping list and alter the quantity of items as well as find stores near them (based on GPS location) that are optimized for item price and location.</p>
<h2 id="overview">Overview</h2>
<p>Our system consists of a camera module for the user to take pictures of barcodes or purchasable products and an LCD screen to display images, shopping lists, and prompts to the user.</p>
<p>We have 3 options for the user to choose from on the main menu as shown on the LCD: Add Item, Edit List, and Find Store.</p>
<p><strong>Add Item</strong></p>
<ul>
<li>This allows the user to use the camera to take a picture of either the barcode or the product to run through our image processing pipeline. Once the picture loads and we get a result, the user can confirm the product or retake the picture in the case the image result is incorrect. The user will select how many of the product they want to add and then the item and its quantity will appear in the shopping list database.</li>
</ul>
<p><strong>Edit Item</strong></p>
<ul>
<li>This allows the user to change the quantity of an item in the shopping list database or remove the item.</li>
</ul>
<p><strong>Find Stores</strong></p>
<ul>
<li>When the user selects this option, they will get a list of the closest stores that have the items in their shopping list.</li>
</ul>
<h3 id="video-demo">Video Demo</h3>
<iframe width="560" height="315" src="https://www.youtube.com/embed/WTEpDErSpzQ" allowfullscreen=""></iframe>
<h3 id="system-diagrams">System Diagrams</h3>
<p><strong>Wiring Schematic</strong></p>
<p><img src="https://lh4.googleusercontent.com/xIVvowslBvK_bEa1th5aFsv8MfAo9nlmNKY5c6CpjhtTluzLYE1IU-LO7vbuVtYwnUTsZ0lwzotgxsV8PnuJinR4mL4IISbUSZfl9yEPJarcnwrpNAtrSJrzXWr-OZQYWD7fRUZW" alt=""></p>
<p><strong>Block Diagram</strong><br>
<img src="https://lh3.googleusercontent.com/KKLLYNp7VEeqcR9_Kv0pT7rmOwsRc9DfbiUKXfdWEx-ZpIsSRUNFO3McIBFP4y7C1TJbOd80Fi5Evirsmwt4tGh-1AsrBNgQqooN0eYW8DxpxjohiGZepnZFAutCp17vawgWhgMq" alt=""></p>
<p><strong>Server Side</strong><br>
<strong><img src="https://lh5.googleusercontent.com/uExbPlYU3X2MfW74MzIDQ2Z_WC4PAmI9IsFWJ0iIOQ8MIjVxw8IFFA5nHtzZ22n2px56sddsyg_tGm6v8EDfStuP0bolWFRFssp7ZJ7q_L9J7qZ5ri8VjIZp3iHllZj75uIYJYdi" alt=""></strong></p>
<h2 id="parts-list">Parts List</h2>
<p>From the class:</p>
<ul>
<li>
<p>ESP32</p>
</li>
<li>
<p>LCD screen</p>
</li>
<li>
<p>IMU</p>
</li>
<li>
<p>GPS</p>
</li>
<li>
<p>2 push buttons</p>
</li>
</ul>
<p>Purchased:</p>
<ul>
<li>2MP Arducam</li>
</ul>
<h2 id="documentation">Documentation</h2>
<h3 id="database">Database</h3>
<p>photo.db <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mo>→</mo></mrow><annotation encoding="application/x-tex">\rightarrow</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="base"><span class="strut" style="height: 0.36687em; vertical-align: 0em;"></span><span class="mrel">→</span></span></span></span></span> Table “jpeg”</p>

<table>
<thead>
<tr>
<th>Column</th>
<th align="left">data</th>
</tr>
</thead>
<tbody>
<tr>
<td><strong>Type</strong></td>
<td align="left">int</td>
</tr>
<tr>
<td><strong>Description</strong></td>
<td align="left">image data where each row entry represents the RGB for a pixel</td>
</tr>
<tr>
<td><strong>Example</strong></td>
<td align="left">114</td>
</tr>
</tbody>
</table><br>  
<p>shopping_list3.db <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mo>→</mo></mrow><annotation encoding="application/x-tex">\rightarrow</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="base"><span class="strut" style="height: 0.36687em; vertical-align: 0em;"></span><span class="mrel">→</span></span></span></span></span> Table “products”</p>

<table>
<thead>
<tr>
<th>Column</th>
<th>user</th>
<th>item</th>
<th>quantity</th>
<th>UPC</th>
<th>closest</th>
<th>cheapest</th>
</tr>
</thead>
<tbody>
<tr>
<td><strong>Type</strong></td>
<td>text</td>
<td>text</td>
<td>int</td>
<td>text</td>
<td>text</td>
<td>text</td>
</tr>
<tr>
<td><strong>Description</strong></td>
<td>The username that has the item in its cart</td>
<td>Name of a product</td>
<td>The number of that item that is in the user’s cart</td>
<td>The UPC code of the item (can be empty if unknown)</td>
<td>The name of the closest store to the user’s location that carries this product</td>
<td>The name of the store to the user’s location that carries this product for the cheapest price</td>
</tr>
<tr>
<td><strong>Example</strong></td>
<td>USER4</td>
<td>acoustic guitar</td>
<td>1</td>
<td>841060045279</td>
<td>Guitar Center</td>
<td>zZounds</td>
</tr>
</tbody>
</table><p><strong>Store_Image.py</strong></p>
<p>Because of the large sizes of images, we need to send image data from the ESP32 in small batches. Every time a batch of information is sent, it is stored in a database file to be read out when all of the image data has reached the server. When all of the image data has reached the server, the data is then written into a JPEG file that can then be analyzed for the presence of a barcode.</p>
<p><strong>Shopping_list.py</strong></p>
<p>When a user confirms a new item to add to the database, the ESP32 sends the item description, the GPS coordinates of the user, the item quantity, and the UPC code if the original image was a barcode. This information is then used to query the product APIs that we used in order to find both the cheapest and the closest stores where the item can be purchased. The name of the item, the UPC code, the item quantity, the closest store, and the cheapest store are then all written to the product database to be read back later when the user requests to view the shopping list.</p>
<h3 id="server-file-descriptions">Server File Descriptions</h3>
<p><strong>Google_image.py</strong><br>
Goal: Recognize a product from an image</p>
<p><img src="https://lh6.googleusercontent.com/TaPEoD01Xh3w3lTxUnTgBSuZGW0n0LWHoQz-NSqjY_jw962msE_6-dn5uVoQNTryx-duSPE4vY7Zm0Ta2RZrannu6TikbTXmZVx8PvoQsrKE4Vbvj1_81L-M5Ha0fXMWPK7j0wRH" alt=""></p>
<p><em>Our script sends a request to the Google API and identifies the image on the left as “chobani greek yogurt”</em></p>
  <br>
<p>We utilized Google’s Cloud Visions API to identify objects a user takes an image of using our system. The API requires a key which we obtained, and we also wrote a Python script to send HTTP POST requests to the Google server. In our script we convert the image to a base64 format and convert it to the utf-8 form in order to pass it into the JSON request.</p>
<p>Initially, the results were very generic such as returning “natural foods” when we sent a picture of an apple. The API allows us to detect specific things using features like Labels, Logos, and Web Entities. Whilst testing the different features the API offers, we found that detecting using Web Entities and returning the bestGuessLabel value from the JSON response gave the most accurate results on average.</p>
<p><strong>Find_barcode.py</strong></p>
<p>Goal: Differentiate between image with barcode and regular image of product</p>
<p>In order to do this, we integrated functions from OpenCV in order to process the images passed to the request handler. We assumed the images would be passed in base64 form, so we first decode it before passing it into a numpy array. To follow along with the process of image processing, we included an example image below:</p>
<p><img src="https://lh5.googleusercontent.com/BtTCLBwc_5ByG3tmhbsXtgI4OZM8f9KRD1Vjv3y6HGpoVWXFhHnlMHa9cdr1QsN8zvSK0xZgyAYzyieyPa1F2srQ7WaSupBcXtt7p6S43ofPn1wjxuQf9m17lGgYZKkH_b-AX-5F" alt=""></p>
<p>In order to process the image, we make it grayscale using the</p>
<pre><code>
greyedOut = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

</code></pre>
<p>command. Next, we find the x and y gradients of the image of the image and subtract them from each other so that we have an image with a high horizontal gradient and low vertical gradient. This allows us to look for distinct changes in the image. This is done in the following lines of code:</p>
<pre><code>
dx = cv2.Scharr(greyedOut, ddepth=cv2.CV_32F, dx=1, dy=0)

dy = cv2.Scharr(greyedOut, ddepth=cv2.CV_32F, dx=0, dy=1)

  

#combine gradients by subtracting

der = cv2.subtract(dy, dx)

der = cv2.convertScaleAbs(der)

</code></pre>
<p>Once this operation is performed, the image looks like this:</p>
<p><img src="https://lh4.googleusercontent.com/LMqf0e7PZkJSipV8bHGJ0eKSIGdQiK_FTMafD6p1zeXvS_RtX7uqrlmrz-ipojmZdDgk1QHbN2rbtROzNr0msMruYoTwqudYQOuzRzpZ0fd6g15-galSnIC7JD9mw0-MPa_PnkgN" alt=""></p>
<p>Next, we want to blur the image to try and fix some of the space present between barcode lines. This is done in the following code segment:</p>
<pre><code>
blurred = cv2.blur(der, (5, 5))

thresh = cv2.threshold(blurred, 225, 255, cv2.THRESH_BINARY)[1]

  
#this allows us to blur vertically or horizontally, depending on direction param

if direction == "Y":

kernel = np.ones((20, 10))

else:

kernel = np.ones((10, 20))

closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

</code></pre>
<p>Once done, the images look like this:</p>
<p><img src="https://lh4.googleusercontent.com/zRncvQwZcZve2eLt7looxlZdodA3WhebkChUe1rXD_hxEAIQ1pKkambx8odxl1wETragRqoBg7dU9D51wXeIajKUIxGVss77ZUX4PaFpKTJSUAEaIJo7rAP5on6eOclt0djjRye7" alt=""></p>
<p>Now we want to get rid of some of the smaller whitespots by ‘eroding’ and then enlarge existing white patches by ‘dilating’ in order to fill in some of the gaps. This is done in the following lines:</p>
<pre><code>
eroded = cv2.erode(closed, None, iterations=25)

dilated = cv2.dilate(eroded, None, iterations=25)

</code></pre>
<p>Once completed, the images look like this:</p>
<p><img src="https://lh6.googleusercontent.com/fLTeU57pDy0WbyBPtIWEAtz5VwVs_Qv6u4qAoQ448u8GqIRRiMcQoNU4UUCaNiI4xuvcGicr9_BgxVbLU93KKq9egGalC0Ox96GMvRJ3IG4C5JAEuWZZrxppE40aLPbO0YBCNy3D" alt=""></p>
<p>Finally, we pick the biggest white chunk and create a bounding box around the blob. If the ‘barcode’ it found was big enough (implying it actually existed), then we return True. Otherwise, we simply return False.</p>
<p><strong>Barcode_reader.py</strong></p>
<p>Goal: Integrate barcode recognition</p>
<p>We used the OpenCV library to detect barcodes in photos and then generate the UPC code from the barcode. We broke down the problem into the following three steps:</p>
<p><u>Finding the barcode in the image</u></p>
<p>Once an image is verified to have a barcode using the “Differentiate between image with barcode and regular image of product” step above, we need to find the barcode in the image and focus on this area. To do this, we run the image through several filters in OpenCV designed to pick out edges and then focus on edges that meet the criteria of fitting a barcode. Once we have found the general region of the barcode, we remove the rest of the image and focus exclusively on this region. A video of this can be found below:</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/-K_nC8TRn0g" allowfullscreen=""></iframe>
<p><u>Rotating the barcode to be straight</u></p>
<p>While it is important for the user to do their best to provide an image with the barcode already straight, the code we wrote has the ability to detect barcodes at up to a 20 degree tilt and straighten them before analyzing them. It does this step simply by bounding each of the barcode rectangles in a minimum area rectangle and then taking the median tilt of these rectangles and straightening the barcode by this amount. This is important as it allows us to correctly analyze the digits on the barcode in the next step. A video of this can be found below:</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/c7TJMKSpVzs" allowfullscreen=""></iframe>
<p><u>Getting the UPC number from a cropped and straightened barcode</u></p>
<p>The final step in the process is extracting the UPC number from the cropped and straightened barcode. To do this, we again use OpenCV to bound the barcode strips in rectangles before measuring their width and determining the UPC number from these values. A video of this can be found below:</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/sRBjfGFUZ5U" allowfullscreen=""></iframe>
<p><u>Combining all of these steps</u></p>
<p>We then combined all of these steps to extract the UPC code from an image with a barcode in it. Obviously, this process still requires that the user provide images of the barcode in decent lighting with good orientation but allows for enough variability that this is not a requirement that is hard to implement in practice for the user. Below is the video of the entire system working properly on five product barcodes:</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/KBate2sTIU8" allowfullscreen=""></iframe>
<p><strong>ProductInfo2.<span>py</span></strong></p>
<p>Goal: Find product information (price, stores, and availability)</p>
<p>In order to get the product information from barcode numbers, we integrated the UPCitemdb API. We send it get requests with the upc code passed in and then parsed the json result so that it displays all the available offers with the most updated price, store domain, and availability. Here are five examples of us passing in a barcode number to receive information about price and availability:</p>
<p><img src="https://lh6.googleusercontent.com/_7hpU48JFIOhaa81rDhqf3AqIiXpWa7aCnr0JTJ6EJ_YJdYON6qt99wZAlTYlF9tzgtAsWyEFyZDbBaBQ0QCCriv5UOVlnf8aM4f02Vp1GDmp8J3g-lT1Yq3ZfCPe3Xpx6LUHaEf" alt=""></p>
<p><img src="https://lh4.googleusercontent.com/JT_3kZu3mUOJ2A00xEpSQzFyekdEBXHRutXHpDBYdk4SOcHAhM7xMjSq3gaTUPnIe5DQ0t-U35NwqrmxHNS8JUp0e1CICAlEKOk3VfbY_UwEtAmWyCLBK4PezE0iixJKTf_-5BYY" alt=""></p>
<p><img src="https://lh5.googleusercontent.com/k4O5SiNgQVZSifzf5cqraX26MgqmCcyEZgBqO0GI2Fj1efs7Rg3h1Ot7LI1PC_St9LLy9RcXinEwhaWKdtdsbwdlaZ06s59-kJRnrLNuVfys5ZzkiUBhdhgEsSx6QhAgKtwjNWJd" alt=""></p>
<p><img src="https://lh4.googleusercontent.com/wtXNEX22NrmxYvbpzYnOhWSJauvK0cE3M15lOemtZiBVwabp1ZOJMrtdJ6M2tTw3uQVyrzSIDu8645XWcVtXlFsigmhNelzmXsBGx4b7Dnh-QUIs6gzVFjjVRrc9mAJB7kalZblH" alt=""></p>
<p><img src="https://lh5.googleusercontent.com/kPrDw1wSTv-4p4RD3zrPL8Pv1wdiyaU9G0MvfKPPGfCCJJmUPhr5zEoyuLLhmBSiuKUbYNLR_OW7ug1qNBbbAIa57tVYdsRxk8t6z6wTR6ktBTVC31OIka0U2RC4qrdIKtu2hXq_" alt=""></p>
<p>The other half of our objective was to find similar information for a product (the most updated price, store domain, and availability) given a description of the product as an input rather than a UPC code. In order to do that, we used the Barcode Lookup API (<a href="https://www.barcodelookup.com/api">https://www.barcodelookup.com/api</a>), which allows for a simple product description as an input and returns similar information about the product. We also passed in geographic information specifying we were in the United States in order to restrict the search to stores within the nation. Since the input is less specific than a UPC code, the API returns many products that match the description, with all of the availability and price information. We decided to choose the product with the most availability information and return all of the relevant information for that product. Here are five examples with product descriptions that match the five product UPC examples from above:</p>
<p><img src="https://lh3.googleusercontent.com/JWVJCNeKW4Cufvuya7PZ6xotS_oF7gAubdmbHqD7r73yCUiNVQosXsJN2sfmtpxyBFEvy6hkvhuPG0yRKPLD3x_XEoZLPIKjsB7pEnNcYWDKp1Oo_BPOOKTPTfDfJRDi3kg3jz4R" alt=""></p>
<p><img src="https://lh6.googleusercontent.com/MaJTn4cvLInpooKY0z3QxjIkBTlv0hORhCZsE95xKbiP7GMQ22tSbuN3bOvU4KnTmfVbg1A054bSkt2RqpjbMy92HL14-OMQY8eTPOCeI3xTPM6brltd7m_ErIPLu2mu_4DYzczS" alt=""></p>
<p><img src="https://lh3.googleusercontent.com/3AkLlRwRgXTNrksDt8bjqzVZWFTObdIPESfQTSas_SeIBa65NlsFd_Ln104-vmvPJhgwl40Wklhj9dJku53F87eCm9Sxf-e_fqwX1Zcj2vq6xahoYJzPt4fs4M2dPrd4Q35mRdWH" alt=""></p>
<p><img src="https://lh5.googleusercontent.com/VkI1S6VeeG2HoDDw5Xg-lq3l5DaSgwkMNKERM54dfFZ_Qbx22QeVjOQJVCxGRE7VVAeYiOZPiWR4SeF7s6MjPwTSe272at_kPd9Hmh0Gc3xnuCY6z3oS8-7APBpVxUj7YOflMuB7" alt=""></p>
<p><img src="https://lh4.googleusercontent.com/RUcij3ixz2h51k9tq5gj2hz3zkLl578U28Lhrr_6av3QVEXpnst2Sov-afXESp1v9Afzlrzlf2vMPZICHZZZT6OiVWcQtMhDJD1-cWzF0kZVzVfI1y5hgWhr43a7aCxx42xK2tY2" alt=""><br>
<strong>Shopping_list.py</strong></p>
<p>Goal: Add a database that the user can add items to and remove items from with GUI on LCD</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/rgYBsJa-SDk" allowfullscreen=""></iframe>
<p>In the video, I…</p>
<ol>
<li>
<p>Add 2 Nutellas</p>
</li>
<li>
<p>Add 1 Peanut Butter</p>
</li>
<li>
<p>Remove 1 Nutella</p>
</li>
<li>
<p>Cycle through items</p>
</li>
<li>
<p>Restart ESP</p>
</li>
<li>
<p>Cycle through items (notice all items persist)</p>
</li>
<li>
<p>Remove all peanut butter</p>
</li>
<li>
<p>Remove all nutella</p>
</li>
<li>
<p>Cycle through items (all items removed, so nothing shows up)</p>
</li>
</ol>
<p>How this was done:</p>
<p>Server-side:</p>
<p>In the event of a POST request, we require 3 values: the string of the added item, the username of the user, and the quantity that we are adding or subtracting.</p>
<p>There are 2 cases to inserting an item:</p>
<ol>
<li>
<p>The item already exists in the database - In this case, we query the database to find the number of that item already inserted into the shopping list. We update the table with this item to add the quantity sent in the form to the quantity already in the table. In the case where the addition results in a nonpositive number, the script deletes that row from the table.</p>
</li>
<li>
<p>The item does not exist in the database - In this case, the script simply adds the item and its quantity to the table if the quantity is positive.</p>
</li>
</ol>
<p>Both the GET and POST requests return a string formatted with product names and their quantities separated by semicolons.</p>
<p>Additionally, this code finds the closest and cheapest stores to buy an item that is being added by using GPS coordinates provided by the user. Next, we pass the item name to another previously made piece of code (<a href="http://ProductInfo2.py">ProductInfo2.py</a>) that queries an API with an item description to find a list of stores where it is available and the price of the item at that store. Once we’ve gathered all of that information, we then decide which store is closest to the user and which contains the items at the cheapest price.</p>
<p>Then we iterate through the possible stores for the item and return the store where buying the items is cheapest. We also make use of another API (TomTom Maps) to find the distance from the user to each of the possible stores. Since some of the stores are just websites, we discard those as possibilities here. Then we iterate through the possible stores for the item and return the nearest store.</p>
<p><strong>Find_stores.py</strong><br>
Goal: Return the cheapest/closest stores to buy all the products in the shopping list</p>
<p>find_stores.py accesses the table products in the shopping_list3.db file. Depending on whether the goal is “close” or “cheap”, the script will return a list of new line seperated stores from the cheapest or closest columns of the table.</p>
<h3 id="arduino-feature-descriptions">Arduino Feature Descriptions</h3>
<p><strong>Display camera input on LCD Screen to allow for easy user feedback</strong></p>
<p>Here is a video of the deliverable: <a href="https://youtu.be/YdldBCKY-mY">https://youtu.be/YdldBCKY-mY</a> (embed)</p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/YdldBCKY-mY" allowfullscreen=""></iframe>
<p>There were two parts to this deliverable:</p>
<ol>
<li>
<p>Adding the ability to switch between camera modes (JPEG and BMP) since JPEG is much smaller and can be uploaded to the server and since it is easy to extract pixel values from BMP.</p>
</li>
<li>
<p>Reformatting the image data from the camera and sending it to the LCD.</p>
</li>
</ol>
<ol>
<li>Switching Camera Modes</li>
</ol>
<p>To allow for switching camera modes, we added button functionality that allows the user to press the button to take a photo and send it to the server. When the user is not pressing the button, it returns to display mode, where the output of the camera is displayed on the LCD screen. To convert between these two modes, we write to the registers found in the InitCAM ArduCAM function to specify which mode we would like to use.</p>
<ol start="2">
<li>Reformatting the image data from the camera and displaying it on LCD.</li>
</ol>
<p>The first challenge was figuring out what format the ArduCAM was returning the BMP data in and what format the LCD needed. It turns out that in both cases it was RGB 565. However, In the case of the ArduCAM, it was returning the low byte and then the high byte for each pixel when in fact, the LCD needed the concatenation of the high byte first with the low byte second. After realizing this, it was simply a matter of creating an array of these 16 bit RGB 565 values and using the pushImage function of the TFT to display them on the screen.</p>
<p><strong>Integrate product lookup systems so that the user can find the stores nearby that contain all (or most) of the items on their grocery list for the cheapest price</strong></p>
<iframe width="560" height="315" src="https://www.youtube.com/embed/Ysqn-ewu1WA" allowfullscreen=""></iframe>
<p>In this video, the user finds the closest and cheapest places to buy ritz crackers. Then, using Postman, they add nutella to the shopping list. This time, using cheapest, the location is the same. However, with the closest option, now both Walmart and Target are returned.</p>
<p>On the ESP side, we added another state for finding stores. In this state, long pressing the left button will change the screen to the main menu. Short pressing either of the buttons will alternate between selecting cheapest and closest stores. Long pressing the right button will confirm the selection and send a get request to the server side script. This request will be formatted with the selected goal (described further below) and the GPS’s longitude and latitude. Then, the output of the script will be displayed on the LCD, listing the stores.on.</p>
<p>On the server side, we made a request handler that integrates work from previous milestones. The request handler takes in three arguments - <code>goal</code>, <code>lat</code>, and <code>lon</code>. The latitude and longitude represent the user’s current location, while goal can be one of two arguments: ‘close’ or ‘cheap.’ The first thing the server does is read all items and corresponding quantities currently in the shopping list from the shopping list database that was already made. Next, for every item on the shopping list, we pass the item name to another previously made piece of code that queries an API with an item description to find a list of stores where it is available and the price of the item at that store. Once we’ve gathered all of that information, we then decide based on the value of <code>goal</code> whether to return a list of stores that are closest to the user or contain the items at the cheapest price.</p>
<p>If goal is “cheap”, then we iterate through the possible stores for each item and return a list of stores where buying the items is cheapest. If goal is “close”, then we make use of another API (TomTom Maps) to find the distance from the user to each of the possible stores. Since some of the stores are just websites, we discard those as possibilities here. Then we iterate through the possible stores for each item and return a list of the nearest stores.</p>
<h2 id="milestone-videos">Milestone Videos</h2>
<table>
  <tbody><tr>
  	<th>Week of</th>
    <th>Milestone</th>
    <th>Demo</th> 
  </tr>
  <tr>
  	<th>April 21</th>
    <td> </td>
    <td> </td>
  </tr>  
  <tr>
  	<th> </th>
    <td>Differentiate between image with barcode and regular image of product</td>
    <td>Show 10 images (from online or taken with phone) being processed by python script with output classifying the image as image with barcode or regular image</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Integrate Google vision API product recognition</td>
    <td>Show 5 different product photos (from online or taken with phone) being processed by a python script with output being the name of the product that can be used to query the store database</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Integrate barcode recognition</td>
    <td>Show 5 different barcode photos (from online or taken with phone) being processed by a python script with output being the UPC of the product</td>
      </tr><tr>
  	<th> </th>
    <td>Set up version control </td>
    <td>Show the github and log of commits</td>
  </tr>
  
  </tbody></table>
<br>
<iframe width="560" height="315" src="https://www.youtube.com/embed/-K_nC8TRn0g" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/c7TJMKSpVzs" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/sRBjfGFUZ5U" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/KBate2sTIU8" allowfullscreen=""></iframe>
<br>
<table>
  <tbody><tr>
  	<th>Week of</th>
    <th>Milestone</th>
    <th>Demo</th> 
  </tr>
  <tr>
  	<th>April 28</th>
    <td> </td>
    <td> </td>
  </tr>  
  <tr>
  	<th> </th>
    <td>Integrate all three systems from last week (full pipeline from user taking photo to deciding what type of photo it is and then outputting UPC or product info). Additionally, work to ensure robustness of this pipeline and that if one part fails (UPC code cannot be scanned for instance), it falls back on a different part of the system (image recognition for instance).
</td>
    <td>Show 5 photos already on the server that are taken as input to a python script and then either output the UPC (if barcode) or a product description (if not barcode)
</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Add camera to board and get photos uploading via post request.
</td>
    <td>Show photo taken on board uploaded to the server and then shown on a separate computer by initiating a GET request.</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Find product information (cheapest price, stores, and availability) using multiple APIs such as Walmart API, UPC code API, etc.</td>
    <td>Show as input 5 UPC labels and product descriptions and as output show the prices, stores, and availabilities of these products</td>
      </tr><tr>
  	<th> </th>
    <td>Add a database that the user can add items to and remove items from with GUI on LCD
</td>
    <td>Show LCD screen with list of items and ability to add or remove items. Show that this list of items remains unchanged even after the device is powercycled.</td>
  </tr>
  
  </tbody></table>
<br>
<iframe width="560" height="315" src="https://www.youtube.com/embed/_ELFTQUplBw" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/rgYBsJa-SDk" allowfullscreen=""></iframe>
<br>
<table>
  <tbody><tr>
  	<th>Week of</th>
    <th>Milestone</th>
    <th>Demo</th> 
  </tr>
  <tr>
  	<th>May 5</th>
    <td> </td>
    <td> </td>
  </tr>  
  <tr>
  	<th> </th>
    <td>Display camera input on LCD Screen to allow for easy user feedback</td>
    <td>Show input to camera being displayed on screen</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Integrate all image processing systems created so far so that user can take a photo, have it upload, and add the information to the grocery list which can then be modified for quantity/add and delete</td>
    <td>Show user taking photo and having the item appear on the grocery list.</td>
  </tr>
  <tr>
  	<th> </th>
    <td>Integrate product lookup systems so that the user can find the stores nearby that contain all (or most) of the items on their grocery list for the cheapest price
</td>
    <td>Show user looking up best store to buy the list of items from considering current location and price</td>
      </tr><tr>
  	<th> </th>
    <td>Display product look up results for the user on the LCD screen for confirmation before looking for stores</td>
    <td>Show input as image and having the LCD confirm it is the correct product</td>
  </tr>
  
  </tbody></table>
<br>
<iframe width="560" height="315" src="https://www.youtube.com/embed/YdldBCKY-mY" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/c5z2SvumvZ4" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/Ysqn-ewu1WA" allowfullscreen=""></iframe>
<br>
<table>
  <tbody><tr>
  	<th>Week of</th>
    <th>Milestone</th>
    <th>Demo</th> 
  </tr>
  <tr>
  	<th>May 12</th>
    <td> </td>
    <td> </td>
  </tr>  
  <tr>
  	<th> </th>
    <td>​Integrate all systems into one working product  </td>
    <td>Video showing the entire project including taking photos to items to add them to the grocery list (with both barcode recognition and google vision), confirming items, removing items from grocery list, finding optimal store (based on location and price), and changing quantity.</td>
  </tr>
  <tr>
  	<th> </th>
    <td>
​Solve blocking issues that occur while displaying photo on LCD
</td>
    <td>Show in video of system that performing a task of the ESP32 is not prevented while images are being displayed on the TFT</td>
  </tr>
  <tr>
  	<th> </th>
    <td>​Use store locator to choose optimal store based on distance and price</td>
    <td>Show a store being chosen and returned to the user from a list of stores based on some predetermined or user chosen metric of distance and price</td>
      </tr><tr>
  	<th> 
  </th></tr>
  </tbody></table>
<br>
<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/WTEpDErSpzQ" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/VVE-StDUMTc" allowfullscreen=""></iframe>
<iframe width="560" height="315" src="https://www.youtube.com/embed/-W_28i-_O3c" allowfullscreen=""></iframe>
</div>
</body>

</html>
