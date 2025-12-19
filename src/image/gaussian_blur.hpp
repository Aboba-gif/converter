#ifndef GAUSSIAN_BLUR_HPP
#define GAUSSIAN_BLUR_HPP

#include "image_aos.hpp"
#include <vector>

namespace gaussian_blur {

    // пычисление ядра Гаусса для заданного радиуса
    std::vector<std::vector<double>> compute_gaussian_kernel(int radius);

    // применение фильтра Гаусса к изображению
    ImageAoS apply_gaussian_blur(const ImageAoS& input_image, int radius);

    // применение фильтра Гаусса к одному каналу
    double apply_kernel_to_pixel(const ImageAoS& image,
                                 int x, int y,
                                 int channel_offset,
                                 const std::vector<std::vector<double>>& kernel,
                                 int radius);

} // namespace gaussian_blur

#endif
