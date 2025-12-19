#include "image_io.hpp"

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>

namespace image_io {

ImageAoS load_image(const std::string& filepath) {
    cv::Mat cv_image = cv::imread(filepath, cv::IMREAD_COLOR);

    if (cv_image.empty()) {
        throw std::runtime_error("Failed to load image: " + filepath);
    }

    if (cv_image.channels() != 3) {
        throw std::runtime_error("Image must have 3 color channels (BGR)");
    }

    const int width  = cv_image.cols;
    const int height = cv_image.rows;

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Invalid image dimensions for file: " + filepath);
    }

    ImageAoS result(width, height);

    // копируем пиксели из BGR Mat OpenCV в AoS-формат (также BGR)
    for (int y = 0; y < height; ++y) {
        const cv::Vec3b* row_ptr = cv_image.ptr<cv::Vec3b>(y);
        for (int x = 0; x < width; ++x) {
            const cv::Vec3b& pixel = row_ptr[x];
            result.at(x, y) = Pixel(pixel[0], pixel[1], pixel[2]); // B, G, R
        }
    }

    return result;
}

void save_image(const ImageAoS& image, const std::string& filepath) {
    if (image.empty()) {
        throw std::runtime_error("Cannot save empty image to: " + filepath);
    }

    const int width  = image.width();
    const int height = image.height();

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Invalid image dimensions when saving: " + filepath);
    }

    cv::Mat cv_image(height, width, CV_8UC3);

    // конвертируем из AoS формата (BGR) в Mat OpenCV (BGR)
    for (int y = 0; y < height; ++y) {
        cv::Vec3b* row_ptr = cv_image.ptr<cv::Vec3b>(y);
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = image.at(x, y);
            row_ptr[x] = cv::Vec3b(pixel.b, pixel.g, pixel.r);
        }
    }

    if (!cv::imwrite(filepath, cv_image)) {
        throw std::runtime_error("Failed to save image: " + filepath);
    }
}

} // namespace image_io
