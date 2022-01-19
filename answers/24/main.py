import numpy as np
import cv2
import matplotlib.pyplot as plt
import sys
from answers.hist import *

def gamma_corr(img, c, g):
    out = (1 / c * (img / 255)) ** (1 / g)
    return np.clip(out * 255, 0, 255).astype(np.uint8)

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256_gamma.png')
    img_result = gamma_corr(img_orig, c=1, g=2.2)

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
