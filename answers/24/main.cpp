#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>
#include "../hist.cpp"

void gamma_corr(const cv::Mat &src, cv::Mat &dst, float c, float g) {
    dst = src.clone();
    dst.convertTo(dst, CV_32F);

    dst.forEach<cv::Vec3f>([&](cv::Vec3f &pixel, const int pos[]) -> void {
        for (int i=0; i<3; i++) {
            pixel[i] = pow(((1 / c) * (pixel[i] / 255.)), (1 / g)) * 255;
        }
    });
    dst.convertTo(dst, CV_8U);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_gamma.png");
    cv::Mat img_result;
    gamma_corr(img_orig, img_result, 1, 2.2);

    cv::Mat hist_orig, hist_result;
    makeHisto(img_orig, hist_orig, 256, 0, 256);
    makeHisto(img_result, hist_result, 256, 0, 256);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::imshow("cpp_origin_hist", hist_orig);
    cv::imshow("cpp_result_hist", hist_result);
    cv::moveWindow("cpp_origin", 0, img_orig.size().height);
    cv::moveWindow("cpp_result", img_orig.size().width, img_orig.size().height);
    cv::moveWindow("cpp_origin_hist", img_orig.size().width + img_result.size().width, img_orig.size().height);
    cv::moveWindow("cpp_result_hist", img_orig.size().width + img_result.size().width + hist_orig.size().width, img_orig.size().height);
    cv::waitKey(0);
    return 0;
}
