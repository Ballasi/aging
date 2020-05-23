#ifndef LOGGER_H
#define LOGGER_H

/*!
 * \file Logger.h
 * \brief File logging utilities
 */

#include <QObject>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

/*!
 * \class Logger
 * \brief Main logger class
 */
class Logger : public QObject {
    Q_OBJECT

public:
  /*!
   * \brief Constructor
   *
   * \param parent: Parent for Qt interaction 
   * \param fileName: name of file to log to
   * \param append: appends to file
   */
  explicit Logger(QObject *parent, QString fileName, bool append);

  /*!
   * \brief Destructor
   *
   * Closes logging file
   */
  ~Logger();

  /*!
   * \brief Toggle date in log
   *
   * Defaults to true
   */
  void setShowDateTime(bool value);

private:
  QFile *file;
  bool m_showDate;

public slots:
  /*!
   * \brief writes to the log file
   */
  void write(const QString &value);
};

#endif // LOGGER_H
