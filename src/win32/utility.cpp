#include <QCoreApplication>
#include <QString>

using namespace Qt::StringLiterals;

QString getHandlerRegPath(const QString& schema)
{
  return "HKEY_CURRENT_USER\\Software\\Classes\\"_L1 % schema % '\\';
}

QString getGlobalHandlerRegPath()
{
  return u"HKEY_CLASSES_ROOT\\nxm\\"_s;
}

QString getApplicationFilePath()
{
  return QCoreApplication::applicationFilePath();
}

QString getApplicationDirPath()
{
  return QCoreApplication::applicationDirPath();
}

bool isNxmHandlerExecutable(const QString& str)
{
  return str.endsWith("nxmhandler.exe"_L1, Qt::CaseInsensitive);
}
