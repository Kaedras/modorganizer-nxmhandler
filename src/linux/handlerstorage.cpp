#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

void HandlerStorage::registerProxy(const QString &proxyPath)
{
  QString path = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() % u"/nxmhandler.desktop"_s;

  QFile file(path);
  if (!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text))
  {
    QMessageBox::warning(nullptr, QObject::tr("Error registering handler"),
                         QObject::tr( "An error occurred while creating nxmhandler.desktop:\n"
                                     "%1\n").arg(file.errorString()));
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

  int status = system("xdg-mime default nxmhandler.desktop x-scheme-handler/nxm");
  if(status != 0) {
    QString message;
    switch (status)
    {
    case -1:
      {
        const int error = errno;
        message = strerror(error);
        break;
      }
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

    QMessageBox::warning(nullptr, QObject::tr("Error registering handler"),
                         QObject::tr( "An error occurred while registering nxmhandler:\n"
                                     "%1\n"
                                     R"(Please run "xdg-mime default nxmhandler.desktop x-scheme-handler/nxm" manually.)").arg(message));
  }
}
