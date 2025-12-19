#include <iostream>
#include <string>
#include <stdexcept>
#include "cxxopts.hpp"
#include "app/converter_app.hpp"

void print_help(const cxxopts::Options& options) {
    std::cout << "converter - Gaussian blur image utility\n\n"
              << "Usage:\n"
              << "  converter -i <input> [-o <output>] [-r <radius>]\n\n"
              << options.help() << '\n';
}

int main(int argc, char** argv) {
    try {
        cxxopts::Options options("converter", "Gaussian blur image converter");

        options.add_options()
            ("h,help", "Show help")
            ("i,input", "Path to input RGB image (required)", cxxopts::value<std::string>())
            ("o,output", "Path to output image (optional, default: <input>_blurred.png)", cxxopts::value<std::string>()->default_value(""))
            ("r,radius", "Gaussian blur radius > 0", cxxopts::value<int>()->default_value("3"))
        ;

        cxxopts::ParseResult result;
        try {
            result = options.parse(argc, argv);
        } catch (const cxxopts::exceptions::exception& e) {
            std::cerr << "Error parsing options: " << e.what() << "\n\n";
            print_help(options);
            return 1;
        }

        if (result.count("help")) {
            print_help(options);
            return 0;
        }

        if (!result.count("input")) {
            std::cerr << "Error: Input image path is required\n\n";
            print_help(options);
            return 1;
        }

        ConverterConfig config;
        config.input_path  = result["input"].as<std::string>();
        config.output_path = result["output"].as<std::string>();
        config.radius      = result["radius"].as<int>();

        if (config.radius <= 0) {
            std::cerr << "Error: Radius must be greater than 0\n\n";
            print_help(options);
            return 1;
        }

        if (config.output_path.empty()) {
            const std::size_t dot_pos = config.input_path.find_last_of('.');
            if (dot_pos != std::string::npos) {
                config.output_path = config.input_path.substr(0, dot_pos) +
                                     "_blurred" +
                                     config.input_path.substr(dot_pos);
            } else {
                config.output_path = config.input_path + "_blurred.png";
            }
        }

        std::cout << "Configuration:\n";
        std::cout << "  Input : " << config.input_path  << "\n";
        std::cout << "  Output: " << config.output_path << "\n";
        std::cout << "  Radius: " << config.radius      << "\n\n";

        return converter_app::run(config);

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
}
