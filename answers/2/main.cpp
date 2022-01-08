#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

void bgr2gray(const cv::Mat &src, cv::Mat &dst) {
    std::vector<cv::Mat> planes;
    cv::split(src, planes);
    dst = planes[0] * 0.0722 + planes[1] * 0.7152 + planes[2] * 0.2126;
    cv::threshold(dst, dst, 0, 255, cv::THRESH_TOZERO);
    cv::threshold(dst, dst, 255, 255, cv::THRESH_TRUNC);
    dst.convertTo(dst, CV_8UC1);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    bgr2gray(img_orig, img_result);

    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
