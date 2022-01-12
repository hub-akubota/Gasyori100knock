#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>
#include "../hist.cpp"

void hist_scaleshift(const cv::Mat &src, cv::Mat &dst, float m, float s) {
    // チャンネルをひとまとめにする
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    cv::Mat tmp;
    for (auto &channel : channels) {
        channel.convertTo(channel, CV_32F);
        tmp.push_back(channel);
    }

    // 平均値と標準偏差を求める
    cv::Scalar mean, std;
    cv::meanStdDev(tmp, mean, std);
    float m0 = mean[0];
    float s0 = std[0];

    // スケール & シフト
    for (auto &channel : channels) {
        channel -= cv::Scalar::all(m0);
        channel *= (s / s0);
        channel += cv::Scalar::all(m);
        channel.convertTo(channel, CV_8U);
    }

    cv::merge(channels, dst);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_dark.png");
    cv::Mat img_result;
    hist_scaleshift(img_orig, img_result, 128, 50);

    cv::Mat hist_orig, hist_result;
    makeHisto(img_orig, hist_orig, 256, 0, 256);
    makeHisto(img_result, hist_result, 256, 0, 256);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::imshow("cpp_origin_hist", hist_orig);
    cv::imshow("cpp_result_hist", hist_result);
    cv::moveWindow("cpp_origin", 0, -img_orig.size().height);
    cv::moveWindow("cpp_result", img_orig.size().width, -img_orig.size().height);
    cv::moveWindow("cpp_origin_hist", img_orig.size().width + img_result.size().width, -img_orig.size().height);
    cv::moveWindow("cpp_result_hist", img_orig.size().width + img_result.size().width + hist_orig.size().width, -img_orig.size().height);
    cv::waitKey(0);
    return 0;
}
