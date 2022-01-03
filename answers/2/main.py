import numpy as np
import cv2

def bgr2gray(img):
    _img = img.copy().astype(np.float32)
    gray = _img[..., 0] * 0.2126 + _img[..., 1] * 0.7152 + _img[..., 2] * 0.0722
    gray = np.clip(gray, 0, 255) # ndarrayを[0,255]に収める
    return gray.astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256.png')
    img_result = bgr2gray(img_orig)
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(3000)