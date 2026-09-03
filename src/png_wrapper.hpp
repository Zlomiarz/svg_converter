#pragma once

#include <expected>
#include <filesystem>
#include <memory>
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
        WriteInfo(const WriteInfo &) = delete;
        WriteInfo &operator=(const WriteInfo &) = delete;
        WriteInfo(WriteInfo &&) = delete;
        WriteInfo &operator=(WriteInfo &&) = delete;
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    class ReadInfo
    {
    public:
        ReadInfo();
        ~ReadInfo();
        ReadInfo(const ReadInfo &) = delete;
        ReadInfo &operator=(const ReadInfo &) = delete;
        ReadInfo(ReadInfo &&) = delete;
        ReadInfo &operator=(ReadInfo &&) = delete;
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
        png_infop end_info = nullptr;
    };

    class FileCloser
    {
    public:
        void operator()(std::FILE *file) const noexcept
        {
            std::fclose(file);
        }
    };

    class RowPointers
    {
    public:
        RowPointers(png_structp _png_ptr, png_infop _info_ptr) : row_pointers(nullptr), png_ptr(_png_ptr), info_ptr(_info_ptr) {}
        ~RowPointers()
        {
            if (row_pointers)
            {
                png_free(png_ptr, row_pointers);
                row_pointers = nullptr;
            }
        }
        RowPointers(const RowPointers &) = delete;
        RowPointers &operator=(const RowPointers &) = delete;
        RowPointers(RowPointers &&) = delete;
        RowPointers &operator=(RowPointers &&) = delete;

        void setup(png_bytep data, size_t buffer_size, unsigned width, unsigned height, unsigned bitdepth, unsigned channels)
        {
            row_pointers = (png_bytepp)png_malloc(png_ptr, sizeof(png_bytepp) * height);
            const unsigned int stride = width * bitdepth * channels / 8;
            if (stride * height > buffer_size)
            {
                throw std::runtime_error("stride * height > buffer_size in setup_row_pointers (loadpng)");
            }
            for (unsigned i = 0; i < height; i++)
            {
                row_pointers[i] = data + i * stride;
            }
            png_set_rows(png_ptr, info_ptr, row_pointers);
        }
        png_bytepp get() const { return row_pointers; }
        png_bytepp row_pointers = nullptr;
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    using FilePtr = std::unique_ptr<std::FILE, FileCloser>;

public:
    template <typename P>
    std::expected<Image<P>, Error>
    read_png(const std::filesystem::path &path)
    {
        auto file = open_file(path, "rb");
        auto read_info = prepare_read(file.get());
        read_png_info(read_info);
        Image<P> image(width, height);
        png_bytep data = reinterpret_cast<png_bytep>(image.getDataPointer());
        size_t buffer_size = width * height * sizeof(P);

        RowPointers row_pointers(read_info->png_ptr, read_info->info_ptr);
        row_pointers.setup(data, buffer_size, width, height, bitdepth, channels);
        png_read_image(read_info->png_ptr, row_pointers.get());
        png_read_end(read_info->png_ptr, read_info->end_info);
        return std::expected<Image<P>, Error>(image);
    }

    template <typename P>
    void write_png(const std::filesystem::path &path, Image<P> &image)
    {
        auto fp = open_file(path, "wb");
        auto write_info = std::make_unique<WriteInfo>();

        png_init_io(write_info->png_ptr, fp.get());
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

        RowPointers rowPointers(write_info->png_ptr, write_info->info_ptr);
        rowPointers.setup(data, buffer_size, image.getWidth(), image.getHeight(), bitdepth, channels);
        png_write_png(write_info->png_ptr, write_info->info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    }

    ~PngWrapper()
    {
    }

private:
    FilePtr open_file(const std::filesystem::path &path, const char *mode);

    std::unique_ptr<ReadInfo> prepare_read(FILE *fp);

    void read_png_info(std::unique_ptr<ReadInfo> &read_info);

    png_uint_32 width = 0, height = 0;
    png_uint_32 bitdepth = 0;
    png_int_32 channels = 0;
    png_int_32 color_type = 0;
};
