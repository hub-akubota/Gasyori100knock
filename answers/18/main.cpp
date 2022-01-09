#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void filter_emboss(const cv::Mat &src, cv::Mat &dst) {
    dst = src.clone();
    dst.convertTo(dst, CV_32F);
    cv::Mat tmp = dst.clone();

    int w = src.size().width;
    int h = src.size().height;

    // prepare kernel
    cv::Mat k = (cv::Mat_<float>(3, 3) << -2, -1, 0, -1, 1, 1, 0, 1, 2);

    dst.forEach<float>([&](float &pixel, const int pos[]) -> void {
        int x = pos[1];
        int y = pos[0];
        float value = 0;
        for (int step_x=0; step_x<3; step_x++) {
            for (int step_y=0; step_y<3; step_y++) {
                int col = x + step_x - 1;
                int row = y + step_y - 1;
                if (col < 0) col = 0;
                if (col >= w) col = w - 1;
                if (row < 0) row = 0;
                if (row >= h) row = h - 1;
                value += tmp.at<float>(row, col) * k.at<float>(step_y, step_x);
            }
        }
        pixel = value;
    });
    double mMin, mMax;
    cv::minMaxLoc(dst, &mMin, &mMax);
    dst -= mMin;
    cv::minMaxLoc(dst, &mMin, &mMax);
    dst /= mMax;
    dst *= 255;
    dst.convertTo(dst, CV_8U);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    if (img_orig.channels() == 3) {
        cv::cvtColor(img_orig, img_orig, cv::COLOR_BGR2GRAY);
    }
    filter_emboss(img_orig, img_result);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
