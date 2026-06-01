#include "../handlerstorage.h"
#include "../utility.h"

#include <QDir>
#include <QFile>

void HandlerStorage::registerSchemaProxy(const QString& proxyPath,
                                         const QString& schema)
{
  QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\" + schema + "\\",
                     QSettings::NativeFormat);
  QString myExe =
      QString("\"%1\" ").arg(QDir::toNativeSeparators(proxyPath)).append("\"%1\"");
  settings.setValue("Default", "URL:" + schema.toUpper() + " Protocol");
  settings.setValue("URL Protocol", "");
  settings.setValue("shell/open/command/Default", myExe);
  settings.sync();
}
