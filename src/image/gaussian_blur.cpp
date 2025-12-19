#include "gaussian_blur.hpp"

#include <cmath>
#include <stdexcept>
#include <iostream>

namespace gaussian_blur {

std::vector<std::vector<double>> compute_gaussian_kernel(int radius) {
    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }

    const int kernel_size = 2 * radius + 1;
    std::vector<std::vector<double>> kernel(
        static_cast<std::size_t>(kernel_size),
        std::vector<double>(static_cast<std::size_t>(kernel_size), 0.0)
    );

    const double sigma = static_cast<double>(radius);
    double sum = 0.0;

    // вычисляем значения ядра по дискретной версии Гаусса
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            const double exponent = -(static_cast<double>(i * i + j * j)) /
                                    (2.0 * sigma * sigma);
            const double value = std::exp(exponent);
            kernel[static_cast<std::size_t>(i + radius)]
                  [static_cast<std::size_t>(j + radius)] = value;
            sum += value;
        }
    }

    // нормализуем ядро так, чтобы сумма коэффициентов была равна 1
    if (sum > 0.0) {
        for (int i = 0; i < kernel_size; ++i) {
            for (int j = 0; j < kernel_size; ++j) {
                kernel[static_cast<std::size_t>(i)]
                      [static_cast<std::size_t>(j)] /= sum;
            }
        }
    }

    return kernel;
}

double apply_kernel_to_pixel(const ImageAoS& image,
                             int x, int y,
                             int channel_offset,
                             const std::vector<std::vector<double>>& kernel,
                             int radius)
{
    const int width  = image.width();
    const int height = image.height();

    double result = 0.0;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int nx = x + dx;
            int ny = y + dy;

            // Обработка граничных случаев (clamp к краю изображения)
            if (nx < 0)        nx = 0;
            if (nx >= width)   nx = width - 1;
            if (ny < 0)        ny = 0;
            if (ny >= height)  ny = height - 1;

            const Pixel& pixel = image.at(nx, ny);
            const double kernel_value =
                kernel[static_cast<std::size_t>(dy + radius)]
                      [static_cast<std::size_t>(dx + radius)];

            double channel_value = 0.0;
            switch (channel_offset) {
                case 0: channel_value = pixel.b; break;
                case 1: channel_value = pixel.g; break;
                case 2: channel_value = pixel.r; break;
                default:
                    // защитный вариант: если передали некорректный канал — просто 0
                    channel_value = 0.0;
                    break;
            }

            result += channel_value * kernel_value;
        }
    }

    if (result < 0.0)   result = 0.0;
    if (result > 255.0) result = 255.0;

    return result;
}

ImageAoS apply_gaussian_blur(const ImageAoS& input_image, int radius) {
    if (input_image.empty()) {
        return ImageAoS();
    }

    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }

    const int width  = input_image.width();
    const int height = input_image.height();

    ImageAoS output_image(width, height);

    const auto kernel = compute_gaussian_kernel(radius);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& out_pixel = output_image.at(x, y);

            out_pixel.b = static_cast<std::uint8_t>(
                apply_kernel_to_pixel(input_image, x, y, 0, kernel, radius)
            );
            out_pixel.g = static_cast<std::uint8_t>(
                apply_kernel_to_pixel(input_image, x, y, 1, kernel, radius)
            );
            out_pixel.r = static_cast<std::uint8_t>(
                apply_kernel_to_pixel(input_image, x, y, 2, kernel, radius)
            );
        }
    }

    return output_image;
}

} // namespace gaussian_blur
