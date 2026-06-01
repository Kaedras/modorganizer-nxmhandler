#pragma once

#include <QString>

#ifdef _WIN32
static const auto handlerRegKey = QStringLiteral("shell/open/command/Default");
static constexpr const char* fileDialogFilter = "Executable (*.exe)";
#else
static const auto handlerRegKey               = QStringLiteral("Desktop Entry/Exec");
static constexpr const char* fileDialogFilter = "Executable";
#endif

// return the registry key/desktop file to use for the specified schema
QString getHandlerRegPath(const QString& schema);

// return the registry key/desktop file to use for the global handler
QString getGlobalHandlerRegPath();

// helper function for appimage support
// calls QCoreApplication::applicationFilePath() when running normally
QString getApplicationFilePath();
// helper function for appimage support
// calls QCoreApplication::applicationDirPath() when running normally
QString getApplicationDirPath();

bool isNxmHandlerExecutable(const QString& str);
