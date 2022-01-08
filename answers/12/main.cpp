#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void filter_motion(const cv::Mat &src, cv::Mat &dst, cv::Size ksize=cv::Size(3, 3)) {
    dst = src.clone();
    cv::Mat tmp = src.clone();
    tmp.convertTo(tmp, CV_32FC3);

    int w = src.size().width;
    int h = src.size().height;

    // prepare kernel
    cv::Mat k = cv::Mat::zeros(ksize, CV_32F);
    for (int x=0; x<ksize.width; x++) {
        for (int y=0; y<ksize.height; y++) {
            if (x != y) continue;
            k.at<float>(y, x) = 1 / (float)ksize.width;
        }
    }

    dst.forEach<cv::Vec3b>([&](cv::Vec3b &pixel, const int pos[]) -> void {
        int x = pos[1];
        int y = pos[0];
        cv::Vec3f value = cv::Vec3f(0, 0, 0);
        for (int step_x=0; step_x<ksize.width; step_x++) {
            for (int step_y=0; step_y<ksize.height; step_y++) {
                int col = x + step_x - (int)(ksize.width / 2);
                int row = y + step_y - (int)(ksize.height / 2);
                if (col < 0) col = 0;
                if (col >= w) col = w - 1;
                if (row < 0) row = 0;
                if (row >= h) row = h - 1;
                value += tmp.at<cv::Vec3f>(row, col) * k.at<float>(step_y, step_x);
            }
        }
        pixel = value;
    });
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    filter_motion(img_orig, img_result, cv::Size(5, 5));
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
