#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

void color_subtraction(const cv::Mat &src, cv::Mat &dst, int div=4) {
    int step = 256 / div;
    typedef cv::Point3_<uint8_t> Pixel;
    dst = src.clone();
    dst.forEach<Pixel>([&](Pixel &pixel, const int pos[]) -> void {
        pixel.x = pixel.x / step * step + step / 2.;
        pixel.y = pixel.y / step * step + step / 2.;
        pixel.z = pixel.z / step * step + step / 2.;
    });
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_result;
    color_subtraction(img_orig, img_result);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
