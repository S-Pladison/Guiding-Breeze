# Устанавливает минимальную версию CMake, необходимую для сборки проекта.
cmake_minimum_required(VERSION 3.18.0)

# Определяет имя проекта.
project(guiding_breeze)

# Настраиваем C++.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем исполняемые файлы проекта.
add_executable(${PROJECT_NAME})

# Добавляет поддиректории с другими CMakeLists.txt файлами.
add_subdirectory(lib)
add_subdirectory(src)