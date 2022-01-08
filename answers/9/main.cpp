#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

// チャンネルごとに最大値を計算
void filter_gaussian(const cv::Mat &src, cv::Mat &dst, cv::Size ksize=cv::Size(3, 3), float sigma=1.3) {
    dst = src.clone();
    cv::Mat tmp = src.clone();
    tmp.convertTo(tmp, CV_32FC3);

    int w = src.size().width;
    int h = src.size().height;

    // prepare kernel
    cv::Mat k = cv::Mat::zeros(ksize, CV_32F);
    float tot_k = 0;
    for (int x=0; x<ksize.width; x++) {
        for (int y=0; y<ksize.height; y++) {
            k.at<float>(y, x) =
                1. / (sqrt(2. * M_PI) * pow(sigma, 2))
                * exp(
                    -(pow((x - (int)(ksize.width / 2)), 2) +
                      pow((y - (int)(ksize.height / 2)), 2))
                    / (2 * pow(sigma, 2)));
            tot_k += k.at<float>(y, x);
        }
    }
    k /= tot_k;

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
                for (int ch=0; ch<3; ch++) {
                    value[ch] += tmp.at<cv::Vec3f>(row, col)[ch] * k.at<float>(step_y, step_x);
                }
            }
        }
        pixel = value;
    });
    dst.convertTo(dst, CV_8UC3);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_noise.png");
    cv::Mat img_result;
    filter_gaussian(img_orig, img_result, cv::Size(5, 5), 10.3);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
