#importing the module
import numpy as np
import cv2
import json


#reading the Qr code image
img = cv2.imread("qr-1.png")

#initializing the cv2 QRCodeDetector
detector = cv2.QRCodeDetector()

#detecting and decoding the Qr code
data, bbox, straight_qrcode = detector.detectAndDecode(img)

#checking if the image is of a QRCode or not
if bbox is not None:
    print(data)


json_data = json.loads(data)


keys = []
values = []
items = json_data.items()
for item in items:
    keys.append(item[0]), values.append(item[1])


print("keys : ", str(keys))
print("values : ", str(values))