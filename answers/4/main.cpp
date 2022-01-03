#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

int otsu_threshold(const cv::Mat &src, cv::Mat &dst) {
    cv::Mat tmp = src.clone();
    if (tmp.channels() == 3) {
        cv::cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
    }

    float max_var = -1;
    float max_thr = 0;
    for (int thr = 0; thr < 255; thr++) {
        cv::Mat M0, M1;
        cv::threshold(tmp, M0, thr, 255, cv::THRESH_TOZERO_INV);
        cv::threshold(tmp, M1, thr, 255, cv::THRESH_TOZERO);
        float w0 = cv::sum(cv::countNonZero(M0))[0]; // # of pixels in class 0
        float w1 = cv::sum(cv::countNonZero(M1))[0]; // # of pixels in class 0
        float m0 = cv::sum(cv::sum(M0))[0] / w0; // mean of class 0
        float m1 = cv::sum(cv::sum(M1))[0] / w1; // mean of class 1
        float var = w0 * w1 / pow((w0 + w1), 2) * pow((m0 - m1), 2); // inter class variance
        if (var > max_var) {
            max_thr = thr;
            max_var = var;
        }
    }

    cv::threshold(tmp, dst, max_thr, 255, cv::THRESH_BINARY);
    return max_thr;
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    int thr = otsu_threshold(img_orig, img_result);
    std::cout << "C++ Threshold: " << thr << std::endl;

    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
