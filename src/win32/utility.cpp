#include <QString>

using namespace Qt::StringLiterals;

QString getHandlerRegPath()
{
  return "HKEY_CURRENT_USER\\Software\\Classes\\nxm\\";
}

QString getGlobalHandlerRegPath()
{
  return "HKEY_CLASSES_ROOT\\nxm\\";
}

QString getApplicationFilePath()
{
  return  QCoreApplication::applicationFilePath();
}

QString getApplicationDirPath()
{
  return QCoreApplication::applicationDirPath();
}

bool isNxmHandlerExecutable(const QString& str) {
  return str.endsWith("nxmhandler.exe"_L1, Qt::CaseInsensitive);
}
