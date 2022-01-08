#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

// チャンネルごとに最大値を計算
void pool_max(const cv::Mat &src, cv::Mat &dst, int ksize_h = 8, int ksize_w = 8) {
    dst = src.clone();

    int h = src.size().height;
    int w = src.size().width;

    int remainder_h = ksize_h - h % ksize_h;
    int line_top = remainder_h / 2;
    int remainder_w = ksize_w - w % ksize_w;
    int line_left = remainder_w / 2;
    int blocks_w = (w + remainder_w) / ksize_w;
    int blocks_h = (h + remainder_h) / ksize_h;

    for (int x=0; x<blocks_w; x++) {
        for (int y=0; y<blocks_h; y++) {
            cv::Vec3b value_max = cv::Vec3b(0, 0, 0);
            for (int step_w=0; step_w<ksize_w; step_w++) {
                for (int step_h=0; step_h<ksize_h; step_h++) {
                    int col = ksize_w * x - line_left + step_w;
                    int row = ksize_h * y - line_top + step_h;
                    if (col < 0) continue;
                    if (col >= w) continue;
                    if (row < 0) continue;
                    if (row >= h) continue;
                    for (int ch=0; ch<3; ch++) {
                        cv::Vec3b value = dst.at<cv::Vec3b>(row, col);
                        if (value[ch] > value_max[ch]) {
                            value_max[ch] = value[ch];
                        }
                    }
                }
            }
            for (int step_w=0; step_w<ksize_w; step_w++) {
                for (int step_h=0; step_h<ksize_h; step_h++) {
                    int col = ksize_w * x - line_left + step_w;
                    int row = ksize_h * y - line_top + step_h;
                    if (col < 0) continue;
                    if (col >= w) continue;
                    if (row < 0) continue;
                    if (row >= h) continue;
                    dst.at<cv::Vec3b>(row, col) = value_max;
                }
            }
        }
    }
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    pool_max(img_orig, img_result);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
