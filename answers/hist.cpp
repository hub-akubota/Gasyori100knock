#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cmath>

void makeHisto(const cv::Mat &src, cv::Mat &dst, int hist_size, float hist_min, float hist_max) {
    int h = src.size().height;

    // チャンネルをひとまとめにする
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    cv::Mat tmp;
    for (auto channel : channels) {
        tmp.push_back(channel);
    }

    // 画素数を数える
    cv::Mat hist;
    float range[] = {hist_min, hist_max};
    const float *hist_range = range;
    cv::calcHist(&tmp, 1, 0, cv::Mat(), hist, 1, &hist_size, &hist_range);

    // 正規化
    cv::normalize(hist, hist, 0.0, 1.0, cv::NORM_MINMAX, -1, cv::Mat());

    // 出力画像の生成
    dst = cv::Mat(cv::Size(hist_size+20, h), CV_8UC3, cv::Scalar::all(255));

    // 背景をグレーに描画
    cv::rectangle(dst, cv::Point(10, 10), cv::Point(hist_size+10, h-10), cv::Scalar(240, 240, 240), -1);

    // ヒストグラムを描画
    for (int i=0; i<hist_size; i++) {
        // Fill
        cv::line(
            dst,
            cv::Point(10+i, h-10),
            cv::Point(10+i, h-10-(int)(hist.at<float>(i)*(h-20))),
            cv::Scalar(0, 0, 0),
            1,
            8,
            0
        );

        // 横軸10ずつラインを引く
        if (i%10 == 0) {
            cv::line(dst, cv::Point(10+i, h-10), cv::Point(10+i, 10), cv::Scalar(170, 170, 170), 1, 8, 0);
        }

        // 横軸50ずつ濃いラインを引く
        if (i%50 == 0) {
            cv::line(dst, cv::Point(10+i, h-10), cv::Point(10+i, 10), cv::Scalar(100, 100, 100), 1, 8, 0);
        }
    }
}
