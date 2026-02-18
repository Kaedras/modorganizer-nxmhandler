#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

using namespace std::string_literals;
using namespace Qt::StringLiterals;

void HandlerStorage::registerProxy(const QString &proxyPath)
{
  // look for xdg-mime
  if (QStandardPaths::findExecutable(u"xdg-mime"_s).isEmpty()) {
    QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                     QObject::tr( "Could not find xdg-mime"));
    return;
  }

  // create a list of application directories based on XDG Base Directory Specification Version 0.8
  // see https://specifications.freedesktop.org/basedir/latest/ for details

  QString xdgDataHome = qEnvironmentVariable("XDG_DATA_HOME");
  if (xdgDataHome.isEmpty()) {
    // If $XDG_DATA_HOME is either not set or empty, a default equal to $HOME/.local/share should be used.
    xdgDataHome = QDir::homePath() % "/.local/share"_L1;
  }
  QStringList dataDirs = {xdgDataHome};
  const QString xdgDataDirs = qEnvironmentVariable("XDG_DATA_DIRS");
  if (xdgDataDirs.isEmpty()) {
    // If $XDG_DATA_DIRS is either not set or empty, a value equal to /usr/local/share/:/usr/share/ should be used.
    dataDirs << u"/usr/local/share"_s;
    dataDirs << u"/usr/share"_s;
  } else {
    dataDirs.append(xdgDataDirs.split(':'));
  }
  for (auto& dir:dataDirs) {
    dir.append(u"/applications"_s);
  }

  // look up nxmhandler desktop file
  QString desktopFilePath;
  std::string desktopFileName;
  const QStringList filter = {u"*nxmhandler*.desktop"_s};
  for (const auto& dir : dataDirs) {
    QFileInfoList entryList = QDir(dir).entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot);
    if (!entryList.empty()) {
      desktopFilePath = entryList.first().absoluteFilePath();
      desktopFileName = entryList.first().fileName().toStdString();
      break;
    }
  }

  // only make changes to desktop files if not running as flatpak
  if (getenv("container") != "flatpak"s) {
    if (desktopFilePath.isEmpty()) {
      // desktop file does not exist, create it in XDG_DATA_HOME
      desktopFilePath = xdgDataHome % "/applications/nxmhandler.desktop"_L1;
      desktopFileName = "nxmhandler.desktop";
    }

    QFile file(desktopFilePath);
    if (!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
      QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                           QObject::tr( "An error occurred while creating %1: %2").arg(desktopFilePath, file.errorString()));
      return;
    }

    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Type=Application\n";
    out << "Name=NXM Handler\n";
    out << "Exec=" << proxyPath << " %u\n";
    out << "StartupNotify=false\n";
    out << "MimeType=x-scheme-handler/nxm\n";
    file.close();
  } else {
    if (desktopFilePath.isEmpty()) {
      QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"), QObject::tr("Application is running as flatpak and no desktop file has been found"));
      return;
    }
  }

  // register nxm handler
  std::string command = std::format("xdg-mime default {} x-scheme-handler/nxm", desktopFileName);
  int status = system(command.c_str());
  if(status != 0) {
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

    QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                         QObject::tr( "An error occurred while registering nxmhandler: %1").arg(message));
  }
}
