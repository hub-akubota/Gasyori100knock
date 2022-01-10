#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>
#include "../hist.cpp"

void hist_normalize(const cv::Mat &src, cv::Mat &dst, float a, float b) {
    dst = src.clone();
    dst.convertTo(dst, CV_32F);

    double c, d;
    cv::minMaxLoc(src, &c, &d);
    if (c == d) {
        return;
    }

    // if c <= xin < d
    dst -= cv::Scalar::all(c);
    dst *= 255 / (d - c);
    dst.convertTo(dst, CV_8U);

    // if xin < c
    dst *= (b - a) / 255;
    dst += cv::Scalar::all(a);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_dark.png");
    cv::Mat img_result;
    hist_normalize(img_orig, img_result, 0, 256);

    cv::Mat hist_orig, hist_result;
    makeHisto(img_orig, hist_orig, 256, 0, 256);
    makeHisto(img_result, hist_result, 256, 0, 256);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::imshow("cpp_origin_hist", hist_orig);
    cv::imshow("cpp_result_hist", hist_result);
    cv::moveWindow("cpp_origin", 0, -img_orig.size().height);
    cv::moveWindow("cpp_result", img_orig.size().width, -img_orig.size().height);
    cv::moveWindow("cpp_origin_hist", img_orig.size().width + img_result.size().width, -img_orig.size().height);
    cv::moveWindow("cpp_result_hist", img_orig.size().width + img_result.size().width + hist_orig.size().width, -img_orig.size().height);
    cv::waitKey(0);
    return 0;
}
