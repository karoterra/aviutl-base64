#pragma once

#include <string>

class Base64Encoder
{
public:
    static constexpr char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    bool _isClosed;
    int _state;
    char _prev;
    size_t _prefixLen;
    std::string _suffix;
    std::string _buf;

    Base64Encoder(const char *prefix, const char *suffix);

    void write(const unsigned char *data, size_t size);

    void finalize();

    bool isClosed() const noexcept;

    void reserve(size_t dataSize);

    const char *c_str() const;
};

