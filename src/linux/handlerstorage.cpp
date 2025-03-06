#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

void HandlerStorage::registerProxy(const QString &proxyPath)
{
  QString path = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() + "/nxmhandler.desktop";

  QFile file(path);
  if (!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text))
  {
    int error = errno;
    QMessageBox::warning(nullptr, QObject::tr("Error registering proxy"),
                         QObject::tr( "An error occurred while registering nxmhandler:\n"
                                     "%1\n").arg(strerror(error)));
    return;
  }

  QString myExe = QDir::toNativeSeparators(proxyPath).append(" %u");

  QTextStream out(&file);
  out << "[Desktop Entry]\n";
  out << "Type=Application\n";
  out << "Name=NXM Handler\n";
  out << "Exec=" << myExe << "\n";
  out << "StartupNotify=false\n";
  out << "MimeType=x-scheme-handler/nxm\n";
  file.close();

  int status = system("xdg-mime default nxmproxy.desktop x-scheme-handler/nxm");
  if(status != 0) {
    int error = errno;
    QMessageBox::warning(nullptr, QObject::tr("Error registering proxy"),
                         QObject::tr( "An error occurred while registering nxmhandler:\n"
                                     "%1\n"
                                     R"(Please run "xdg-mime default nxmhandler.desktop x-scheme-handler/nxm" manually.)").arg(strerror(error)));
  }
}
