#include "png_wrapper.hpp"

#include <iostream>

PngWrapper::WriteInfo::WriteInfo()
{
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        throw std::runtime_error("png_create_write_struct failed in WriteInfo constructor");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        throw std::runtime_error("png_create_info_struct failed in WriteInfo constructor");
    }
}

PngWrapper::WriteInfo::~WriteInfo()
{
    if (png_ptr)
    {
        png_destroy_write_struct(&png_ptr, info_ptr ? &info_ptr : nullptr);
        png_ptr = nullptr;
        info_ptr = nullptr;
    }
}

PngWrapper::ReadInfo::ReadInfo()
{
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        throw std::runtime_error("png_create_read_struct failed in ReadInfo constructor");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw std::runtime_error("png_create_info_struct failed in ReadInfo constructor");
    }
    end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        throw std::runtime_error("png_create_info_struct failed in ReadInfo constructor");
    }
}

PngWrapper::ReadInfo::~ReadInfo()
{
    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : nullptr, end_info ? &end_info : nullptr);
        png_ptr = nullptr;
        info_ptr = nullptr;
        end_info = nullptr;
    }
}

FILE *PngWrapper::open_file(const std::filesystem::path &path, const char *mode)
{
    fp = fopen(path.string().c_str(), mode);
    if (!fp)
        throw std::runtime_error("file not found in open_file (loadpng)");
    return fp;
}

void PngWrapper::read_png_info()
{
    png_read_info(read_info->png_ptr, read_info->info_ptr);
    width = png_get_image_width(read_info->png_ptr, read_info->info_ptr);
    height = png_get_image_height(read_info->png_ptr, read_info->info_ptr);

    bitdepth = png_get_bit_depth(read_info->png_ptr, read_info->info_ptr);
    channels = png_get_channels(read_info->png_ptr, read_info->info_ptr);
    color_type = png_get_color_type(read_info->png_ptr, read_info->info_ptr);
}

std::expected<png_structp, Error> PngWrapper::prepare_read()
{
    unsigned char header[9];
    int number_to_check = 8;
    fread(header, 1, number_to_check, fp);
    int is_png = !png_sig_cmp(header, 0, number_to_check);
    if (!is_png)
    {
        return std::unexpected(Error::InvalidFormat);
    }

    read_info = std::make_unique<ReadInfo>();
    png_init_io(read_info->png_ptr, fp);
    // Inicidate how many bytes we already read;
    png_set_sig_bytes(read_info->png_ptr, number_to_check);
    return std::expected<png_structp, Error>(read_info->png_ptr);
}

void PngWrapper::setup_row_pointers(png_bytep data, size_t buffer_size)
{
    if (write_info)
    {
        row_pointers = (png_bytepp)png_malloc(write_info->png_ptr, sizeof(png_bytepp) * height);
    }
    else if (read_info)
    {
        row_pointers = (png_bytepp)png_malloc(read_info->png_ptr, sizeof(png_bytepp) * height);
    }
    else
    {
        throw std::runtime_error("both write_info and read_info are null in setup_row_pointers");
    }
    const unsigned int stride = width * bitdepth * channels / 8;
    if (stride * height > buffer_size)
    {
        std::cerr << "stride: " << stride << ", height: " << height << ", buffer_size: " << buffer_size << std::endl;
        throw std::runtime_error("stride * height > buffer_size in setup_row_pointers (loadpng)");
    }
    for (unsigned i = 0; i < height; i++)
    {
        row_pointers[i] = data + i * stride;
    }
    if (write_info)
    {
        png_set_rows(write_info->png_ptr, write_info->info_ptr, row_pointers);
    }
    else if (read_info)
    {
        png_set_rows(read_info->png_ptr, read_info->info_ptr, row_pointers);
    }
    else
    {
        throw std::runtime_error("both write_info and read_info are null in setup_row_pointers");
    }
}

void PngWrapper::cleanup()
{
    if (row_pointers)
    {
        if (write_info)
        {
            png_free(write_info->png_ptr, row_pointers);
        }
        else if (read_info)
        {
            png_free(read_info->png_ptr, row_pointers);
        }
        else
        {
            throw std::runtime_error("row_pointers is not null but both write_info and read_info are null in cleanup()");
        }
        row_pointers = nullptr;
    }
    write_info = nullptr;
    read_info = nullptr;
    if (fp)
    {
        fclose(fp);
        fp = nullptr;
    }
}