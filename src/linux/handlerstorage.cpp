#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

void HandlerStorage::registerProxy(const QString &proxyPath)
{
  QString path = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() + "nxmhandler.desktop";
  QSettings settings(path, QSettings::Format::IniFormat);

  QString myExe = QDir::toNativeSeparators(proxyPath).append(" %u");
  settings.beginGroup("Desktop Entry");
  settings.setValue("Type", "Application");
  settings.setValue("Name", "nxm handler");
  settings.setValue("Exec", myExe);
  settings.setValue("StartupNotify", false);
  settings.setValue("MimeType", "x-scheme-handler/nxm");
  settings.endGroup();
  settings.sync();

  int status = system("xdg-mime default nxmproxy.desktop x-scheme-handler/nxm");
  if(status != 0) {
    int error = errno;
    QMessageBox::warning(nullptr, QObject::tr("Error registering proxy"),
                         QObject::tr( "An error occurred while registering nxmhandler:\n"
                                     "%1\n"
                                     R"(Please run "xdg-mime default nxmhandler.desktop x-scheme-handler/nxm" manually.)").arg(strerror(error)));
  }
}
