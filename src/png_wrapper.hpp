#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <png.h>

#include "image.hpp"
#include "wrappers_utils.hpp"

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

    class RowPointers
    {
    public:
        RowPointers(png_structp _png_ptr, png_infop _info_ptr) : row_pointers(nullptr), png_ptr(_png_ptr), info_ptr(_info_ptr) {}
        ~RowPointers();

        RowPointers(const RowPointers &) = delete;
        RowPointers &operator=(const RowPointers &) = delete;
        RowPointers(RowPointers &&) = delete;
        RowPointers &operator=(RowPointers &&) = delete;

        void setup(png_bytep data, size_t buffer_size, unsigned width, unsigned height, unsigned bitdepth, unsigned channels);
        png_bytepp get() const { return row_pointers; }

    private:
        png_bytepp row_pointers = nullptr;
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;
    };

    struct ImageInfo
    {
        png_uint_32 width = 0;
        png_uint_32 height = 0;
        png_uint_32 bitdepth = 0;
        png_uint_32 channels = 0;
        png_int_32 color_type = 0;
    };

public:
    std::expected<AnyImage, Error>
    read_png(const std::filesystem::path &path)
    {
        auto file = std::unique_ptr<FILE, FileCloser>(fopen(path.string().c_str(), "rb"), FileCloser());
        if (!file)
        {
            return std::unexpected(Error::FileNotFound);
        }
        auto read_info = prepare_read(file.get());
        ImageInfo image_info = read_png_info(read_info);
        setup_read_transformations(read_info->png_ptr, image_info.color_type, image_info.bitdepth, image_info.channels);
        if (image_info.color_type == PNG_COLOR_TYPE_RGBA)
        {
            return read_png_impl<RGBA>(read_info, image_info);
        }
        else if (image_info.color_type == PNG_COLOR_TYPE_RGB)
        {
            return read_png_impl<RGB>(read_info, image_info);
        }
        else if (image_info.color_type == PNG_COLOR_TYPE_GRAY)
        {
            return read_png_impl<L>(read_info, image_info);
        }
        else
        {
            return std::unexpected(Error::InvalidFormat);
        }
    }

    template <typename P>
    std::expected<Image<P>, Error> read_png_impl(std::unique_ptr<ReadInfo> &read_info, const ImageInfo &image_info)
    {
        Image<P> image(image_info.width, image_info.height);
        png_bytep data = reinterpret_cast<png_bytep>(image.getDataPointer());
        size_t buffer_size = image_info.width * image_info.height * sizeof(P);

        RowPointers row_pointers(read_info->png_ptr, read_info->info_ptr);
        row_pointers.setup(data, buffer_size, image_info.width, image_info.height, image_info.bitdepth, image_info.channels);
        png_read_image(read_info->png_ptr, row_pointers.get());
        png_read_end(read_info->png_ptr, read_info->end_info);
        return std::expected<Image<P>, Error>(image);
    }

    template <typename P>
    void write_png(const std::filesystem::path &path, Image<P> &image)
    {
        auto fp = std::unique_ptr<FILE, FileCloser>(fopen(path.string().c_str(), "wb"), FileCloser());
        if (!fp)
        {
            return;
        }
        auto write_info = std::make_unique<WriteInfo>();
        png_uint_32 bitdepth = 0;
        png_uint_32 channels = 0;
        png_int_32 color_type = 0;
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
        size_t buffer_size = image.getWidth() * image.getHeight() * sizeof(P);

        RowPointers rowPointers(write_info->png_ptr, write_info->info_ptr);
        rowPointers.setup(data, buffer_size, image.getWidth(), image.getHeight(), bitdepth, channels);
        png_write_png(write_info->png_ptr, write_info->info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    }

    ~PngWrapper()
    {
    }

private:
    std::unique_ptr<ReadInfo> prepare_read(FILE *fp);

    ImageInfo read_png_info(std::unique_ptr<ReadInfo> &read_info);

    void setup_read_transformations(png_structp png_ptr, png_int_32 &color_type, png_uint_32 &bitdepth, png_uint_32 &channels);
};
