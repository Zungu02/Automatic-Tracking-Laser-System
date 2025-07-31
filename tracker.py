#!/usr/bin/env python3
import cv2
import time
import serial
import numpy as np
from picamera import PiCamera
from picamera.array import PiRGBArray

# ——— Configuration ———
SERIAL_PORT      = '/dev/serial0'
BAUD_RATE        = 115200

FRAME_W, FRAME_H = 640, 480
FPS              = 10

PROTO_TXT = 'MobileNetSSD_deploy.prototxt.txt'
MODEL     = 'MobileNetSSD_deploy.caffemodel'

CONF_THRESHOLD   = 0.3
PERSON_CLASS_ID  = 15

BRIGHTNESS = 90
CONTRAST   = 90
ISO        = 1000

# ——— Initialize UART ———
try:
    uart = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print("Serial connected")
except Exception as e:
    print("UART init failed:", e)
    exit(1)

# ——— Load DNN model ———
net = cv2.dnn.readNetFromCaffe(PROTO_TXT, MODEL)

# ——— Setup PiCamera ———
camera = PiCamera()
camera.resolution = (FRAME_W, FRAME_H)
camera.framerate = FPS
camera.brightness = BRIGHTNESS
camera.contrast = CONTRAST
camera.iso = ISO
rawCapture = PiRGBArray(camera, size=(FRAME_W, FRAME_H))
time.sleep(1)

print("Starting tracking (press Ctrl+C to stop)...")

try:
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        image = frame.array
        blob = cv2.dnn.blobFromImage(image, 0.007843, (300, 300), 127.5)
        net.setInput(blob)
        detections = net.forward()

        h, w = FRAME_H, FRAME_W
        best_conf = 0
        best_box = None

        for i in range(detections.shape[2]):
            conf = float(detections[0, 0, i, 2])
            class_id = int(detections[0, 0, i, 1])
            if class_id == PERSON_CLASS_ID and conf > CONF_THRESHOLD and conf > best_conf:
                box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
                best_conf = conf
                best_box = box.astype("int")

        status = "SEARCH"
        if best_box is not None:
            status = f"TRACK ({best_conf:.2f})"
            x1, y1, x2, y2 = best_box
            cx = (x1 + x2) // 2
            cy = (y1 + y2) // 2

            # Draw box & center
            cv2.rectangle(image, (x1, y1), (x2, y2), (0,255,0), 2)
            cv2.circle(image, (cx, cy), 5, (0,0,255), -1)

            # Send center position to ESP32
            uart.write(f"POS:{cx},{cy}\n".encode('ascii'))
        else:
            # No detection: Send IDLE command
            uart.write(b"IDLE\n")

        # Display tracking info
        cv2.putText(image, status, (10,30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0,0,255), 2)
        cv2.imshow("Body Tracking", image)

        rawCapture.truncate(0)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("\nUser interrupted")

finally:
    camera.close()
    cv2.destroyAllWindows()
    uart.close()
    print("Clean exit")
