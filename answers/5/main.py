import numpy as np
import cv2

def bgr2hsv(img):
    tmp = img.copy().astype(np.float32)
    v_max = tmp.max(axis=2)
    v_min = tmp.min(axis=2)
    v_argmin = tmp.argmin(axis=2) # v_minのindex
    bgr = cv2.split(tmp)
    b = bgr[0]
    g = bgr[1]
    r = bgr[2]
    tmp = np.zeros_like(tmp, dtype=np.float32)

    diff = np.maximum(v_max - v_min, 1e-10)

    # Hue
    index = v_argmin == 0 # B
    tmp[..., 0][index] = 60 * (b - g)[index] / diff[index] + 60
    index = v_argmin == 1 # G
    tmp[..., 0][index] = 60 * (r - b)[index] / diff[index] + 300
    index = v_argmin == 2 # R
    tmp[..., 0][index] = 60 * (b - g)[index] / diff[index] + 180
    index = v_max == v_min
    tmp[..., 0][index] = 0
    # Saturation
    tmp[..., 1] = v_max - v_min
    # Value
    tmp[..., 2] = v_max
    return tmp

def hsv2bgr(img):
    hsv = cv2.split(img)
    _h = hsv[0] / 60
    s = hsv[1]
    v = hsv[2]
    x = s * (1 - np.abs(_h % 2 - 1))
    z = np.zeros_like(x)
    vals = np.array([[z, x, s], [z, s, x], [x, s, z], [s, x, z], [s, z, x], [x, z, s]])

    tmp = np.zeros_like(img)

    for i in range(6):
        index = _h.astype(int) == i
        for j in range(3):
            tmp[..., j][index] = (v - s)[index] + vals[i, j][index]

    return np.clip(tmp, 0, 255).astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256.png')
    img_hsv = bgr2hsv(img_orig)
    #img_hsv[..., 0] = (img_hsv[..., 0] + 180) % 360
    img_result = hsv2bgr(img_hsv)
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(3000)