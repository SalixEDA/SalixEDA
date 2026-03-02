/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Paths
*/
#ifndef SDDOPTIONSPAGEPATH_H
#define SDDOPTIONSPAGEPATH_H

#include <QWidget>
#include <QLineEdit>

class SdDOptionsPagePath : public QWidget
  {
    Q_OBJECT

    QLineEdit *mHomePath;             //Current user home path [Каталог пользователя]
    QLineEdit *mLibraryPath;          //Library path [Каталог библиотек]
    QLineEdit *mPatternPath;          //Каталог шаблонов
    QLineEdit *mCategoryPath;         //Base path for store category hierarchy
  public:
    explicit SdDOptionsPagePath(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGEPATH_H
