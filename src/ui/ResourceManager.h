#ifndef UI_RESOURCEMANAGER_H_
#define UI_RESOURCEMANAGER_H_

/*!
 * \file ResourceManager.h
 * \brief A class that manages resources such as icons
 * \author aging-team
 */

#include <QDir>
#include <QIcon>
#include <QStandardPaths>

class ResourceManager {
  /*!
   * \class ResourceManager
   * \brief A class that manages resources such as icons
   *
   *  The goal of this class is to make resource management easier for the
   *  rest of the program: it searches for the resource directory and pick the
   *  one that fits best (`/usr/share` if it's an installed program, `../res`
   *  if it's a debug build).
   */
public:
  /*!
   *  \brief Constructor
   *
   *  Base constructor of ResourceManager
   */
  ResourceManager();
  /*!
   *  \brief Constructor
   *
   *  Constructor of ResourceManager
   *
   *  \param isDarkTheme : boolean that describe the set of resources we use
   */
  explicit ResourceManager(bool isDarkTheme);
  /*!
   *  \brief QIcon getter
   *
   *  Returns the QIcon of the corresponding icon
   *
   *  Example usage:
   *  @code
   *  ResourceManager resources(true);
   *  QIcon playIcon = resources.getIcon("play");
   *  @endcode
   *
   *  \param name : name of the icon
   */
  QIcon getIcon(QString name);
  /*!
   *  \brief Icon's path getter
   *
   *  Returns the path of the corresponding icon
   *
   *  Example usage:
   *  @code
   *  ResourceManager resources(true);
   *  QString playIconPath = resources.getIconPath("play");
   *  // playIconPath can be equal to ../res/icons/dark/play.svg
   *  @endcode
   *
   *  \param name : name of the icon
   */
  QString getIconPath(QString name);

private:
  bool isDarkTheme;
  QDir icon_dir;
};

#endif // UI_RESOURCEMANAGER_H_
