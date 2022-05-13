import unittest
import sys
sys.path.append('__HOME__/final_project/process_image')
import barcode_reader as bcr

class PlainBarcode(unittest.TestCase):
    def test_barcode1(self):
    	img = bcr.get_image('test_images/plain_barcodes/barcode1.jfif')
    	self.assertEqual(bcr.get_UPC(img),[1,2,9,0,0,2,7,0,2,0,4,7])

    def test_barcode2(self):
    	img = bcr.get_image('test_images/plain_barcodes/barcode2.jpg')
    	self.assertEqual(bcr.get_UPC(img),[7,2,5,2,7,2,7,3,0,7,0,6])

    def test_barcode3(self):
    	img = bcr.get_image('test_images/plain_barcodes/barcode3.jpg')
    	self.assertEqual(bcr.get_UPC(img),[6,7,1,8,6,0,0,1,2,3,2,0])

    def test_barcode4(self):
    	img = bcr.get_image('test_images/plain_barcodes/barcode4.jpeg')
    	self.assertEqual(bcr.get_UPC(img),[1,2,3,4,5,6,7,8,9,0,1,2])

    def test_barcode5(self):
    	img = bcr.get_image('test_images/plain_barcodes/barcode5.png')
    	self.assertEqual(bcr.get_UPC(img),[0,3,6,0,0,0,2,9,1,4,5,2])

class RotatedBarcode(unittest.TestCase):
	def test_barcode1(self):
		img = bcr.get_image('test_images/rotated_barcodes/barcode1.jfif')
		self.assertEqual(bcr.get_UPC(img),[1,2,9,0,0,2,7,0,2,0,4,7])

	def test_barcode2(self):
		img = bcr.get_image('test_images/rotated_barcodes/barcode2.jpg')
		self.assertEqual(bcr.get_UPC(img),[7,2,5,2,7,2,7,3,0,7,0,6])

	def test_barcode3(self):
		img = bcr.get_image('test_images/rotated_barcodes/barcode3.jpg')
		self.assertEqual(bcr.get_UPC(img),[6,7,1,8,6,0,0,1,2,3,2,0])

	def test_barcode4(self):
		img = bcr.get_image('test_images/rotated_barcodes/barcode4.jpeg')
		self.assertEqual(bcr.get_UPC(img),[1,2,3,4,5,6,7,8,9,0,1,2])

	def test_barcode5(self):
		img = bcr.get_image('test_images/rotated_barcodes/barcode5.png')
		self.assertEqual(bcr.get_UPC(img),[0,3,6,0,0,0,2,9,1,4,5,2])

class ProductBarcode(unittest.TestCase):
	def test_barcode1(self):
		img = bcr.get_image('test_images/product_barcodes/barcode1.JPG')
		img = bcr.isolate_barcode(img)
		self.assertEqual(bcr.get_UPC(img),[3,8,1,3,7,0,0,1,5,3,1,4])

	def test_barcode2(self):
		img = bcr.get_image('test_images/product_barcodes/barcode2.JPG')
		img = bcr.isolate_barcode(img)
		self.assertEqual(bcr.get_UPC(img),[0,1,6,0,0,0,1,2,1,8,3,6])

	def test_barcode3(self):
		img = bcr.get_image('test_images/product_barcodes/barcode3.JPG')
		img = bcr.isolate_barcode(img)
		self.assertEqual(bcr.get_UPC(img),[0,4,6,5,0,0,0,2,2,5,1,7])

	def test_barcode4(self):
		img = bcr.get_image('test_images/product_barcodes/barcode4.JPG')
		img = bcr.isolate_barcode(img)
		self.assertEqual(bcr.get_UPC(img),[0,7,8,0,0,0,0,1,2,1,9,4])

	def test_barcode5(self):
		img = bcr.get_image('test_images/product_barcodes/barcode5.JPG')
		img = bcr.isolate_barcode(img)
		self.assertEqual(bcr.get_UPC(img),[0,4,3,6,1,9,5,4,0,1,2,3])


def main():
	unittest.main(verbosity=3, exit=False)

if __name__ == "__main__":
    main()
