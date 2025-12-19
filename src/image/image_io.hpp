#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <string>
#include "image_aos.hpp"

namespace image_io {

    // Загрузка изображения из файла в формат ImageAoS (BGR-каналы, как в OpenCV)
    ImageAoS load_image(const std::string& filepath);

    // Сохранение изображения из формата ImageAoS (BGR) в файл
    void save_image(const ImageAoS& image, const std::string& filepath);

} // namespace image_io

#endif // IMAGE_IO_HPP
