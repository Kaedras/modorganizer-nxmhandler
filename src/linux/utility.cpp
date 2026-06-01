#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QString>

using namespace Qt::StringLiterals;

QString getHandlerRegPath(const QString&)
{
  if (getenv("APPIMAGE") != nullptr) {
    const QStringList applicationDirs =
        QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    const QStringList filter = {u"*nxmhandler.desktop"_s};
    for (const auto& dir : applicationDirs) {
      QFileInfoList entryList =
          QDir(dir).entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot);
      if (!entryList.empty()) {
        return entryList.first().absoluteFilePath();
      }
    }
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
