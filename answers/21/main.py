import numpy as np
import cv2
import matplotlib.pyplot as plt
import sys
from answers.hist import *

def hist_normalize(img, a, b):
    c, d = img.min(), img.max()
    # if c <= xin < d
    out = (b - a) / (d - c) * (img - c) + a
    # if xin < c
    out[img < c] = a
    # if xin > d
    out[img > d] = b
    return np.clip(out, 0, 255).astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256_dark.png')
    img_result = hist_normalize(img_orig, a=0, b=256)

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
