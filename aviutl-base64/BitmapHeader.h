#pragma once

struct BitmapHeader {
    static constexpr char fileType[] = "BM";

    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;

    unsigned int infoSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitSize;
    unsigned int compression;
    unsigned int dataSize;
    unsigned int xPixelPerMeter;
    unsigned int yPixelPerMeter;
    unsigned int colorUsed;
    unsigned int colorImportant;

    BitmapHeader(int width, int height) :
        fileSize(54 + 4 * width * height),
        reserved1(0),
        reserved2(0),
        offset(54),
        infoSize(40),
        width(width),
        height(height),
        planes(1),
        bitSize(32),
        compression(0),
        dataSize(4 * width * height),
        xPixelPerMeter(0),
        yPixelPerMeter(0),
        colorUsed(0),
        colorImportant(0)
    {
    }
};