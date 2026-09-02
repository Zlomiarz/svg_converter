#pragma once

#include <expected>
#include <filesystem>
#include <png.h>

#include "image.hpp"

enum class Error
{
    FileNotFound,
    InvalidFormat,
    ReadError
};

class PngWrapper
{

    class WriteInfo
    {
    public:
        WriteInfo();
        ~WriteInfo();
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    class ReadInfo
    {
    public:
        ReadInfo();
        ~ReadInfo();
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        png_infop end_info = nullptr;
    };

public:
    template <typename P>
    std::expected<Image<P>, Error>
    read_png(const std::filesystem::path &path)
    {
        cleanup();
        fp = open_file(path, "rb");
        auto result = prepare_read();
        if (!result)
            return std::unexpected(result.error());
        read_png_info();
        Image<P> image(width, height);
        png_bytep data = reinterpret_cast<png_bytep>(image.getDataPointer());
        size_t buffer_size = width * height * sizeof(P);

        setup_row_pointers(data, buffer_size);
        png_read_image(read_info->png_ptr, row_pointers);
        png_read_end(read_info->png_ptr, read_info->end_info);
        return std::expected<Image<P>, Error>(image);
    }

    template <typename P>
    void write_png(const std::filesystem::path &path, Image<P> &image)
    {
        cleanup();
        fp = open_file(path, "wb");
        write_info = std::make_unique<WriteInfo>();

        png_init_io(write_info->png_ptr, fp);
        if constexpr (std::is_same_v<P, RGBA>)
        {
            color_type = PNG_COLOR_TYPE_RGBA;
            bitdepth = 8;
            channels = 4;
        }
        else if constexpr (std::is_same_v<P, RGB>)
        {
            color_type = PNG_COLOR_TYPE_RGB;
            bitdepth = 8;
            channels = 3;
        }
        else if constexpr (std::is_same_v<P, L>)
        {
            color_type = PNG_COLOR_TYPE_GRAY;
            bitdepth = 8;
            channels = 1;
        }
        png_set_IHDR(write_info->png_ptr, write_info->info_ptr, image.getWidth(), image.getHeight(), 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_bytep data = reinterpret_cast<png_bytep>(image.getDataPointer());
        size_t buffer_size = width * height * sizeof(P);

        setup_row_pointers(data, buffer_size);
        png_write_png(write_info->png_ptr, write_info->info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
        cleanup();
    }

    ~PngWrapper()
    {
        cleanup();
    }

private:
    FILE *open_file(const std::filesystem::path &path, const char *mode);

    std::expected<png_structp, Error> prepare_read();

    void read_png_info();

    void setup_row_pointers(png_bytep data, size_t buffer_size);

    void cleanup();

    FILE *fp = nullptr;
    std::unique_ptr<WriteInfo> write_info;
    std::unique_ptr<ReadInfo> read_info;
    png_bytepp row_pointers = nullptr;
    png_uint_32 width = 0, height = 0;
    png_uint_32 bitdepth = 0;
    png_int_32 channels = 0;
    png_int_32 color_type = 0;
};
