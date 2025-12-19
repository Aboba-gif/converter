from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout

class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("cxxopts/3.3.1")
        self.requires("opencv/4.12.0")
    
    def configure(self):
        opencv = self.options["opencv"]

        opencv.contrib = False
        opencv.with_qt = False
        opencv.with_gtk = False
        opencv.with_tbb = False
        opencv.with_openmp = False

        opencv.with_ffmpeg = False
        opencv.with_gstreamer = False
        opencv.with_msmf = False
        opencv.with_v4l = False
        opencv.with_gtk = False
        opencv.with_opengl = False

        if "with_ipp" in opencv:
            opencv.with_ipp = False
        if "with_openexr" in opencv:
            opencv.with_openexr = False

    def layout(self):
        cmake_layout(self)

    def generate(self):
        dp = CMakeDeps(self)
        dp.generate()
        tc = CMakeToolchain(self)
        tc.generate()
