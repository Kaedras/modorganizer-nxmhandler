#include <QString>
#include <QStandardPaths>

QString getHandlerRegPath()
{
  return QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() + "/nxmhandler.desktop";
}

QString getGlobalHandlerRegPath()
{
  return getHandlerRegPath();
}