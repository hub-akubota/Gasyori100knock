#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void filter_LoG(const cv::Mat &src, cv::Mat &dst, cv::Size ksize=cv::Size(3, 3), float sigma=1.2) {
    dst = src.clone();
    dst.convertTo(dst, CV_32F);
    cv::Mat tmp = dst.clone();

    int w = src.size().width;
    int h = src.size().height;

    // prepare kernel
    cv::Mat k = cv::Mat::zeros(ksize, CV_32F);
    float tot_k = 0;
    for (int step_x=0; step_x<ksize.width; step_x++) {
        int x = step_x - (int)(ksize.width / 2);
        for (int step_y=0; step_y<ksize.height; step_y++) {
            int y = step_y - (int)(ksize.height / 2);
            k.at<float>(step_y, step_x) =
                (pow(x, 2) + pow(y, 2) - (2 * pow(sigma, 2)))
                / (2 * M_PI * pow(sigma, 6))
                * exp(-(pow(x, 2) + pow(y, 2))
                       / (2 * pow(sigma, 2)));
            tot_k += k.at<float>(step_y, step_x);
        }
    }
    k /= tot_k;

    dst.forEach<cv::Vec3f>([&](cv::Vec3f &pixel, const int pos[]) -> void {
        int x = pos[1];
        int y = pos[0];
        cv::Vec3f value = cv::Vec3f(0, 0, 0);
        for (int step_x=0; step_x<ksize.width; step_x++) {
            for (int step_y=0; step_y<ksize.height; step_y++) {
                int col = x + step_x - 1;
                int row = y + step_y - 1;
                if (col < 0) col = 0;
                if (col >= w) col = w - 1;
                if (row < 0) row = 0;
                if (row >= h) row = h - 1;
                value += tmp.at<cv::Vec3f>(row, col) * k.at<float>(step_y, step_x);
            }
        }
        pixel = value;
    });

    std::vector<cv::Mat> channels;
    cv::split(dst, channels);
    double mMin = INT_MAX;
    for (int ch=0; ch<3; ch++) {
        double min, max;
        cv::minMaxLoc(channels[ch], &min, &max);
        if (min < mMin) mMin = min;
    }
    double mMax = -INT_MAX;
    for (int ch=0; ch<3; ch++) {
        channels[ch] -= mMin;
        double min, max;
        cv::minMaxLoc(channels[ch], &min, &max);
        if (max > mMax) mMax = max;
    }
    for (int ch=0; ch<3; ch++) {
        channels[ch] /= mMax;
        channels[ch] *= 255;
    }
    cv::merge(channels, dst);
    dst.convertTo(dst, CV_8U);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_noise.png");
    cv::Mat img_result;
    filter_LoG(img_orig, img_result, cv::Size(5, 5), 3);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_origin", 2*img_orig.size().width, 0);
    cv::moveWindow("cpp_result", 3*img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
