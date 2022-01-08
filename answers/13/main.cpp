#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void filter_max_min(const cv::Mat &src, cv::Mat &dst, cv::Size ksize=cv::Size(3, 3)) {
    dst = src.clone();
    if (dst.channels() == 3) {
        cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
    }
    cv::Mat tmp = dst.clone();

    int w = src.size().width;
    int h = src.size().height;

    dst.forEach<unsigned char>([&](unsigned char &pixel, const int pos[]) -> void {
        int x = pos[1];
        int y = pos[0];
        unsigned char value_max = 0;
        unsigned char value_min = 255;
        for (int step_x=0; step_x<ksize.width; step_x++) {
            for (int step_y=0; step_y<ksize.height; step_y++) {
                int col = x + step_x - (int)(ksize.width / 2);
                int row = y + step_y - (int)(ksize.height / 2);
                if (col < 0) col = 0;
                if (col >= w) col = w - 1;
                if (row < 0) row = 0;
                if (row >= h) row = h - 1;
                unsigned char value = tmp.at<unsigned char>(row, col);
                if (value > value_max) value_max = value;
                if (value < value_min) value_min = value;
            }
        }
        pixel = value_max - value_min;
    });
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    filter_max_min(img_orig, img_result, cv::Size(5, 5));
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
