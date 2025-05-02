#include "../handlerstorage.h"
#include "../utility.h"

#include <QFile>
#include <QDir>

void HandlerStorage::registerProxy(const QString &proxyPath)
{
  QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\nxm\\", QSettings::NativeFormat);
  QString myExe = QString("\"%1\" ").arg(QDir::toNativeSeparators(proxyPath)).append("\"%1\"");
  settings.setValue("Default", "URL:NXM Protocol");
  settings.setValue("URL Protocol", "");
  settings.setValue("shell/open/command/Default", myExe);
  settings.sync();
}
