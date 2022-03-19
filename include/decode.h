

#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

/* Greyscale formats */
// Decodes the given image with the given dimensions from 4-bit intensity format to RGBA8.
void btiDecodeI4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from 8-bit intensity format to RGBA8.
void btiDecodeI8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from 4-bit intensity + alpha format to RGBA8.
void btiDecodeIA4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from 8-bit intensity + alpha format to RGBA8.
void btiDecodeIA8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

/* Color formats */
// Decodes the given image with the given dimensions from RGB565 format to RGBA8.
void btiDecodeRGB565(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGB5A3 format to RGBA8.
void btiDecodeRGB5A3(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);
// Decodes the given image with the given dimensions from RGBA32 format to RGBA8.
void btiDecodeRGBA32(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

/* Palette formats */
// Decodes the given image with the given dimensions from 4-bit paletted format to RGBA8.
void btiDecodeC4(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat);
// Decodes the given image with the given dimensions from 8-bit paletted format to RGBA8.
void btiDecodeC8(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height, uint8_t* palette, uint8_t paletteFormat);

/* CMPR (S3TC/DXT1) */
// Decodes the given image with the given dimensions from DXT1-compressed format to RGBA8.
void btiDecodeCMPR(uint8_t* dst, uint8_t* src, uint16_t width, uint16_t height);

#endif