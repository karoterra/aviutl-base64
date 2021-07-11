#include <stdint.h>
#include <vector>
#include <lua.hpp>
#include <png.h>
#include "Base64Encoder.h"
#include "BitmapHeader.h"
#include "MD5.h"

using namespace std;

struct BGRA {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
};

int encodeBMP(lua_State *L) {
    const uint8_t *data = static_cast<uint8_t *>(lua_touserdata(L, 1));
    int w = lua_tointeger(L, 2);
    int h = lua_tointeger(L, 3);
    const char *prefix = lua_tostring(L, 4);
    const char *suffix = lua_tostring(L, 5);

    BitmapHeader header(w, h);
    Base64Encoder encoder(prefix, suffix);

    encoder.write(reinterpret_cast<const uint8_t *>(header.fileType), 2);
    encoder.write(reinterpret_cast<const uint8_t *>(&header), sizeof(header));
    for (int y = h - 1; y >= 0; y--) {
        encoder.write(&data[4 * y * w], 4 * w);
    }
    encoder.finalize();

    lua_pushstring(L, encoder.c_str());

    return 1;
}

void png_write_data(png_structp png_ptr, png_bytep data, size_t length) {
    if (png_ptr == nullptr) {
        return;
    }

    Base64Encoder *encoder = reinterpret_cast<Base64Encoder *>(png_get_io_ptr(png_ptr));
    encoder->write(data, length);
}

void png_flush_data(png_structp png_ptr) {
    return;
}

int encodePNG(lua_State *L) {
    uint8_t *data = static_cast<uint8_t *>(lua_touserdata(L, 1));
    int w = lua_tointeger(L, 2);
    int h = lua_tointeger(L, 3);
    const char *prefix = lua_tostring(L, 4);
    const char *suffix = lua_tostring(L, 5);

    vector<png_bytep> rows(h);
    for (int y = 0; y < h; y++) {
        rows[y] = &data[y * w * 4];
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        lua_pushstring(L, "");
        return 1;
    }
    png_infop pngInfo = png_create_info_struct(png);
    if (!pngInfo) {
        png_destroy_write_struct(&png, nullptr);
        lua_pushstring(L, "");
        return 1;
    }
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &pngInfo);
        lua_pushstring(L, "");
        return 1;
    }

    Base64Encoder encoder(prefix, suffix);
    png_set_write_fn(png, (png_voidp)&encoder, png_write_data, png_flush_data);
    png_set_IHDR(png, pngInfo, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, pngInfo);
    png_set_bgr(png);
    png_write_image(png, rows.data());
    png_write_end(png, pngInfo);
    png_destroy_write_struct(&png, &pngInfo);
    encoder.finalize();

    lua_pushstring(L, encoder.c_str());
    return 1;
}

int hashMD5(lua_State *L) {
    uint8_t *data = static_cast<uint8_t *>(lua_touserdata(L, 1));
    int w = lua_tointeger(L, 2);
    int h = lua_tointeger(L, 3);

    MD5 md5;
    uint8_t digest[16];
    md5.update(data, 4U * w * h);
    md5.finalize(digest);

    char str[33] = { 0 };
    for (int i = 0; i < 16; i++) {
        sprintf_s(&str[i * 2], 33, "%02x", digest[i]);
    }
    lua_pushstring(L, str);
    return 1;
}

static luaL_Reg functions[] = {
    {"encodeBMP", encodeBMP},
    {"encodePNG", encodePNG},
    {"hashMD5", hashMD5},
    {nullptr, nullptr},
};

extern "C" __declspec(dllexport) int luaopen_KaroterraBase64(lua_State * L) {
    luaL_register(L, "KaroterraBase64", functions);
    return 1;
}
