#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

QString getHandlerRegPath()
{
  const QStringList applicationDirs =
    QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

  const QStringList filter = {u"*nxmhandler.desktop"_s};
  for (const auto& dir : applicationDirs) {
    QFileInfoList entryList = QDir(dir).entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot);
    if (!entryList.empty()) {
      return entryList.first().absoluteFilePath();
    }
  }

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
