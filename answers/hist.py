import numpy as np
import cv2
import matplotlib.pyplot as plt

def makeHisto(img, hist_size, hist_min, hist_max):
    h = img.shape[0]

    # チャンネルを一つにまとめる
    img = img.ravel()

    # 画素数を数える
    hist = cv2.calcHist([img], [0], None, [hist_size], [hist_min, hist_max])

    # 正規化
    hist = cv2.normalize(hist, None, 0.0, 1.0, cv2.NORM_MINMAX)

    # 出力画像の生成
    out = np.zeros((h, hist_size+20, 3), np.uint8)
    out += 255

    # 背景をグレーに描画
    out = cv2.rectangle(out, (10, 10), (hist_size+10, h-10), (240, 240, 240), -1)

    # ヒストグラムを描画
    for i in range(hist_size):
        # Fill
        out = cv2.line(out, (10+i, h-10), (10+i, int(h-10-hist[i]*(h-20))), (0, 0, 0), 1, 8, 0)
        # 10ごとにラインを引く
        if i%10 == 0:
            out = cv2.line(out, (10+i, h-10), (10+i, 10), (170, 170, 170), 1, 8, 0)
        # 50ごとにラインを引く
        if i%50 == 0:
            out = cv2.line(out, (10+i, h-10), (10+i, 10), (100, 100, 100), 1, 8, 0)
    return out

if __name__ == '__main__':
    img_orig = cv2.imread('questions/dataset/images/imori_256x256_dark.png')
    img_orig = cv2.cvtColor(img_orig, cv2.COLOR_BGR2GRAY)
    img_result = makeHisto(img_orig, 256, 0, 256)
    cv2.imshow('python_origin', img_orig)
    cv2.imshow('python_result', img_result)
    cv2.moveWindow('python_result', img_orig.shape[1], 0)
    cv2.waitKey(0)
