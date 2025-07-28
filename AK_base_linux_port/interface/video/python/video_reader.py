import time
import numpy as np
import cv2
import os

FRAME_WIDTH = 192
FRAME_HEIGHT = 192
BYTES_PER_PIXEL = 2
FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT * BYTES_PER_PIXEL
INPUT_FILE = '/mnt/hdd/huy_workspace/Base_solution/AK_base_linux_port/interface/video/python/video_rgb565.raw'

def read_frame(file_path):
    try:
        with open(file_path, 'rb') as f:
            data = f.read(FRAME_SIZE)
            if len(data) != FRAME_SIZE:
                return None  # incomplete frame
            return data
    except FileNotFoundError:
        return None

def rgb565_to_bgr(raw_bytes):
    # Convert raw bytes to numpy array of uint16
    frame = np.frombuffer(raw_bytes, dtype='<u2').reshape((FRAME_HEIGHT, FRAME_WIDTH))

    # Extract RGB components from RGB565
    r = ((frame >> 11) & 0x1F) << 3
    g = ((frame >> 5) & 0x3F) << 2
    b = (frame & 0x1F) << 3

    # Stack into RGB image
    rgb = np.dstack((r, g, b)).astype(np.uint8)

    # Convert RGB to BGR for OpenCV display
    bgr = cv2.cvtColor(rgb, cv2.COLOR_RGB2BGR)
    return bgr

def main():
    print("Starting RGB565 frame reader and display...")
    while True:
        frame_bytes = read_frame(INPUT_FILE)
        if frame_bytes is None:
            print("Frame not ready or incomplete, waiting...")
            time.sleep(0.05)
            continue

        img = rgb565_to_bgr(frame_bytes)
        cv2.imshow('RGB565 Stream', img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        time.sleep(1 / 30)  # limit to ~30 FPS

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
