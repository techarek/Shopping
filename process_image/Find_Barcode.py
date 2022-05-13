import base64
import numpy as np
import cv2


def find_area(points):
    '''
    finds area of a rectangle given a list of 4 bounding points

    points: a length 4 list of points (each point is a tuple of (x,y) coordinates

    returns: area of rectangle
    '''
    first_triangle = np.array([[points[0][0], points[1][0], points[2][0]],
                           [points[0][1], points[1][1], points[2][1]],
                           [1, 1, 1]])

    sec_triangle = np.array([[points[0][0], points[3][0], points[2][0]],
                           [points[0][1], points[3][1], points[2][1]],
                           [1, 1, 1]])

    area = 0.5 * (abs(np.linalg.det(first_triangle)) + abs(np.linalg.det(sec_triangle)))

    return area


def thing(image, direction):
    '''
    Determines if a barcode can be found in an image

    image_base64: an image encoded in base64
    direction: 'Y' if we want to read vertical barcode

    returns: False if no barcode in image,
             True if barcode in image
    '''
    # load image

    # make image greyscale
    greyedOut = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # find x and y gradients
    dx = cv2.Scharr(greyedOut, ddepth=cv2.CV_32F, dx=1, dy=0)
    dy = cv2.Scharr(greyedOut, ddepth=cv2.CV_32F, dx=0, dy=1)



    # combine gradients by subtracting
    der = cv2.subtract(dy, dx)
    der = cv2.convertScaleAbs(der)

    # blur gradient version of image so that you fill in gaps made by lines of barcode
    blurred = cv2.blur(der, (5, 5))
    thresh = cv2.threshold(blurred, 225, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)[1]

    # this allows us to blur vertically or horizontally, depending on direction param
    if direction == "Y":
        kernel = np.ones((20, 10))
    else:
        kernel = np.ones((10, 20))
    closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

    # erode and dilate image to get rid of extra, small white dots and expand 'meaningful' white blobs
    eroded = cv2.erode(closed, None, iterations=25)
    dilated = cv2.dilate(eroded, None, iterations=25)

    # find biggest (ie. most significant) blob
    contours = cv2.findContours(dilated, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]
    try:
        choice = sorted(contours, key=cv2.contourArea, reverse=True)[0]
    except:
        return False

    # finds bounding points of what was determined to be the barcode
    rect = cv2.minAreaRect(choice)
    box = cv2.boxPoints(rect)
    image_area = image.shape[0] * image.shape[1]

    return find_area(box)/image_area > 0.001


def request_handler(request):
    image_b64 = request.get("form").get("image")
    return thing(image_b64, "Y") or thing(image_b64, "X")
