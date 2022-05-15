from types import NoneType
import cv2
from tracker import *
from Adafruit_IO import Client
import threading

aio = Client('khanhtran01', 'aio_aVYg97AKI9fGh3lqSa9lShUqm48Z')
print("setup complete")
redlightControl = 0
mode = 0 # 0 is not count
shutdown = False

def receiveData():
    while True:
        data = aio.receive('red-light')
        print("receive complete")
        global redlightControl
        redlightControl = int(data.value)
        print(redlightControl)
        global mode
        if redlightControl == 1 and mode == 0:
            mode = 1
        elif redlightControl == 0 and mode == 1:
            mode = 2 # send data
        if shutdown == True:
            break


def carTracking():
    tracker = EuclideanDistTracker()

    cap = cv2.VideoCapture("highway.mp4")
    global mode
    # Object detection from Stable camera
    object_detector = cv2.createBackgroundSubtractorMOG2(history=100, varThreshold=40)
    count = 0
    while True:
        ret, frame = cap.read()
        if type(frame) is NoneType:
            break
        # Extract Region of interest
        roi = frame[650: 730,500: 840]

        # 1. Object Detection
        mask = object_detector.apply(roi)
        # mask = cv2.GaussianBlur(mask,(3,3), cv2.BORDER_DEFAULT)
        _, mask = cv2.threshold(mask, 200, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        detections = []
        for cnt in contours:
            # Calculate area and remove small elements
            area = cv2.contourArea(cnt)
            if area > 400:
                #cv2.drawContours(roi, [cnt], -1, (0, 255, 0), 2)
                x, y, w, h = cv2.boundingRect(cnt)
                detections.append([x, y, w, h])

        # 2. Object Tracking
        max = 0
        resetFlag = False
        if mode == 2:
            resetFlag = True
        boxes_ids = tracker.update(detections, resetFlag)
        for box_id in boxes_ids:
            x, y, w, h, id = box_id
            if id > max:
                max = id
            if mode == 1:
                count = max
            elif mode == 2:
                test = aio.feeds('bad-car')
                aio.send_data(test.key, count)
                mode = 0
            cv2.putText(roi, str(id+1), (x, y - 15), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)
            cv2.rectangle(roi, (x, y), (x + w, y + h), (0, 255, 0), 3)
        # mask = cv2.Canny(mask,0,1520)
        # cv2.imshow("roi", roi)
        cv2.imshow("Frame", frame)
        # cv2.imshow("Mask", mask)
        key = cv2.waitKey(30)
        if key == 27:
            break
        

    cap.release()
    cv2.destroyAllWindows()
    global shutdown
    shutdown = True

try:
    t1 = threading.Thread(target=receiveData)
    t2 = threading.Thread(target=carTracking)
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    print("done")
except:
    print("error")