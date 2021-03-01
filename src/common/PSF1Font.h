#ifndef PSF1_FONT_H
#define PSF1_FONT_H

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char characterSize;
} PSF1Header;

typedef struct {
    PSF1Header *header;
    void *glyphBuffer;
} PSF1Font;

#endif
