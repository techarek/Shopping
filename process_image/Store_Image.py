import json
import cv2
import struct
import sys
import numpy as np
sys.path.append('__HOME__/final_project/utilities')
import database as db
# sys.path.append('__HOME__/final_project/process_image')
# import Process_Image as process


photo_database = "__HOME__/final_project/.gitignore/photo.db"
#photo_database = "photo.db"
table = "jpeg"
def request_handler(request):
	last_section = False
	if not request["method"] == "POST":
		return "MUST BE POST REQUEST"
	if not request["is_json"]:
		return "MUST BE JSON"
	data = json.loads(request["data"])
	new_photo = bool(int(data["new"]))
	photo_text = data["photo"]

	if new_photo:
		db.delete_all_rows(photo_database,table)
	#print(photo_text)
	if "255,217" in photo_text:
		final_text_index = photo_text.find("255,217")
		last_section = True
		photo_text = photo_text[:final_text_index+8]

	values = list(map(int,photo_text.split(",")[:-1]))
	if not last_section:
		db.create_database(photo_database,table,"(data int)")
		for val in values:
			db.insert_into_database(photo_database,table,(val,))
	if last_section:
		rows = db.lookup_database(photo_database,table)
		img = [row[0] for row in rows] + values
		#change back to
		path = "__HOME__/final_project/.gitignore/tmp.jpg"
		tmpfile = open(path, "wb")
		#tmpfile = open("tmp.jpg", "wb")
		for i in img:
		    tmpfile.write(struct.pack("B",i))
		tmpfile.close()

	return "Completed"
