#pragma once

#ifdef __EMSCRIPTEN__
#include <cstdint>

extern "C" void js_download_file(
    const char* filename,
    const uint8_t* data,
    int size,
    const char* mime);
#endif