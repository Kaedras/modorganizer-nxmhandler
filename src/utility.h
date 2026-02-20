#pragma once

#include <QString>

#ifdef _WIN32
static const auto handlerRegKey = QStringLiteral("shell/open/command/Default");
static constexpr const char* fileDialogFilter = "Executable (*.exe)";
#else
static const auto handlerRegKey = QStringLiteral("Desktop Entry/Exec");
static constexpr const char* fileDialogFilter = "Executable";
#endif

QString getHandlerRegPath();
QString getGlobalHandlerRegPath();
QString getApplicationFilePath();
QString getApplicationDirPath();
bool isNxmHandlerExecutable(const QString& str);
