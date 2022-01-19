import numpy as np
import cv2
import matplotlib.pyplot as plt
import sys
from answers.hist import *


def bilinear_inter(img, a, b):
    h, w, c = img.shape
    out_h = int(h * a)
    out_w = int(w * b)

    xs, ys = np.meshgrid(range(out_w), range(out_h)) # output image index

    _xs = np.floor(xs / b).astype(int) # original x
    _ys = np.floor(ys / a).astype(int) # original y

    dx = xs / b - _xs
    dy = ys / a - _ys

    dx = np.repeat(np.expand_dims(dx, axis=-1), c, axis=-1) # repeat channel
    dy = np.repeat(np.expand_dims(dy, axis=-1), c, axis=-1) # repeat channel

    _xs1p = np.minimum(_xs + 1, w - 1)
    _ys1p = np.minimum(_ys + 1, h - 1)


    out = (1 - dx) * (1 - dy) * img[_ys, _xs] + dx * (1 - dy) * img[_ys, _xs1p] + \
                    (1 - dx) * dy * img[_ys1p, _xs] + dx * dy * img[_ys1p, _xs1p]

    return np.clip(out, 0, 255).astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256.png')
    img_result = bilinear_inter(img_orig, a=1.5, b=1.5)

    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(0)
