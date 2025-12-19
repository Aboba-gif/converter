#ifndef CONVERTER_APP_HPP
#define CONVERTER_APP_HPP

#include <string>

struct ConverterConfig {
    std::string input_path;
    std::string output_path;
    int radius{3};
};

namespace converter_app {

    // основной сценарий приложения: загрузка, фильтрация, сохранение
    int run(const ConverterConfig& config);

} // namespace converter_app

#endif
