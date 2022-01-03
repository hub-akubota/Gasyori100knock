#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

void bgr2rgb(const cv::Mat &src, cv::Mat &dst) {
    std::vector<cv::Mat> planes;
	cv::split(src, planes);
    std::vector<cv::Mat> merges;
    merges.push_back(planes[2]);
    merges.push_back(planes[1]);
    merges.push_back(planes[0]);
	cv::merge(merges, dst);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    bgr2rgb(img_orig, img_result);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
