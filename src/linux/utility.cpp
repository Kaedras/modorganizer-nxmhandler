#include <QCoreApplication>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QString>

using namespace Qt::StringLiterals;

QString getHandlerRegPath(const QString&)
{
  QString location = QStandardPaths::locate(QStandardPaths::ApplicationsLocation,
                                            u"nxmhandler.desktop"_s);
  if (!location.isEmpty()) {
    return location;
  }

  return QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) %
         "/nxmhandler.desktop"_L1;
}

QString getGlobalHandlerRegPath()
{
  return getHandlerRegPath({});
}

QString getApplicationFilePath()
{
  QString executable = qEnvironmentVariable("APPIMAGE");
  if (!executable.isEmpty()) {
    return executable;
  }
  return QCoreApplication::applicationFilePath();
}

QString getApplicationDirPath()
{
  QString appDir = qEnvironmentVariable("APPDIR");
  if (!appDir.isEmpty()) {
    return appDir;
  }
  return QCoreApplication::applicationDirPath();
}

bool isNxmHandlerExecutable(const QString& str)
{
  if (getenv("APPIMAGE") != nullptr) {
    // nxmhandler is a subcommand in the modorganizer appimage
    static const QRegularExpression regex(u"ModOrganizer-.*.AppImage"_s);
    return str.contains(regex);
  }

  return str.endsWith("nxmhandler"_L1, Qt::CaseInsensitive);
}
