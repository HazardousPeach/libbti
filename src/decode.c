#include "decode.h"

#include <string.h>
#include <stdlib.h>

#define BytesToShort(a, b) (a << 8 | b)
#define BytesToInt(a, b, c, d) (a << 24 | b << 16 | c << 8 | d)

#define PALETTE_IA8 0
#define PALETTE_RGB565 1
#define PALETTE_RGB5A3 2

void btiDecodeI4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 8;
    uint32_t numBlocksH = height / 8;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 8; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 8; pixelX += 2) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 8 + pixelX >= width) || (blockY * 8 + pixelY >= height))
                        continue;

                    uint8_t data = src[offset++];

                    // Each byte represents two pixels.
                    uint8_t pixel0 = (data & 0xF0) >> 4;
                    uint8_t pixel1 = (data & 0x0F);

                    uint32_t destIndex = (width * ((blockY * 8) + pixelY) + (blockX * 8) + pixelX) * 4;

                    dst[destIndex] = pixel0 * 0x11;
                    dst[destIndex + 1] = pixel0 * 0x11;
                    dst[destIndex + 2] = pixel0 * 0x11;
                    dst[destIndex + 3] = pixel0 * 0x11;

                    dst[destIndex + 4] = pixel1 * 0x11;
                    dst[destIndex + 5] = pixel1 * 0x11;
                    dst[destIndex + 6] = pixel1 * 0x11;
                    dst[destIndex + 7] = pixel1 * 0x11;
                }
            }
        }
    }
}

void btiDecodeI8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 8;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 8; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 8 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    // Each byte represents one pixel.
                    uint8_t data = src[offset++];

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 8) + pixelX) * 4;

                    dst[destIndex] = data;
                    dst[destIndex + 1] = data;
                    dst[destIndex + 2] = data;
                    dst[destIndex + 3] = data;
                }
            }
        }
    }
}

void btiDecodeIA4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 8;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 8; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 8 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    uint8_t data = src[offset++];

                    // Each byte contains alpha and luminance of the current pixel.
                    uint8_t alpha = (data & 0xF0) >> 4;
                    uint8_t luminance = (data & 0x0F);

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 8) + pixelX) * 4;

                    dst[destIndex] = luminance * 0x11;
                    dst[destIndex + 1] = luminance * 0x11;
                    dst[destIndex + 2] = luminance * 0x11;
                    dst[destIndex + 3] = alpha * 0x11;
                }
            }
        }
    }
}

void btiDecodeIA8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 4;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 4 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    // The alpha and luminance values of the current pixel are stored in two bytes.
                    uint8_t alpha = src[offset++];
                    uint8_t luminance = src[offset++];

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 4) + pixelX) * 4;

                    dst[destIndex] = luminance;
                    dst[destIndex + 1] = luminance;
                    dst[destIndex + 2] = luminance;
                    dst[destIndex + 3] = alpha;
                }
            }
        }
    }
}

uint32_t RGB565toRGBA8(uint16_t data) {
    uint8_t r = (data & 0xF100) >> 11;
    uint8_t g = (data & 0x07E0) >> 5;
    uint8_t b = (data & 0x001F);

    uint32_t output = 0x000000FF;
    output |= (r << 3) << 24;
    output |= (g << 2) << 16;
    output |= (b << 3) << 8;

    return output;
}

void btiDecodeRGB565(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 4;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 4 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    // RGB values for this pixel are stored in a 16-bit integer.
                    uint16_t data = BytesToShort(src[offset++], src[offset++]);
                    uint32_t rgba8 = RGB565toRGBA8(data);

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 4) + pixelX) * 4;

                    dst[destIndex] = (rgba8 & 0xFF000000) >> 24;
                    dst[destIndex + 1] = (rgba8 & 0x00FF0000) >> 16;
                    dst[destIndex + 2] = (rgba8 & 0x0000FF00) >> 8;
                    dst[destIndex + 3] = rgba8 & 0x000000FF;
                }
            }
        }
    }
}

uint32_t RGB5A3toRGBA8(uint16_t data) {
    uint8_t r, g, b, a;

    // No alpha bits to extract.
    if (data & 0x8000) {
        a = 0xFF;

        r = (data & 0x7C00) >> 10;
        g = (data & 0x03E0) >> 5;
        b = (data & 0x001F);

        r = (r << (8 - 5)) | (r >> (10 - 8));
        g = (g << (8 - 5)) | (g >> (10 - 8));
        b = (b << (8 - 5)) | (b >> (10 - 8));
    }
    // Alpha bits present.
    else {
        a = (data & 0x7000) >> 12;
        r = (data & 0x0F00) >> 8;
        g = (data & 0x00F0) >> 4;
        b = (data & 0x000F);

        a = (a << (8 - 3)) | (a << (8 - 6)) | (a >> (9 - 8));
        r = (r << (8 - 4)) | r;
        g = (g << (8 - 4)) | g;
        b = (b << (8 - 4)) | b;
    }

    uint32_t output = a;
    output |= r << 24;
    output |= g << 16;
    output |= b << 8;

    return output;
}

