#pragma once
#include <variant>
#include "image.hpp"

enum class Error
{
    FileNotFound,
    InvalidFormat,
    ReadError,
    UnsupportedFormat
};

class FileCloser
{
public:
    void operator()(std::FILE *file) const noexcept
    {
        std::fclose(file);
    }
};

using FilePtr = std::unique_ptr<std::FILE, FileCloser>;

using AnyImage = std::variant<
    Image<L>,
    Image<RGB>,
    Image<RGBA>>;
