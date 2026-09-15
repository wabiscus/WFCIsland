#pragma once

#ifdef _WIN32
#include <string>

/** @brief Show a save dialog
 * @param defaultFilename The default filename to display in the dialog
 * @param filter The file type filter for the dialog
 * @param defExt The default file extension to use if the user doesn't specify one
 * @return The selected file path or an empty string if the user cancels the dialog
 */
// filter : ex. "PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0"
// defExt : ex. "png"
// Returns the selected file path or an empty string if the user cancels the dialog
std::string ShowSaveDialog(
    const std::string &defaultFilename,
    const char *filter,
    const char *defExt);
#endif