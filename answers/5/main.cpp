#include <vector>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

void bgr2hsv(const cv::Mat &src, cv::Mat &dst) {
    typedef cv::Point3_<float> Pixel;
    dst = src.clone();
    dst.convertTo(dst, CV_32FC3);
    dst.forEach<Pixel>([&](Pixel &pixel, const int pos[]) -> void {
        float b = pixel.x;
        float g = pixel.y;
        float r = pixel.z;
        float v_max = std::max({b, g, r});
        float v_min = std::min({b, g, r});
        float diff = v_max - v_min;
        float h = 0;
        float s = diff;
        float v = v_max;
        if (v_min == v) {
            h = 0;
        } else if (v_min == b) { // B
            h = 60 * (g - r) / diff + 60.;
        } else if (v_min == g) { // G
            h = 60 * (r - b) / diff + 300.;
        } else if (v_min == r) { // R
            h = 60 * (b - g) / diff + 180.;
        }
        pixel.x = h;
        pixel.y = s;
        pixel.z = v;
    });
}

void hsv2bgr(const cv::Mat &src, cv::Mat &dst) {
    typedef cv::Point3_<float> Pixel;
    dst = src.clone();
    dst.forEach<Pixel>([&](Pixel &pixel, const int pos[]) -> void {
        float h = pixel.x;
        float s = pixel.y;
        float v = pixel.z;
        float _h = h / 60.;
        float x = s * (1 - std::abs(std::fmod(_h, 2) - 1));
        float vals[6][3] = {
            {0, x, s},
            {0, s, x},
            {x, s, 0},
            {s, x, 0},
            {s, 0, x},
            {x, 0, s}
        };
        for (int i = 0; i < 6; i++) {
            if (i <= _h && _h < i + 1) {
                pixel.x = v - s + vals[i][0];
                pixel.y = v - s + vals[i][1];
                pixel.z = v - s + vals[i][2];
                break;
            }
        }
    });
    dst.convertTo(dst, CV_8UC3);
}

int main() {
    cv::Mat img_orig = cv::imread("questions/dataset/images/imori_256x256.png");
    cv::Mat img_hsv;
    cv::Mat img_result;
    bgr2hsv(img_orig, img_hsv);
    hsv2bgr(img_hsv, img_result);
    cv::imshow("cpp_origin", img_orig);
    cv::imshow("cpp_result", img_result);
    cv::moveWindow("cpp_result", img_orig.size().width, 0);
    cv::waitKey(3000);
    return 0;
}
