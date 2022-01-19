#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>
#include "../hist.cpp"

void bilinear_inter(const cv::Mat &src, cv::Mat &dst, float a, float b) {
    // 出力画像
    int dst_h = src.size().height * a;
    int dst_w = src.size().width * b;
    dst = cv::Mat::zeros(cv::Size(dst_h, dst_w), CV_8UC3);

    dst.forEach<cv::Vec3b>([&](cv::Vec3b &pixel, const int pos[]) -> void {
        int _x = pos[1];
        int _y = pos[0];

        int x = floor(_x / b);
        int y = floor(_y / a);

        float dx = _x / b - x;
        float dy = _y / a - y;

        pixel = (1 - dx) * (1 - dy) * src.at<cv::Vec3b>(y, x) +
                dx * (1 - dy) * src.at<cv::Vec3b>(y, x + 1) +
                (1 - dx) * dy * src.at<cv::Vec3b>(y + 1, x) +
                dx * dy * src.at<cv::Vec3b>(y + 1, x + 1);
    });
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    bilinear_inter(img_orig, img_result, 1.5, 1.5);

    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_origin", 0, -img_orig.size().height);
    cv::moveWindow("cpp_result", img_orig.size().width, -img_orig.size().height);
    cv::waitKey(0);
    return 0;
}