void btiDecodeRGB5A3(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 4;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 4 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    // RGBA values for this pixel are stored in a 16-bit integer.
                    uint16_t data = BytesToShort(src[offset++], src[offset++]);
                    uint32_t rgba8 = RGB5A3toRGBA8(data);

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 4) + pixelX) * 4;

                    dst[destIndex] = (rgba8 & 0xFF000000) >> 24;
                    dst[destIndex + 1] = (rgba8 & 0x00FF0000) >> 16;
                    dst[destIndex + 2] = (rgba8 & 0x0000FF00) >> 8;
                    dst[destIndex + 3] = rgba8 & 0x000000FF;
                }
            }
        }
    }
}

void btiDecodeRGBA32(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 4;
    uint32_t numBlocksH = height / 4;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block

            // Alpha/red values for current pixel
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 4 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 4) + pixelX) * 4;

                    dst[destIndex + 3] = src[offset++];
                    dst[destIndex + 2] = src[offset++];
                }
            }

            // Green/blue values for current pixel
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 4 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 4) + pixelX) * 4;

                    dst[destIndex + 1] = src[offset++];
                    dst[destIndex] = src[offset++];
                }
            }
        }
    }
}

void UnpackPixelFromPalette(uint8_t* palette, uint8_t paletteFormat, uint8_t index, uint32_t offset, uint8_t* dst) {
    if (paletteFormat == PALETTE_IA8) {
        dst[offset] = palette[2 * index + 1];
        dst[offset + 1] = palette[2 * index];
    }
    else if (paletteFormat == PALETTE_RGB565) {
        uint16_t paletteColor = (palette[2 * index]) << 8 | (palette[2 * index + 1]);
        uint32_t rgba8 = RGB565toRGBA8(paletteColor);

        dst[offset] = (rgba8 & 0xFF000000) >> 24;
        dst[offset + 1] = (rgba8 & 0x00FF0000) >> 16;
        dst[offset + 2] = (rgba8 & 0x0000FF00) >> 8;
        dst[offset + 3] = rgba8 & 0x000000FF;
    }
    else if (paletteFormat == PALETTE_RGB5A3) {
        uint16_t paletteColor = (palette[2 * index]) << 8 | (palette[2 * index + 1]);
        uint32_t rgba8 = RGB5A3toRGBA8(paletteColor);

        dst[offset] = (rgba8 & 0xFF000000) >> 24;
        dst[offset + 1] = (rgba8 & 0x00FF0000) >> 16;
        dst[offset + 2] = (rgba8 & 0x0000FF00) >> 8;
        dst[offset + 3] = rgba8 & 0x000000FF;
    }
}

void btiDecodeC4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat) {
    if (src == NULL || palette == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 8;
    uint32_t numBlocksH = height / 8;

    // Holds the palette indices for each pixel in the image.
    uint8_t* paletteIndexBuffer = malloc(width * height * 8);
    if (paletteIndexBuffer == NULL)
        return;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 8; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 8; pixelX += 2) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 8 + pixelX >= width) || (blockY * 8 + pixelY >= height))
                        continue;

                    uint8_t data = src[offset];

                    // Each byte contains the palette indices for 2 pixels.
                    uint8_t pixel0 = (data & 0xF0) >> 4;
                    uint8_t pixel1 = (data & 0x0F);

                    uint32_t destIndex = (width * ((blockY * 8) + pixelY) + (blockX * 8) + pixelX);

                    paletteIndexBuffer[destIndex] = pixel0;
                    paletteIndexBuffer[destIndex + 1] = pixel1;
                }
            }
        }
    }

    uint32_t pixelSize = paletteFormat == PALETTE_IA8 ? 2 : 4;
    uint32_t destOffset = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            UnpackPixelFromPalette(palette, paletteFormat, paletteIndexBuffer[y * width + x], destOffset, dst);
            destOffset += pixelSize;
        }
    }

    free(paletteIndexBuffer);
}

void btiDecodeC8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat) {
    if (src == NULL || palette == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = width / 8;
    uint32_t numBlocksH = height / 4;

    // Holds the palette indices for each pixel in the image.
    uint8_t* paletteIndexBuffer = malloc(width * height * 8);
    if (paletteIndexBuffer == NULL)
        return;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Iterate the pixels in the current block
            for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
                for (uint32_t pixelX = 0; pixelX < 8; pixelX++) {
                    // Bounds check to ensure the pixel is within the image.
                    if ((blockX * 8 + pixelX >= width) || (blockY * 4 + pixelY >= height))
                        continue;

                    uint8_t data = src[offset++];
                    uint32_t destIndex = (width * ((blockY * 4) + pixelY) + (blockX * 8) + pixelX);

                    paletteIndexBuffer[destIndex] = data;
                }
            }
        }
    }

    uint32_t pixelSize = paletteFormat == PALETTE_IA8 ? 2 : 4;
    uint32_t destOffset = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            UnpackPixelFromPalette(palette, paletteFormat, paletteIndexBuffer[y * width + x], destOffset, dst);
            destOffset += pixelSize;
        }
    }

    free(paletteIndexBuffer);
}

