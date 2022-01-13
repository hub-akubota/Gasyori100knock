#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>
#include "../hist.cpp"

void hist_equ(const cv::Mat &src, cv::Mat &dst) {
    // チャンネルをひとまとめにする
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    cv::Mat tmp;
    for (auto &channel : channels) {
        tmp.push_back(channel);
    }

    // cv::Matをstd::vectorに変換
    std::vector<uchar> array;
    array.assign(tmp.begin<uchar>(), tmp.end<uchar>());
    float s = array.size();

    // 合計値を計算
    float x_max = 255;
    std::vector<int> pixels;
    pixels.resize(256, 0);
    for (int i=0; i<256; i++) {
        if (i != 0) {
            pixels[i] = pixels[i-1];
        }
        pixels[i] += std::count(array.begin(), array.end(), i);
    }

    // 平坦化
    dst = src.clone();
    dst.convertTo(dst, CV_32F);
    if (dst.channels() == 3) {
        dst.forEach<cv::Vec3f>([&](cv::Vec3f &pixel, const int pos[]) -> void {
            pixel[0] = x_max / s * pixels[(int)pixel[0]];
            pixel[1] = x_max / s * pixels[(int)pixel[1]];
            pixel[2] = x_max / s * pixels[(int)pixel[2]];
        });
    } else {
        dst.forEach<float>([&](float &pixel, const int pos[]) -> void {
            pixel = x_max / s * pixels[(int)pixel];
        });
    }
    dst.convertTo(dst, CV_8U);
}

void hist_equ_rgb(const cv::Mat &src, cv::Mat &dst) {
    // チャンネルごとに計算する
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    for (auto &channel : channels) {
        // cv::Matをstd::vectorに変換
        std::vector<uchar> array;
        array.assign(channel.begin<uchar>(), channel.end<uchar>());
        float s = array.size();

        // 合計値を計算
        float x_max = 255;
        std::vector<int> pixels;
        pixels.resize(256, 0);
        for (int i=0; i<256; i++) {
            if (i != 0) {
                pixels[i] = pixels[i-1];
            }
            pixels[i] += std::count(array.begin(), array.end(), i);
        }

        // 平坦化
        channel.convertTo(channel, CV_32F);
        channel.forEach<float>([&](float &pixel, const int pos[]) -> void {
            pixel = x_max / s * pixels[(int)pixel];
        });
        channel.convertTo(channel, CV_8U);
    }
    // マージ
    cv::merge(channels, dst);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256_dark.png");
    cv::Mat img_result;
    hist_equ_rgb(img_orig, img_result);

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
