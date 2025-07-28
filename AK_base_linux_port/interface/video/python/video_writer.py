import cv2
import numpy as np
import time
import os

FRAME_WIDTH = 192
FRAME_HEIGHT = 192
FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT * 2  # RGB565 2 bytes per pixel
OUTPUT_FILE = '/mnt/hdd/huy_workspace/Base_solution/AK_base_linux_port/interface/video/python/video_rgb565.raw'

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Failed to open webcam")
    exit()

# Create file with fixed size initially (optional)
if not os.path.exists(OUTPUT_FILE):
    with open(OUTPUT_FILE, 'wb') as f:
        f.write(b'\x00' * FRAME_SIZE)

with open(OUTPUT_FILE, 'r+b') as f:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to capture frame")
            break

        # Resize and convert BGR->RGB
        frame = cv2.resize(frame, (FRAME_WIDTH, FRAME_HEIGHT))
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Convert RGB888 -> RGB565
        r = (rgb[:, :, 0] >> 3).astype(np.uint16)
        g = (rgb[:, :, 1] >> 2).astype(np.uint16)
        b = (rgb[:, :, 2] >> 3).astype(np.uint16)
        rgb565 = (r << 11) | (g << 5) | b

        # Write at beginning of file
        f.seek(0)
        f.write(rgb565.astype('<u2').tobytes())
        f.flush()

        time.sleep(1 / 30)  # maintain ~30 FPS

cap.release()
