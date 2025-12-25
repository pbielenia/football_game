from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout


class ConanPackage(ConanFile):
    name = "football_game"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "raylib/5.5",
    )
    tool_requires = "cmake/4.1.2"
    default_options = {"boost/*:shared": False}

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        toolchain = CMakeToolchain(self)
        if self.settings.build_type == "Debug":
            toolchain.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        toolchain.generate()

    def layout(self):
        cmake_layout(self)
