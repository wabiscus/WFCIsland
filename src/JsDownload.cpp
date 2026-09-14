#ifdef __EMSCRIPTEN__
#include <cstdint>
#include <emscripten.h>

EM_JS(void, js_download_file, (const char* filename, const uint8_t* data, int size, const char* mime), {
    const bytes = new Uint8Array(Module.HEAPU8.buffer, data, size);
    const blob = new Blob([bytes], { type: UTF8ToString(mime) });
    const url = URL.createObjectURL(blob);

    const a = document.createElement('a');
    a.href = url;
    a.download = UTF8ToString(filename);
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);

    URL.revokeObjectURL(url);
});
#endif