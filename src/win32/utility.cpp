#include <QString>

QString getHandlerRegPath()
{
  return "HKEY_CURRENT_USER\\Software\\Classes\\nxm\\";
}

QString getGlobalHandlerRegPath()
{
  return "HKEY_CLASSES_ROOT\\nxm\\";
}