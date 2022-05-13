import cv2 as cv
import numpy as np
from math import pi, log

BARCODE_CONVERSION = {3211: 0,2221: 1, 2122: 2, 1411: 3, 1132: 4, 1231: 5, 1114: 6, 1312: 7, 1213: 8, 3112: 9}

def get_image(src):
	"""Read in the image file from its location in memory and return image object.
	
	Parameters: 
		src (string): filepath to image

	Returns:
		ndarray: image in RGB format
	"""

	return cv.imread(src)

def preprocess_image(image):
	"""Takes a cropped image of a barcode and returns a white-padded image with straightened bars.

	
	Parameters:
		image (ndarray): image in RGB format

	Returns:
		ndarray: white padded image with straightened black bars in GRAY format
	"""

	gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY) 
	ret,BW = cv.threshold(gray,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
	straight = reorient(BW)
	#vert = create_vertical_lines(straight)
	padded = cv.copyMakeBorder(straight,5,5,5,5,cv.BORDER_CONSTANT,value = 255)
	#ret,BW = cv.threshold(padded,127,255,0)
	return padded

def create_vertical_lines(image,threshold = (255//2)+50):
	"""Takes an image and fills every column that has an average value of less than threshold with black and every other column with white.

	Parameters:
		image (ndarray): GRAY formatted image
		threshold (int): average value that a column must exceed to be filled with white

	Returns:
		ndarray: black and white GRAY formatted image with each column being entirely white or black based on average value compared to threshold
	"""

	w,h = image.shape
	vline_colors = np.where(np.average(image,axis = 0)>=threshold,255,0)
	return np.uint8(vline_colors*np.ones((w,h)))


def draw_contours(gray_image,contours,cnt_color = (0,255,0), thickness = 1):
	"""Draws given contours on image.

	Parameters:
		gray_image (ndarray): GRAY formatted image to have contours drawn on
		contours (list): List of contours in cv contours format
		cnt_color (tuple): RGB value to draw contour in
		thickness (int): thickness to draw contours in


	Return:
		ndarray: image in RGB format with contours drawn on it in cnt_color 
	"""

	color = cv.cvtColor(gray_image,cv.COLOR_GRAY2BGR)
	cv.drawContours(color,contours,-1,cnt_color, thickness)
	return color
	
def show_image(image):
	"""Shows image in new window titled image and waits for any keystroke to terminate.

	Parameters:
		image (ndarray): image to be displayed
	"""

	cv.imshow("image",image)
	cv.waitKey(0)

def make_box(rectangles):
	"""Converts list of rectangles to np array of integer corner points 

	Parameters:
		rectangles (array-like): list of rectangles in cv minAreaRectangle format

	Returns: 
		np array: list containing corner points of each rectangle in a tuple
	"""
	return np.array([np.int0(cv.boxPoints(tuple(rect))) for rect in rectangles])

def classify(widths):
	"""Takes a series of single colored (either black or white) bar widths from barcode and 
	converts it into a list of classifications, where each bar width is classified into one 
	of {1,2,3,4} based on relative width.

	Parameters:
		widths (list): list of floats representing the widths of bars

	Returns:
		np array: list of same length of widths where each element represents the classification 
					the corresponding width in widths

	 """

	minimum = (widths[0]+widths[-1])/2
	first_bin = np.average([x for x in widths if round(x/minimum)==1])
	second_bin = np.average([x for x in widths if round(x/first_bin) == 2])
	step_size = np.max([second_bin/2,first_bin])
	bins = np.array([first_bin,second_bin,3*step_size,4*step_size])
	
	clsfy = np.vectorize(lambda x: np.argmin(np.abs(bins-np.array([x]*4)))+1)
	
	return clsfy(widths)

def space_between_rects(rects):
	"""Finds the amount of space between each consecutive rectangle
	
	Parameters: 
		rects (ndarray): list of rectangles in cv minAreaRectangle format

	Returns:
		np array: element i corresponds to the distance between rects[i] and rects[i+1]
	"""

	return np.array([rects[i+1][0][0]-rects[i][0][0]-(rects[i+1][1][0]+rects[i][1][0])/2 for i in range(len(rects)-1)])

def interleave(list1,list2):
	"""Interleaves two lists

	Paraemters:
		list1 (np array): first list of size n to be interleaved 
		list2 (np array): second list of size n or n-1 to be interleaved

	Returns:
		np array: Both lists interleaved with first list going first.
	"""

	out = np.empty((list1.size+list2.size))
	out[::2] = list1
	out[1::2] = list2
	return out

def reverse(num):
	num_str = str(num)
	num_str = num_str[::-1]
	return int(num_str)

def get_closest_codes(num):
	min_dist = 16
	closest_codes = [] 
	for code in BARCODE_CONVERSION.keys():
		distance = sum([abs(int(n)-int(m)) for n,m in zip(str(code),str(num))])
		if distance < min_dist:
			closest_codes = [code]
			min_dist = distance
		elif distance == min_dist:
			closest_codes.append(code)

	return [BARCODE_CONVERSION[code] for code in closest_codes]


def code_to_num(code):
	"""Returns barcode number corresponding to 4 digit barcode code

	Parameters:
		code (int): four digit code to be translated

	Returns:
		int: barcode number corresponding to code
	"""

	if code in BARCODE_CONVERSION:
		return BARCODE_CONVERSION[code]
	elif reverse(code) in BARCODE_CONVERSION:
		return BARCODE_CONVERSION[reverse(code)]
	else:
		return -1

def recover_barcode(translated,original):
	check = (-3*sum([i for i in translated[0::2] if not i == -1]) - sum([i for i in translated[1:11:2] if not i == -1]))%10
	indices = [i for i,num in enumerate(translated) if num == -1]
	substitutes = [get_closest_codes(original[i]) for i in indices]
	find_matching(indices,substitutes,check,translated[-1],translated)
	return translated


def find_matching(indices,substitutes,check,checksum,translated):
	if len(substitutes) == 0:
		return True
	for substitute in substitutes[0]:
		if indices[0] % 2 == 1 and indices[0] < 11:
			new_check = (check - substitute)%10
		elif indices[0] == 11:
			checksum = substitute
			new_check = check
		else:
			new_check = (check - 3*substitute)%10

		if len(indices)>1:
			translated[indices[0]] = substitute
			if find_matching(indices[1:],substitutes[1:],new_check,checksum,translated):
				return True
			translated[indices[0]] = -1
		elif checksum == new_check: #base case
			translated[indices[0]] = substitute
			return True
	return False


def translate_to_numbers(list1):
	"""Takes list of sequential classified barcode widths and translates them to a UPC number. Puts -1 in places where the set of four digits doesn't correspond to a code.

	Parameters: 
		list1 (list): list of values in {1,2,3,4} where every four values corresponds to a digit in the UPC number

	Returns:
		list: list with each element being a the next digit of the UPC number
	"""

	if len(list1) % 4 != 0:
		return [-1]*12

	code = np.int0((np.array([1000,100,10,1]*(len(list1)//4))*list1))
	code = code.reshape((len(list1)//4,4))
	code = np.sum(code,axis=1)
	translate = np.vectorize(code_to_num)
	translated = translate(code)
	
	if -1 in translated:
		translated = recover_barcode(translated,code)

	return translated

def straighten_rectangles(rects):
	"""Takes all rectangles that are more than -45 degrees tilted and re-encodes them as a rectangle with angle 90 + original angle

	Parameters:
		rects (array-like): list of rectangles in cv minAreaRect format

	Returns:
		list: Same list of rectangles where no rectangle has angle less than -45 degrees. 
	"""

	def flip_rect(rect):
		if rect[2] < -45:
			return (rect[0],(rect[1][1],rect[1][0]),rect[2]+90)
		return rect

	return [flip_rect(rect) for rect in rects]

def get_barcode_rectangles(processed_image):
	"""Takes a cropped GRAY image of a barcode and returns a list of rectangles representing the black bars on the barcode

	Parameters:
		processed_image (ndarray): image in GRAY format of barcode, already cropped and preprocessed (see preprocess image)

	Returns:
		np array: rectangles corresponding to black barcode bars in cv minAreaRect format sorted by increasing x value in image 
	"""
	
	contours, hierarchy = cv.findContours(processed_image, mode=cv.RETR_LIST, method=cv.CHAIN_APPROX_SIMPLE)

	rectangles = np.array([cv.minAreaRect(cnt) for cnt in contours])
	rectangles = straighten_rectangles(rectangles)

	image_with_barcodes = draw_contours(processed_image,make_box(rectangles))

	barcode_rectangles = np.array([rect for rect in rectangles if rect[1][0] != 0 and rect[1][1]/rect[1][0] >= 5]) #changed 5 to 3
	image_with_barcodes = draw_contours(processed_image,make_box(barcode_rectangles))

	barcode_center_y = np.median([rect[0][1] for rect in barcode_rectangles])
	barcode_height = np.median([rect[1][1] for rect in barcode_rectangles])

	barcode_rectangles = np.array([rect for rect in barcode_rectangles if np.abs(rect[0][1]-barcode_center_y) < barcode_height/2])
	image_with_barcodes = draw_contours(processed_image,make_box(barcode_rectangles))

	ordered_barcode_rectangles = np.array(sorted(barcode_rectangles,key=lambda x:x[0][0]))
	return ordered_barcode_rectangles

def measure_bars(processed_image):
	h,w = processed_image.shape
	heights = [h//3,h//2,2*h//3]
	binary_image = processed_image>0

	widths = []
	for height in heights:
		width = []
		row = binary_image[height]
		prev = 1
		prev_transition_index = 0
		for i in range(len(row)):
			if row[i] != prev:
				width.append(i - prev_transition_index)
				prev = row[i]
				prev_transition_index = i
		if len(width) == 60:
			widths.append(width[1:-1])
	return np.average(widths,axis=0)

def classify_widths(widths):
	gaps = classify(widths[1::2])
	bars = classify(widths[0::2])
	full_code = interleave(bars,gaps)
	full_code = np.concatenate((full_code[3:27],full_code[32:56]))
	return full_code

def classify_bars(ordered_barcode_rectangles):
	"""Takes a list of black barcode rectangles and returns the classified widths of all of the bars in the image (black and white) in increasing x order

	Parameters:
		ordered_barcode_rectangles (array-like): rectangles in cv minAreaRect format corresponding to black bars in barcode in increasing x order in image

	Returns:
		np array: list of bar-width classifications for all bars in barcode in increasing x order
	"""

	gaps = classify(space_between_rects(ordered_barcode_rectangles))
	bars = classify(np.array([rect[1][0] for rect in ordered_barcode_rectangles]))
	full_code = interleave(bars,gaps)
	full_code = np.concatenate((full_code[3:27],full_code[32:56]))
	return full_code


def get_UPC(img):
	"""Returns UPC code for cropped barcode image

	Parameters:
		img (ndarra): image object of cropped barcode

	Returns:
		list: UPC barcode where each digit is an element in the list
	"""
	processed_image = preprocess_image(img)
	#ordered_barcode_rectangles = get_barcode_rectangles(processed_image)
	#full_code = classify_bars(ordered_barcode_rectangles)
	widths = measure_bars(processed_image)
	full_code = classify_widths(widths)
	return list(translate_to_numbers(full_code))

def reorient(img):
	"""Takes rotated image of cropped barcode and rotates so that bars are vertical

	Parameters:
		img (ndarray): cropped image of rotated barcode

	Returns:
		ndarray: straightened barcode image
	"""
	rows,cols = img.shape
	img = cv.copyMakeBorder(img,5,5,5,5,cv.BORDER_CONSTANT,value = 255)
	rectangles = get_barcode_rectangles(img)
	median_angle = np.median([rect[2] for rect in rectangles])
	M = cv.getRotationMatrix2D((cols/2,rows/2),median_angle,1)
	dst = cv.warpAffine(img,M,(cols,rows),borderMode = cv.BORDER_CONSTANT,borderValue = 255)
	return dst


def blank_mask(gray_img):
	"""Returns black image in GRAY format the shape of gray_img

	Parameters:
		gray_img (ndarray): image in GRAY format for mask to be the same size as

	Returns:
		ndarray: image the same size as gray_img with all pixels black
	"""

	_,mask = cv.threshold(np.uint8(gray_img),255,255,cv.THRESH_BINARY)
	return mask

def std_rect(endpts):
	"""Returns the smallest rectangle whose edges are parallel to image edges that completely encompasses all points in endpts

	Parameters:
		endpts (list): list of endpoints to construct rectangle around

	Returns:
		tuple: minimum x value, maximum x value, minimum y value, maximum y vale
	"""

	x_vals = [pt[0] for pt in endpts]
	y_vals = [pt[1] for pt in endpts]
	return (min(x_vals),max(x_vals),min(y_vals),max(y_vals))


def isolate_barcode(img):
	"""Takes image file containing a barcode and returns an image file of the cropped barcode

	Parameters:
		img (ndarray): image with barcode in BGR format

	Returns:
		ndarray: cropped image of barcode in BGR format
	"""
	img = cv.resize(img,(1920,1080))
	gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
	ret,BW = cv.threshold(gray,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
	edged = cv.Canny(BW, 50, 100, 2)
	blurred = cv.GaussianBlur(edged, (3, 3),0)#last gray

	rectangles = get_barcode_rectangles(blurred)
	BW = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
	mask = blank_mask(BW)
	boxes = draw_contours(mask,make_box(rectangles),thickness = 5)
	boxes = cv.GaussianBlur(boxes, (15, 15),0)

	boxes = cv.dilate(boxes,None,iterations = 35)

	gray_boxes = cv.cvtColor(boxes, cv.COLOR_BGR2GRAY)
	_,BW_boxes = cv.threshold(np.uint8(gray_boxes),100,255,cv.THRESH_BINARY)

	contours, hierarchy = cv.findContours(BW_boxes, mode=cv.RETR_LIST, method=cv.CHAIN_APPROX_SIMPLE)
	rectangles = np.array([cv.minAreaRect(cnt) for cnt in contours])
	largest_barcode_rectangle = np.array(sorted(rectangles,key=lambda x:x[1][0]*x[1][1],reverse=True))[0]

	minx, maxx, miny, maxy = std_rect(np.int0(cv.boxPoints(tuple(largest_barcode_rectangle))))
	width = maxx - minx
	height = maxy - miny

	if minx<0:
		minx = 0
	if miny < 0:
		miny=0

	#barcode = img[max(int(miny - 0.1*width),0) :min(int(maxy + 0.1*width),1920),minx:maxx)

	barcode = img[miny:maxy,minx:maxx]
	
	return barcode