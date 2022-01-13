import numpy as np
import cv2
import matplotlib.pyplot as plt
import sys
from answers.hist import *

def hist_equ(img):
    out = np.zeros_like(img, dtype=np.float32)
    s = img.size
    x_max = 255
    h = 0
    for i in range(256):
        h += (img == i).sum()
        out[img == i] = x_max / s * h
    return np.clip(out, 0, 255).astype(np.uint8)

def hist_equ_rgb(img):
    out = np.zeros_like(img, dtype=np.float32)
    s = img.shape[0] * img.shape[1]
    x_max = 255
    for c in range(3):
        h = 0
        for i in range(256):
            h += (img[..., c] == i).sum()
            out[..., c][img[..., c] == i] = x_max / s * h
    return np.clip(out, 0, 255).astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256_dark.png')
    #img_orig = cv2.cvtColor(img_orig, cv2.COLOR_BGR2GRAY)
    img_result = hist_equ(img_orig)

    hist_orig = makeHisto(img_orig, 256, 0, 256)
    hist_result = makeHisto(img_result, 256, 0, 256)

    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.imshow('python_origin_hist', hist_orig)
    cv2.imshow('python_result_hist', hist_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.moveWindow('python_origin_hist', img_orig.shape[1]+img_result.shape[1], 0)
    cv2.moveWindow('python_result_hist', img_orig.shape[1]+img_result.shape[1]+hist_orig.shape[1], 0)
    cv2.waitKey(0)
