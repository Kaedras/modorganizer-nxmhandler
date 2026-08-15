#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>

using namespace std;
using namespace Qt::StringLiterals;

namespace
{
void showError(const QString& message)
{
  QMessageBox::warning(
      nullptr, QObject::tr("Error registering nxm handler"),
      QObject::tr("An error occurred while registering nxmhandler: %1").arg(message));
}
}  // namespace

void HandlerStorage::registerSchemaProxy(const QString& proxyPath,
                                         const QString& schema)
{
  // look for xdg-mime
  const QString xdgMime = QStandardPaths::findExecutable(u"xdg-mime"_s);
  if (xdgMime.isEmpty()) {
    QMessageBox::warning(nullptr, QObject::tr("Error registering url handler"),
                         QObject::tr("Could not find xdg-mime"));
    return;
  }

  const QStringList applicationDirs =
      QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

  // look up nxmhandler desktop file
  QString desktopFilePath;
  QString desktopFileName;
  const QStringList filter = {u"*nxmhandler.desktop"_s};
  for (const auto& dir : applicationDirs) {
    QFileInfoList entryList =
        QDir(dir).entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot);
    if (!entryList.empty()) {
      desktopFilePath = entryList.first().absoluteFilePath();
      desktopFileName = entryList.first().fileName();
      break;
    }
  }

  // only make changes to desktop files if not running in a container
  if (getenv("container") == nullptr) {
    if (desktopFilePath.isEmpty()) {
      // desktop file does not exist, create it in XDG_DATA_HOME
      desktopFilePath = applicationDirs.first() % "/nxmhandler.desktop"_L1;
      desktopFileName = u"nxmhandler.desktop"_s;
    }

    QFile file(desktopFilePath);
    if (!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
      QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                           QObject::tr("An error occurred while creating %1: %2")
                               .arg(desktopFilePath, file.errorString()));
      return;
    }

    const QString fileTemplate =
        u"[Desktop Entry]\n"
        "Type=Application\n"
        "Name=NXM Handler\n"
        "Exec=%1 %u\n"
        "StartupNotify=false\n"
        "NoDisplay=true\n"
        "MimeType=x-scheme-handler/nxm;x-scheme-handler/modl\n"_s;

    QTextStream out(&file);
    out << fileTemplate.arg(
        getenv("APPIMAGE") == nullptr ? proxyPath : proxyPath % " nxmhandler"_L1);

    file.close();
  } else {
    if (desktopFilePath.isEmpty()) {
      QMessageBox::warning(
          nullptr, QObject::tr("Error registering nxm handler"),
          QObject::tr(
              "Application is running as flatpak and no desktop file has been found"));
      return;
    }
  }

  QProcess p;
  p.setProgram(xdgMime);
  p.setArguments({u"default"_s, desktopFileName, "x-scheme-handler/"_L1 % schema});

  p.start();

  if (!p.waitForStarted() || !p.waitForFinished()) {
    showError(p.errorString());
    return;
  }

  int status = p.exitCode();
  if (status != 0) {
    QString message;
    switch (status) {
    case -1:
      message = strerror(errno);
      break;
    case 1:
      message = u"Error in command line syntax."_s;
      break;
    case 2:
      message = u"One of the files passed on the command line did not exist."_s;
      break;
    case 3:
      message = u"A required tool could not be found."_s;
      break;
    case 4:
      message = u"The action failed."_s;
      break;
    case 5:
      message = u"No permission to read one of the files passed on the command line."_s;
      break;
    default:
      message = u"Unknown error."_s;
      break;
    }

    showError(message);
  }
}
