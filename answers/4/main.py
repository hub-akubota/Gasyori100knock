import numpy as np
import cv2

def otsu_thresh(img):
    max_var = -1
    max_th = 0
    for th in range(1, 254):
        m0 = img[img <= th].mean() if img[img <= th].size != 0 else 0 # mean class 0
        m1 = img[img > th].mean() if img[img > th].size != 0 else 0   # mean class 1
        w0 = img[img <= th].size   # pixel num class 0
        w1 = img[img > th].size    # pixel num class 1
        var = w0 * w1 / ((w0 + w1) ** 2) * ((m0 - m1) ** 2) # inter class variance
        if var > max_var:
            max_th = th
            max_var = var
    _, img = cv2.threshold(img, max_th, 255, cv2.THRESH_BINARY)
    return max_th, img

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256.png')
    img_gray = cv2.cvtColor(img_orig, cv2.COLOR_BGR2GRAY)
    th, img_result = otsu_thresh(img_gray)
    print('PYTHON Threshold: {}'.format(th))
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(3000)