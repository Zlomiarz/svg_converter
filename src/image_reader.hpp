#pragma once

#include <expected>

#include "jpeg_wrapper.hpp"
#include "png_wrapper.hpp"
#include "wrappers_utils.hpp"

class ImageReader
{
public:
    std::expected<AnyImage, Error>
    read(const std::filesystem::path &path)
    {
        const auto extension = path.extension();

        if (extension == ".png" || extension == ".PNG")
        {
            PngWrapper png_;
            return png_.read_png(path);
        }

        if (extension == ".jpg" || extension == ".JPG" ||
            extension == ".jpeg" || extension == ".JPEG")
        {
            JpegWrapper jpeg_;
            auto result = jpeg_.read_jpeg(path);

            if (!result)
                return std::unexpected(result.error());

            return AnyImage{std::move(*result)};
        }

        return std::unexpected(Error::UnsupportedFormat);
    }
};