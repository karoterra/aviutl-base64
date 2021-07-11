#include <cmath>
#include <cstring>
#include "MD5.h"

inline uint32_t rotate(uint32_t x, int n)
{
    return (x << n) | (x >> (32 - n));
}

inline uint32_t F(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) | (~x & z);
}

inline uint32_t G(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & z) | (y & ~z);
}

inline uint32_t H(uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ y ^ z;
}

inline uint32_t I(uint32_t x, uint32_t y, uint32_t z)
{
    return y ^ (x | ~z);
}

inline uint32_t FF(
    uint32_t a, uint32_t b, uint32_t c, uint32_t d,
    uint32_t x, uint32_t s, uint32_t t
) {
    return b + rotate(a + F(b, c, d) + x + t, s);
}

inline uint32_t GG(
    uint32_t a, uint32_t b, uint32_t c, uint32_t d,
    uint32_t x, uint32_t s, uint32_t t
) {
    return b + rotate(a + G(b, c, d) + x + t, s);
}

inline uint32_t HH(
    uint32_t a, uint32_t b, uint32_t c, uint32_t d,
    uint32_t x, uint32_t s, uint32_t t
) {
    return b + rotate(a + H(b, c, d) + x + t, s);
}

inline uint32_t II(
    uint32_t a, uint32_t b, uint32_t c, uint32_t d,
    uint32_t x, uint32_t s, uint32_t t
) {
    return b + rotate(a + I(b, c, d) + x + t, s);
}

MD5::MD5()
{
    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
    _count = 0;
    for (size_t i = 0; i < 64; i++) {
        _buf[i] = 0;
    }
}

void MD5::update(const uint8_t *data, size_t len)
{
    size_t index = _count & 0x3F;
    size_t partLen = 64 - index;
    size_t i = 0;

    if (len >= partLen) {
        memcpy_s(&_buf[index], partLen, data, partLen);
        transform(_buf);

        for (i = partLen; i + 63 < len; i += 64) {
            transform(&data[i]);
        }
        index = 0;
    }

    memcpy_s(&_buf[index], 64 - index, &data[i], len - i);
    _count += len;
}

void MD5::finalize(uint8_t digest[16])
{
    size_t index = _count & 0x3F;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);
    uint64_t bitCount = _count << 3;

    update(Padding, padLen);
    update((const uint8_t *)&bitCount, 8);

    memcpy_s(digest, 16, _state, 16);

    _state[0] = 0x67452301;
    _state[1] = 0xEFCDAB89;
    _state[2] = 0x98BADCFE;
    _state[3] = 0x10325476;
    _count = 0;
}

void MD5::transform(const uint8_t block[64])
{
    uint32_t a = _state[0], b = _state[1], c = _state[2], d = _state[3];
    uint32_t x[16];

    memcpy_s(x, 64, block, 64);

    // Round 1
    a = FF(a, b, c, d, x[0], 7, 0xd76aa478);
    d = FF(d, a, b, c, x[1], 12, 0xe8c7b756);
    c = FF(c, d, a, b, x[2], 17, 0x242070db);
    b = FF(b, c, d, a, x[3], 22, 0xc1bdceee);
    a = FF(a, b, c, d, x[4], 7, 0xf57c0faf);
    d = FF(d, a, b, c, x[5], 12, 0x4787c62a);
    c = FF(c, d, a, b, x[6], 17, 0xa8304613);
    b = FF(b, c, d, a, x[7], 22, 0xfd469501);
    a = FF(a, b, c, d, x[8], 7, 0x698098d8);
    d = FF(d, a, b, c, x[9], 12, 0x8b44f7af);
    c = FF(c, d, a, b, x[10], 17, 0xffff5bb1);
    b = FF(b, c, d, a, x[11], 22, 0x895cd7be);
    a = FF(a, b, c, d, x[12], 7, 0x6b901122);
    d = FF(d, a, b, c, x[13], 12, 0xfd987193);
    c = FF(c, d, a, b, x[14], 17, 0xa679438e);
    b = FF(b, c, d, a, x[15], 22, 0x49b40821);
    // Round 2
    a = GG(a, b, c, d, x[1], 5, 0xf61e2562);
    d = GG(d, a, b, c, x[6], 9, 0xc040b340);
    c = GG(c, d, a, b, x[11], 14, 0x265e5a51);
    b = GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
    a = GG(a, b, c, d, x[5], 5, 0xd62f105d);
    d = GG(d, a, b, c, x[10], 9, 0x2441453);
    c = GG(c, d, a, b, x[15], 14, 0xd8a1e681);
    b = GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
    a = GG(a, b, c, d, x[9], 5, 0x21e1cde6);
    d = GG(d, a, b, c, x[14], 9, 0xc33707d6);
    c = GG(c, d, a, b, x[3], 14, 0xf4d50d87);
    b = GG(b, c, d, a, x[8], 20, 0x455a14ed);
    a = GG(a, b, c, d, x[13], 5, 0xa9e3e905);
    d = GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
    c = GG(c, d, a, b, x[7], 14, 0x676f02d9);
    b = GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);
    // Round 2
    a = HH(a, b, c, d, x[5], 4, 0xfffa3942);
    d = HH(d, a, b, c, x[8], 11, 0x8771f681);
    c = HH(c, d, a, b, x[11], 16, 0x6d9d6122);
    b = HH(b, c, d, a, x[14], 23, 0xfde5380c);
    a = HH(a, b, c, d, x[1], 4, 0xa4beea44);
    d = HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
    c = HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
    b = HH(b, c, d, a, x[10], 23, 0xbebfbc70);
    a = HH(a, b, c, d, x[13], 4, 0x289b7ec6);
    d = HH(d, a, b, c, x[0], 11, 0xeaa127fa);
    c = HH(c, d, a, b, x[3], 16, 0xd4ef3085);
    b = HH(b, c, d, a, x[6], 23, 0x4881d05);
    a = HH(a, b, c, d, x[9], 4, 0xd9d4d039);
    d = HH(d, a, b, c, x[12], 11, 0xe6db99e5);
    c = HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
    b = HH(b, c, d, a, x[2], 23, 0xc4ac5665);
    // Round 2
    a = II(a, b, c, d, x[0], 6, 0xf4292244);
    d = II(d, a, b, c, x[7], 10, 0x432aff97);
    c = II(c, d, a, b, x[14], 15, 0xab9423a7);
    b = II(b, c, d, a, x[5], 21, 0xfc93a039);
    a = II(a, b, c, d, x[12], 6, 0x655b59c3);
    d = II(d, a, b, c, x[3], 10, 0x8f0ccc92);
    c = II(c, d, a, b, x[10], 15, 0xffeff47d);
    b = II(b, c, d, a, x[1], 21, 0x85845dd1);
    a = II(a, b, c, d, x[8], 6, 0x6fa87e4f);
    d = II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
    c = II(c, d, a, b, x[6], 15, 0xa3014314);
    b = II(b, c, d, a, x[13], 21, 0x4e0811a1);
    a = II(a, b, c, d, x[4], 6, 0xf7537e82);
    d = II(d, a, b, c, x[11], 10, 0xbd3af235);
    c = II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
    b = II(b, c, d, a, x[9], 21, 0xeb86d391);

    _state[0] += a;
    _state[1] += b;
    _state[2] += c;
    _state[3] += d;
}
