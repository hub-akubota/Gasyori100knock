import numpy as np
import cv2

def filter_sobel(img, k_size=(3, 3), mode='x'):
    k = np.zeros(k_size)
    if mode == 'x':
        k[:, 0] = 1
        k[k_size[0] // 2, 0] = 2
        k[:, -1] = -1
        k[k_size[0] // 2, -1] = -2
    else:
        k[0] = 1
        k[0, k_size[0] // 2] = 2
        k[-1] = -1
        k[-1, k_size[0] // 2] = -2
    out = cv2.filter2D(img.astype(np.float32), -1, k)
    out -= out.min()  # normalize > [0, 1]
    out /= out.max()
    return out

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256.png')
    img_gray = cv2.cvtColor(img_orig, cv2.COLOR_BGR2GRAY)
    img_result = filter_sobel(img_gray, (3, 3))
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(3000)
