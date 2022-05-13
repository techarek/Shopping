import sys
import os
sys.path.append('__HOME__/final_project/process_image')
import barcode_reader as bcr


folders = {'plain':'plain_barcodes', 'rotated': 'rotated_barcodes', 'product': 'product_barcodes'}

def request_handler(request):
	if request["method"] == "GET":
		try:
			folder = request["values"]["type"]
			num = request["values"]["num"]
		except:
			return "Invalid Request: Must have args type and num"

		folder_path = "__HOME__/final_project/process_image/test_images/{}/".format(folders[folder])
		all_photos_in_folder = os.listdir(folder_path)

		if num != "0":
			for photo in all_photos_in_folder:
				if "barcode{}".format(num) in photo:
					photo_path = folder_path + photo
					break
			
			img = bcr.get_image(photo_path)
			if folder == 'product':
				img = bcr.isolate_barcode(img)
			return bcr.get_UPC(img)
		else:
			barcodes = []
			for photo in all_photos_in_folder:
				photo_path = folder_path + photo
				img = bcr.get_image(photo_path)
				if folder == 'product':
					img = bcr.isolate_barcode(img)
				barcodes.append("".join(list(map(str,bcr.get_UPC(img)))))
			return barcodes


if __name__ == "__main__":
	r = {"method":"GET", "values":{"type":"product","num":"0"}}
	print(request_handler(r))
