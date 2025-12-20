#include <QCoreApplication>
#include <QString>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

QString getHandlerRegPath()
{
  return QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() % "/nxmhandler.desktop"_L1;
}

QString getGlobalHandlerRegPath()
{
  return getHandlerRegPath();
}

QString getApplicationFilePath()
{
  QString executable = qgetenv("APPIMAGE");
  if (!executable.isEmpty()) {
    return executable;
  }
  return  QCoreApplication::applicationFilePath();
}

QString getApplicationDirPath()
{
  QString appImage = qgetenv("APPIMAGE");
  if (!appImage.isEmpty()) {
    const qsizetype lastSlash = appImage.lastIndexOf('/');
    appImage.truncate(lastSlash);
    return appImage;
  }
  return QCoreApplication::applicationDirPath();
}

bool isNxmHandlerExecutable(const QString& str) {
  const QString appImage = qgetenv("APPIMAGE");
  if (!appImage.isEmpty()) {
    return str.endsWith("nxmhandler-x86_64.AppImage"_L1, Qt::CaseInsensitive);
  }

  return str.endsWith("nxmhandler"_L1, Qt::CaseInsensitive);
}