uint8_t* DecodeCMPRSubBlock(uint8_t* src, uint32_t* offset) {
    uint8_t* data = malloc(4 * 4 * 4);
    
    if (data == NULL)
        return NULL;

    uint16_t color0 = BytesToShort((*offset)++, (*offset)++);
    uint16_t color1 = BytesToShort((*offset)++, (*offset)++);
    uint32_t bits = BytesToInt((*offset)++, (*offset)++, (*offset)++, (*offset)++);

    uint32_t colorTable[4] = {0, 0, 0, 0};
    colorTable[0] = RGB565toRGBA8(color0);
    colorTable[1] = RGB565toRGBA8(color1);

    uint8_t r0, g0, b0, a0, r1, g1, b1, a1;
    r0 = (colorTable[0] & 0xFF000000) >> 24;
    g0 = (colorTable[0] & 0x00FF0000) >> 16;
    b0 = (colorTable[0] & 0x0000FF00) >> 8;
    a0 = (colorTable[0] & 0x000000FF);

    r1 = (colorTable[1] & 0xFF000000) >> 24;
    g1 = (colorTable[1] & 0x00FF0000) >> 16;
    b1 = (colorTable[1] & 0x0000FF00) >> 8;
    a1 = (colorTable[1] & 0x000000FF);

    if (color0 > color1) {
        colorTable[2] |= ((2 * r0 + r1) / 3) << 24;
        colorTable[2] |= ((2 * g0 + g1) / 3) << 16;
        colorTable[2] |= ((2 * b0 + b1) / 3) << 8;
        colorTable[2] |= 0xFF;

        colorTable[3] |= ((r0 + 2 * r1) / 3) << 24;
        colorTable[3] |= ((g0 + 2 * g1) / 3) << 16;
        colorTable[3] |= ((b0 + 2 * b1) / 3) << 8;
        colorTable[3] |= 0xFF;
    }
    else {
        colorTable[2] |= ((r0 + r1) / 2) << 24;
        colorTable[2] |= ((g0 + g1) / 2) << 16;
        colorTable[2] |= ((b0 + b1) / 2) << 8;
        colorTable[2] |= 0xFF;

        colorTable[3] |= ((r0 + 2 * r1) / 3) << 24;
        colorTable[3] |= ((g0 + 2 * g1) / 3) << 16;
        colorTable[3] |= ((b0 + 2 * b1) / 3) << 8;
        colorTable[3] |= 0x00;
    }

    for (uint32_t pixelY = 0; pixelY < 4; pixelY++) {
        for (uint32_t pixelX = 0; pixelX < 4; pixelX++) {
            uint32_t i = pixelY * 4 + pixelX;
            uint32_t bitOffset = (15 - i) * 2;
            uint32_t di = i * 4;
            uint32_t si = (bits >> bitOffset) & 3;

            data[di + 0] = (colorTable[si] & 0xFF000000) >> 24;
            data[di + 1] = (colorTable[si] & 0x00FF0000) >> 16;
            data[di + 2] = (colorTable[si] & 0x0000FF00) >> 8;
            data[di + 3] = (colorTable[si] & 0x000000FF);
        }
    }

    return data;
}

void btiDecodeCMPR(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height) {
    if (src == NULL || dst == NULL)
        return;

    uint32_t offset = 0;
    uint32_t numBlocksW = (width + 7) / 8;
    uint32_t numBlocksH = (height + 7) / 8;

    // Iterate the blocks in the image
    for (uint32_t blockY = 0; blockY < numBlocksH; blockY++) {
        for (uint32_t blockX = 0; blockX < numBlocksW; blockX++) {
            // Each block has a set of 2x2 sub-blocks.
            for (uint32_t subBlockY = 0; subBlockY < 2; subBlockY++) {
                for (uint32_t subBlockX = 0; subBlockX < 2; subBlockX++) {
                    uint32_t subBlockWidth = max(0, min(4, width - (subBlockX * 4 + blockX * 8)));
                    uint32_t subBlockHeight = max(0, min(4, height - (subBlockY * 4 + blockY * 8)));

                    uint8_t* subBlockData = DecodeCMPRSubBlock(src, &offset);
                    if (subBlockData == NULL)
                        return;

                    for (uint32_t pixelY = 0; pixelY < subBlockHeight; pixelY++) {
                        uint32_t destX = blockX * 8 + subBlockX * 4;
                        uint32_t destY = blockY * 8 + (subBlockY * 4) + pixelY;

                        if (destX >= width || destY >= height)
                            continue;

                        uint32_t destOffset = (destY * width + destX) * 4;
                        memcpy(dst + destOffset, subBlockData + (pixelY * 4 * 4), subBlockWidth * 4);
                    }

                    free(subBlockData);
                }
            }
        }
    }
}
