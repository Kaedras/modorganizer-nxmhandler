#pragma once

#include <QString>

#ifdef _WIN32
static constexpr const char* handlerRegKey = "shell/open/command/Default";
static constexpr const char* fileDialogFilter = "Executable (*.exe)";
#else
static constexpr const char* handlerRegKey = "Desktop Entry/Exec";
static constexpr const char* fileDialogFilter = "Executable";
#endif

QString getHandlerRegPath();
QString getGlobalHandlerRegPath();
QString getApplicationFilePath();
QString getApplicationDirPath();
bool isNxmHandlerExecutable(const QString& str);
