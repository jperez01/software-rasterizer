add_subdirectory(glm)

add_library(stb_image STATIC)
target_sources(stb_image PRIVATE
    stb_image/image.cpp)
target_include_directories(stb_image INTERFACE stb_image)