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
    QMessageBox::warning(nullptr, QObject::tr("Error registering handler"),
                         QObject::tr( "An error occurred while creating nxmhandler.desktop:\n"
                                     "%1\n").arg(file.errorString()));
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

  int status = system("xdg-mime default nxmhandler.desktop x-scheme-handler/nxm");
  if(status != 0) {
    QString message;
    switch (status)
    {
    case -1:
      const int error = errno;
      message = strerror(error);
      break;
    case 1:
      message = QStringLiteral("Error in command line syntax.");
      break;
    case 2:
      message = QStringLiteral("One of the files passed on the command line did not exist.");
      break;
    case 3:
      message = QStringLiteral("A required tool could not be found.");
      break;
    case 4:
      message = QStringLiteral("The action failed.");
      break;
    case 5:
      message = QStringLiteral("No permission to read one of the files passed on the command line.");
      break;
    default:
      message = QStringLiteral("Unknown error.");
      break;
    }

    QMessageBox::warning(nullptr, QObject::tr("Error registering handler"),
                         QObject::tr( "An error occurred while registering nxmhandler:\n"
                                     "%1\n"
                                     R"(Please run "xdg-mime default nxmhandler.desktop x-scheme-handler/nxm" manually.)").arg(message));
  }
}
