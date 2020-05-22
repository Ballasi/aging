#include <ui/ResourceManager.h>

ResourceManager::ResourceManager() : isDarkTheme(false) {}

ResourceManager::ResourceManager(bool isDarkTheme) {
  this->isDarkTheme = isDarkTheme;

  QString relative_path("../res");

  QString data_path("/usr/share/aging");
  QDir absolute_dir(data_path);

  if (!absolute_dir.exists()) {
    QDir relative_dir(relative_path);
    if (relative_dir.exists())
      data_path = relative_path;
    else
      qDebug("Icon folder not available.");
  }

  icon_dir = QDir(data_path);
  icon_dir.cd("icons");

  if (isDarkTheme)
    icon_dir.cd("dark");
  else
    icon_dir.cd("light");
}

QIcon ResourceManager::getIcon(QString name) {
  return QIcon(icon_dir.absoluteFilePath(name + ".svg"));
}

QString ResourceManager::getIconPath(QString name) {
  return icon_dir.absoluteFilePath(name + ".svg");
}
