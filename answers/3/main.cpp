#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

void binary(const cv::Mat &src, cv::Mat &dst, int thr) {
    cv::Mat tmp = src.clone();
    if (tmp.channels() == 3) {
        cv::cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
    }
    dst = tmp >= thr;
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    binary(img_orig, img_result, 127);

    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
