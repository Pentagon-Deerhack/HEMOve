import cv2
import json
import requests
import time
from requests.structures import CaseInsensitiveDict

url = "https://demo.thingsboard.io/api/v1/Yn5VHwYZhjyl8zpcHF4T/telemetry"
headers = CaseInsensitiveDict()
headers["Content-Type"] = "application/json"

# Initialize the cv2 QRCode detector
detector = cv2.QRCodeDetector()

# Open the webcam
cap = cv2.VideoCapture(0)

# Set the scanner window dimensions
scan_width = 300
scan_height = 300

# Set the position of the scanner window
scan_x = int((cap.get(cv2.CAP_PROP_FRAME_WIDTH) - scan_width) / 2)
scan_y = int((cap.get(cv2.CAP_PROP_FRAME_HEIGHT) - scan_height) / 2)

# Set the delay time in seconds
delay_time = 5

# Set the start time to the current time
start_time = time.time()

while True:
    # Read the image from the webcam
    ret, img = cap.read()

    # Create a gray version of the image
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Detect and decode the QR code in the image
    data, bbox, _ = detector.detectAndDecode(img)

    # If a QR code is detected
    if data and (time.time() - start_time) >= delay_time:
        # Print the decoded data to the console
        print("Data:", data)

        # Convert the data string to a dictionary
        data_dict = eval(data)

        # Loading the json data
        json_data = json.loads(data)

        # Extracting the keys and values from the json data
        keys = []
        values = []
        for key, value in json_data.items():
            keys.append(key)
            values.append(value)

        # Constructing the data string to be sent
        data = {}
        for i in range(len(keys)):
            data[keys[i]] = values[i]

        # Sending the data to Thingsboard
        response = requests.post(url, headers=headers, json=data)
        print(response.status_code)
        
        # Set the start time to the current time
        start_time = time.time()

    # Draw the scanner window
    cv2.rectangle(img, (scan_x, scan_y), (scan_x+scan_width, scan_y+scan_height), (255, 0, 0), 2)

    # Show the image in a window
    cv2.imshow("QR Code Scanner", img)

    # Exit the loop if the "s" key is pressed
    if cv2.waitKey(1) & 0xFF == ord("s"):
        break
# Release the webcam and close the window
cap.release()
cv2.destroyAllWindows()
