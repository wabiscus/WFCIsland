#pragma once

#ifdef _WIN32
#include <string>

// filter : ex. "PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0"
// defExt : ex. "png"
// Retourne le chemin choisi, ou une chaîne vide si annulé.
std::string ShowSaveDialog(
    const std::string& defaultFilename,
    const char* filter,
    const char* defExt);
#endif