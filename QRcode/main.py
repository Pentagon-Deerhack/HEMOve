#importing the module
import numpy as np
import cv2


#reading the Qr code image
img = cv2.imread("qr-1.png")

#initializing the cv2 QRCodeDetector
detector = cv2.QRCodeDetector()

#detecting and decoding the Qr code
data, bbox, straight_qrcode = detector.detectAndDecode(img)

#checking if the image is of a QRCode or not
if bbox is not None:
    print(data)
