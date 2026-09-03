#include "png_wrapper.hpp"

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

PngWrapper::FilePtr PngWrapper::open_file(const std::filesystem::path &path, const char *mode)
{
    auto fp = std::unique_ptr<FILE, FileCloser>(fopen(path.string().c_str(), mode), FileCloser());
    if (!fp)
        throw std::runtime_error("file not found in open_file (loadpng)");
    return fp;
}

void PngWrapper::read_png_info(std::unique_ptr<ReadInfo> &read_info)
{
    png_read_info(read_info->png_ptr, read_info->info_ptr);
    width = png_get_image_width(read_info->png_ptr, read_info->info_ptr);
    height = png_get_image_height(read_info->png_ptr, read_info->info_ptr);

    bitdepth = png_get_bit_depth(read_info->png_ptr, read_info->info_ptr);
    channels = png_get_channels(read_info->png_ptr, read_info->info_ptr);
    color_type = png_get_color_type(read_info->png_ptr, read_info->info_ptr);
}

std::unique_ptr<PngWrapper::ReadInfo> PngWrapper::prepare_read(FILE *fp)
{
    unsigned char header[9];
    int number_to_check = 8;
    fread(header, 1, number_to_check, fp);
    int is_png = !png_sig_cmp(header, 0, number_to_check);
    if (!is_png)
    {
        throw std::runtime_error("file is not a valid PNG in prepare_read");
    }

    auto read_info = std::make_unique<ReadInfo>();
    png_init_io(read_info->png_ptr, fp);
    png_set_sig_bytes(read_info->png_ptr, number_to_check);
    return read_info;
}
