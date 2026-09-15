#pragma once

#ifdef __EMSCRIPTEN__
#include <cstdint>

/** @brief Download a file from JavaScript
 * @param filename The name of the file to download
 * @param data The data to download
 * @param size The size of the data
 * @param mime The MIME type of the file
 */
extern "C" void js_download_file(
    const char* filename,
    const uint8_t* data,
    int size,
    const char* mime);
#endif