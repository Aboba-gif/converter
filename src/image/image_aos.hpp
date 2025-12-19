#ifndef IMAGE_AOS_HPP
#define IMAGE_AOS_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>

// AoS (BGR)
struct Pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;

    Pixel() : b(0), g(0), r(0) {}
    Pixel(uint8_t blue, uint8_t green, uint8_t red)
        : b(blue), g(green), r(red) {}
};

// класс для хранения изображения в формате AoS
class ImageAoS {
public:
    ImageAoS() : width_(0), height_(0) {}

    ImageAoS(int width, int height)
        : data_(static_cast<std::size_t>(width) * static_cast<std::size_t>(height)),
          width_(width),
          height_(height) 
    {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Image dimensions must be positive");
        }
    }

    // Доступ к пикселям
    Pixel& at(int x, int y) {
        check_bounds(x, y);
        return data_[static_cast<std::size_t>(y) * width_ + x];
    }

    const Pixel& at(int x, int y) const {
        check_bounds(x, y);
        return data_[static_cast<std::size_t>(y) * width_ + x];
    }

    // Геттеры
    int width() const noexcept  { return width_;  }
    int height() const noexcept { return height_; }
    std::size_t size() const noexcept { return data_.size(); }

    bool empty() const noexcept { return data_.empty(); }

private:
    std::vector<Pixel> data_;
    int width_;
    int height_;

    void check_bounds(int x, int y) const {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) {
            throw std::out_of_range("Pixel coordinates out of range");
        }
    }
};

#endif
