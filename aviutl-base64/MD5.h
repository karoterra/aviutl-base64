#pragma once

#include <stdint.h>

class MD5
{
public:
    MD5();

    void update(const uint8_t *data, size_t len);

    void finalize(uint8_t digest[16]);

private:
    static constexpr uint8_t Padding[64] = { 0x80, 0 };

    uint32_t _state[4];
    uint64_t _count;
    uint8_t _buf[64];

    void transform(const uint8_t block[64]);
};
