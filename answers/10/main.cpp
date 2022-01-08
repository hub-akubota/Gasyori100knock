#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void filter_median(const cv::Mat &src, cv::Mat &dst, cv::Size ksize=cv::Size(3, 3)) {
    dst = src.clone();
    cv::Mat tmp = src.clone();

    int w = src.size().width;
    int h = src.size().height;

    dst.forEach<cv::Vec3b>([&](cv::Vec3b &pixel, const int pos[]) -> void {
        int x = pos[1];
        int y = pos[0];
        std::array<std::vector<unsigned char>, 3> value;
        for (int step_x=0; step_x<ksize.width; step_x++) {
            for (int step_y=0; step_y<ksize.height; step_y++) {
                int col = x + step_x - (int)(ksize.width / 2);
                int row = y + step_y - (int)(ksize.height / 2);
                if (col < 0) col = 0;
                if (col >= w) col = w - 1;
                if (row < 0) row = 0;
                if (row >= h) row = h - 1;
                for (int ch=0; ch<3; ch++) {
                    value[ch].push_back(tmp.at<cv::Vec3b>(row, col)[ch]);
                }
            }
        }
        int median = ksize.width * ksize.height / 2 + 1;
        for (int ch=0; ch<3; ch++) {
            std::sort(value[ch].begin(), value[ch].end());
            pixel[ch] = value[ch][median];
        }
    });
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_noise.png");
    cv::Mat img_result;
    filter_median(img_orig, img_result, cv::Size(3, 3));
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
