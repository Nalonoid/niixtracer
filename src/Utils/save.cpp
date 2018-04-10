#include <stdlib.h>
#include <stdio.h>

#include "Image/image.hpp"
#include "Utils/save.hpp"

void save2bmp(const char *path, Image img, unsigned dpi)
{
    unsigned width      { img.width()        };
    unsigned height     { img.height()       };
    unsigned data_size  { 4 * width * height };
    unsigned file_size  { 54 + data_size     };

    double factor { 39.375                        };
    unsigned m    { static_cast<unsigned>(factor) };
    unsigned ppm  { dpi * m                       };

    unsigned char file_header[14] = { 'B', 'M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    unsigned char info_header[40] = { 40,0,0,0, 0,0,0,0, 0,0, 0,0, 1,0,24,0 };

    file_header[ 2] = (unsigned char) (file_size      );
    file_header[ 3] = (unsigned char) (file_size >>  8);
    file_header[ 4] = (unsigned char) (file_size >> 16);
    file_header[ 5] = (unsigned char) (file_size >> 24);

    info_header[ 4] = (unsigned char) (width      );
    info_header[ 5] = (unsigned char) (width >>  8);
    info_header[ 6] = (unsigned char) (width >> 16);
    info_header[ 7] = (unsigned char) (width >> 24);

    info_header[ 8] = (unsigned char) (height      );
    info_header[ 9] = (unsigned char) (height >>  8);
    info_header[10] = (unsigned char) (height >> 16);
    info_header[11] = (unsigned char) (height >> 24);

    info_header[21] = (unsigned char) (data_size      );
    info_header[22] = (unsigned char) (data_size >>  8);
    info_header[23] = (unsigned char) (data_size >> 16);
    info_header[24] = (unsigned char) (data_size >> 24);

    info_header[25] = (unsigned char) (ppm      );
    info_header[26] = (unsigned char) (ppm >>  8);
    info_header[27] = (unsigned char) (ppm >> 16);
    info_header[28] = (unsigned char) (ppm >> 24);

    info_header[29] = (unsigned char) (ppm      );
    info_header[30] = (unsigned char) (ppm >>  8);
    info_header[31] = (unsigned char) (ppm >> 16);
    info_header[32] = (unsigned char) (ppm >> 24);

    FILE *file { fopen(path, "wb") };

    fwrite(file_header, 1, 14, file);
    fwrite(info_header, 1, 40, file);

    Color c { Colors::BLACK };

    for (unsigned j {0}; j < height; ++j)
    {
        for (unsigned i {0}; i < width; ++i)
        {
            c = img.pixels()[i][j].color;

            // Using the bmp format pixel values must be stored upside-down
            unsigned char color[3] { (unsigned char) (int)(c.b()*255.0)
                                    ,(unsigned char) (int)(c.g()*255.0)
                                    ,(unsigned char) (int)(c.r()*255.0) };

            fwrite(color, 1, 3, file);
        }
    }

    fclose(file);
}
