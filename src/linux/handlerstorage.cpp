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
    QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                         QObject::tr("Could not find xdg-mime"));
    return;
  }

  // look up nxmhandler desktop file
  QString desktopFile = getHandlerRegPath(schema);

  QFile file(desktopFile);
  if (!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
    QMessageBox::warning(nullptr, QObject::tr("Error registering nxm handler"),
                         QObject::tr("An error occurred while opening %1: %2")
                             .arg(desktopFile, file.errorString()));
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
  out << fileTemplate.arg(getenv("APPIMAGE") == nullptr ? proxyPath
                                                        : proxyPath % " nxmhandler"_L1);

  file.close();

  QProcess p;
  p.setProgram(xdgMime);
  p.setArguments(
      {u"default"_s, u"nxmhandler.desktop"_s, "x-scheme-handler/"_L1 % schema});

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
