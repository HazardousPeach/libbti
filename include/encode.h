#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

/* Greyscale formats */
// Decodes the given image with the given dimensions from RGBA8 format to 4-bit intensity.
void btiEncodeI4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA8 format to 8-bit intensity.
void btiEncodeI8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA8 format to 4-bit intensity + alpha.
void btiEncodeIA4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA8 format to 8-bit intensity + alpha.
void btiEncodeIA8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

/* Color formats */
// Decodes the given image with the given dimensions from RGBA8 format to RGB565.
void btiEncodeRGB565(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA8 format to RGB5A3.
void btiEncodeRGB5A3(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA8 format to RGBA32.
void btiEncodeRGBA32(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

/* Palette formats */
// Decodes the given image with the given dimensions from RGBA8 format to 4-bit paletted.
void btiEncodeC4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat);
// Decodes the given image with the given dimensions from RGBA8 format to 8-bit paletted.
void btiEncodeC8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat);

/* CMPR (S3TC/DXT1) */
// Decodes the given image with the given dimensions from RGBA8 format to DXT1-compressed.
void btiEncodeCMPR(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

#endif