#pragma once

#include <expected>
#include <filesystem>
#include <memory>

#include <jpeglib.h>
#include <jerror.h>

#include "wrappers_utils.hpp"
#include "image.hpp"

class JpegWrapper
{
public:
    std::expected<Image<RGB>, Error>
    read_jpeg(const std::filesystem::path &path)
    {
        unsigned long width, height;
        unsigned char *rowptr[1];           // pointer to an array
        struct jpeg_decompress_struct info; // for our jpeg info
        struct jpeg_error_mgr err;          // the error handler
        const int num_components = 3;       // RGB has 3 components

        FilePtr file = std::unique_ptr<FILE, FileCloser>(fopen(path.string().c_str(), "rb"), FileCloser());
        if (!file)
        {
            return std::unexpected(Error::FileNotFound);
        }

        info.err = jpeg_std_error(&err);
        jpeg_create_decompress(&info); // fills info structure

        jpeg_stdio_src(&info, file.get()); // set source file
        jpeg_read_header(&info, TRUE);     // read jpeg file header

        jpeg_start_decompress(&info); // decompress the file

        // set width and height
        width = info.output_width;
        height = info.output_height;
        if (info.num_components != num_components)
        {

            jpeg_destroy_decompress(&info);
            return std::unexpected(Error::InvalidFormat);
        }

        Image<RGB> ret((unsigned)width, (unsigned)height);
        unsigned char *data = reinterpret_cast<unsigned char *>(ret.getDataPointer());

        //--------------------------------------------
        // read scanlines one at a time & put bytes
        //    in jdata[] array. Assumes an RGB image
        //--------------------------------------------
        while (info.output_scanline < info.output_height) // loop
        {
            rowptr[0] = data +
                        num_components * info.output_width * info.output_scanline;

            jpeg_read_scanlines(&info, rowptr, 1);
        }
        //---------------------------------------------------

        jpeg_finish_decompress(&info); // finish decompressing

        jpeg_destroy_decompress(&info);

        return ret;
    }
};