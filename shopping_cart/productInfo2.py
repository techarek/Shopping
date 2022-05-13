import random
import string
import sqlite3
import urllib.request
import requests
import json

def getProductName(data):
	try:
		upc = int(data)
		r = requests.get("""https://api.upcitemdb.com/prod/trial/lookup?upc={}""".format(upc,))
		response = r.json()
		return response['items'][0]["title"]
	except ValueError:
		return "MUST BE VALID UPC CODE"

def getProduct(data):
	#processing barcode
	try:
		upc = float(data)
		# send get request to UPC API with the barcode number
		r = requests.get("""https://api.upcitemdb.com/prod/trial/lookup?upc={}""".format(upc,))
		response = r.json()

		to_return = {}
		
		for item in response['items']:
			#print("Item name: {}\t Brand: {}\t Lowest price: {} \t Highest price: {}".format(item['title'], item['brand'], item['lowest_recorded_price'], item['highest_recorded_price']))

			#print("OFFERS:")
			#print("---------------------------")
			for offer in item['offers']:
				#print("Website: " + offer['domain'])
				#print("Product: " + offer['title'])
				#print("Price: " + str(offer['price']))

				if offer['availability'] == "":
					#print("Availability: Yes")
					if offer['price'] != 0:
						to_return[offer['merchant']] = offer['price']
				else:
					pass
					#print("Availability: Out of stock")


		return to_return

	#processing product name
	except ValueError:
		product_description = data.replace(" ", "%20")
		key = "h8xthm5zm7fmc4125gf9xpjnfliz65"

		# send get request to API with the product name
		url = "https://api.barcodelookup.com/v2/products?key={}&geo=us&search={}".format(key, product_description)
		# print(url)
		with urllib.request.urlopen(url) as url:
			response = json.loads(url.read().decode())

		max_stores = 0
		best_index = 0
		for i in range(len(response['products'])):
			if len(response["products"][i]['stores']) > max_stores:
				max_stores = len(response["products"][i]['stores'])
				best_index = i

		to_return = {}
		#print("ITEM DESCRIPTION")
		#print(response["products"][best_index]['description'], '\n')
		
		#print("OFFERS:")
		#print("---------------------------")
		stores = response["products"][best_index]['stores']

		for store in stores:
			#print("STORE NAME", store['store_name'])
			#print("PRICE:", store['currency_symbol'] + store['store_price'])
			#print("URL:", store['product_url'])
			#print("\n")
			to_return[store['store_name']] = store['store_price']

		return to_return


#if __name__ == "__main__":
	#getProduct("kinder bueno")
	#print(getProduct('765667527931'))
	#print(getProduct('705911703599'))
