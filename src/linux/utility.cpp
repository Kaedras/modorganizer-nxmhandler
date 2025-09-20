#include <QString>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

QString getHandlerRegPath()
{
  return QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first() % "/nxmhandler.desktop"_L1;
}

QString getGlobalHandlerRegPath()
{
  return getHandlerRegPath();
}
