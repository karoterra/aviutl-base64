#include "Base64Encoder.h"

Base64Encoder::Base64Encoder(const char *prefix, const char *suffix) :
    _isClosed(false),
    _state(0),
    _prev(0),
    _suffix(suffix),
    _buf(prefix)
{
    _prefixLen = _buf.length();
}

void Base64Encoder::write(const unsigned char *data, size_t size)
{
    if (isClosed()) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        switch (_state) {
        case 0:
            _buf.push_back(table[data[i] >> 2]);
            _prev = (data[i] & 0x3) << 4;
            break;
        case 1:
            _buf.push_back(table[_prev | (data[i] >> 4)]);
            _prev = (data[i] & 0xF) << 2;
            break;
        case 2:
            _buf.push_back(table[_prev | (data[i] >> 6)]);
            _buf.push_back(table[data[i] & 0x3F]);
            _prev = 0;
            break;
        }

        _state = (_state + 1) % 3;
    }
}

void Base64Encoder::finalize()
{
    _isClosed = true;

    switch (_state) {
    case 1:
    case 2:
        _buf.push_back(table[_prev]);
        break;
    }

    const size_t dataSize = _buf.length() - _prefixLen;
    const size_t n = (4 - dataSize % 4) % 4;
    for (size_t i = 0; i < n; i++) {
        _buf.push_back('=');
    }

    _buf += _suffix;
}

bool Base64Encoder::isClosed() const noexcept
{
    return _isClosed;
}

void Base64Encoder::reserve(size_t dataSize)
{
    size_t capSize = (dataSize + 2) / 3 * 4;
    _buf.reserve(capSize);
}

const char *Base64Encoder::c_str() const
{
    return _buf.c_str();
}
