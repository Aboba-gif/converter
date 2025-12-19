#include "converter_app.hpp"

#include "../image/image_io.hpp"
#include "../image/gaussian_blur.hpp"

#include <iostream>
#include <chrono>
#include <stdexcept>

namespace converter_app {

int run(const ConverterConfig& config) {
    try {
        std::cout << "Starting Gaussian blur processing...\n";

        const auto start_time = std::chrono::high_resolution_clock::now();

        // загрузка изображения
        std::cout << "Loading image from: " << config.input_path << '\n';
        ImageAoS image = image_io::load_image(config.input_path);
        std::cout << "Image loaded: " << image.width() << " x "
                  << image.height() << " pixels\n";

        // применение фильтра Гаусса
        std::cout << "Applying Gaussian blur with radius "
                  << config.radius << "...\n";
        ImageAoS blurred_image =
            gaussian_blur::apply_gaussian_blur(image, config.radius);

        // сохранение результата
        std::cout << "Saving result to: " << config.output_path << '\n';
        image_io::save_image(blurred_image, config.output_path);

        const auto end_time = std::chrono::high_resolution_clock::now();
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time);

        std::cout << "\nProcessing completed successfully.\n";
        std::cout << "Time elapsed: " << duration.count() << " ms\n";
        std::cout << "Output saved to: " << config.output_path << '\n';

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << "\n";
        return 1;
    }
}

} // namespace converter_app
