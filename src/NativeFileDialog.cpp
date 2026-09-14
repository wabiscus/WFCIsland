#ifdef _WIN32
#include "NativeFileDialog.hpp"
#include <windows.h>
#include <commdlg.h>

std::string ShowSaveDialog(
    const std::string& defaultFilename,
    const char* filter,
    const char* defExt)
{
    char szFile[MAX_PATH];
    strncpy_s(szFile, defaultFilename.c_str(), sizeof(szFile) - 1);

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = defExt;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn))
        return std::string(szFile);

    return {}; // annulé par l'utilisateur
}
#endif