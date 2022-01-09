import numpy as np
import cv2

def filter_LoG(img, k_size=(3, 3), sigma=1.2):
    # kernel
    k = np.zeros(k_size, dtype=np.float32)
    pad_x = k_size[0] // 2
    pad_y = k_size[1] // 2
    for x in range(-pad_y, -pad_y + k_size[1]):
        for y in range(-pad_x, -pad_x + k_size[0]):
            k[y + pad_y, x + pad_y] = (x ** 2 + y ** 2 - 2 * (sigma ** 2)) * np.exp( - (x ** 2 + y ** 2) / (2 * (sigma ** 2)))
    k /= k.sum()

    out = cv2.filter2D(img.astype(np.float32), -1, k)
    out -= out.min() # normalize > [0, 1]
    out /= out.max()
    return out

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256_noise.png')
    img_result = filter_LoG(img_orig, k_size=(5, 5), sigma=3)
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(3000)
