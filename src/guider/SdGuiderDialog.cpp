/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderDialog A dialog widget for displaying and navigating through a scenario structure.

  This class presents a hierarchical view of scenes and steps using a QTreeWidget.
  Scenes (top-level items) are selectable, while steps (child items) are view-only
  and cannot be selected directly. The dialog maintains an always-expanded state
  for all items. It provides navigation methods to track current scene and step
  indices, move to the next step, and handle scene selection events. The dialog
  emits a signal when a scene is selected and includes a build button for
  triggering scenario execution.
*/

#include "SdGuiderDialog.h"
#include "SvLib/SvDir.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QCryptographicHash>

QString SdGuiderScena::hash() const {
  QCryptographicHash hasher( QCryptographicHash::Blake2s_128 );

  // Добавляем название сцены в хэш
  hasher.addData( mTitle.toUtf8() );

  // Добавляем количество шагов для контроля структуры
  hasher.addData( QByteArray::number( mSteps.size() ) );

  // Добавляем каждый шаг в хэш
  for( const auto &step : std::as_const(mSteps) ) {
    // Добавляем текст шага
    hasher.addData( step.mTiter.toUtf8() );

    // Добавляем длительность шага
    hasher.addData( QByteArray::number( step.mDuration ) );

    // Добавляем разделитель для надежности (чтобы избежать коллизий
    // при конкатенации, например "ab"+"c" vs "a"+"bc")
    hasher.addData( "|" );
    }

  // Возвращаем результат в виде hex-строки
  return QString::fromLatin1( hasher.result().toHex() );
  }


SdGuiderDialog::SdGuiderDialog(SdWMain *wmain )
  : QDialog( nullptr )
  , mWMain(wmain) {
  // Устанавливаем флаг автоматического удаления при закрытии
  //setAttribute( Qt::WA_DeleteOnClose );

  // Создаем основной layout
  QVBoxLayout *mainLayout = new QVBoxLayout( this );

  // Создаем и настраиваем QLabel для подсказки
  mHintLabel = new QLabel( this );
  mHintLabel->setWordWrap( true );
  mHintLabel->setStyleSheet( "QLabel { background-color: #f0f0f0; padding: 5px; }" );
  mainLayout->addWidget( mHintLabel );

  // Создаем и настраиваем QTreeWidget
  mTreeWidget = new QTreeWidget( this );
  mTreeWidget->setHeaderLabel( tr("Script") );
  mTreeWidget->setAlternatingRowColors( true );
  mTreeWidget->setIndentation( 20 );

  // Запрет сворачивания (все элементы всегда развернуты)
  mTreeWidget->setItemsExpandable( false );
  mTreeWidget->setExpandsOnDoubleClick( false );

  // Разрешаем выбор только сцен (строк верхнего уровня)
  mTreeWidget->setSelectionMode( QAbstractItemView::SingleSelection );

  // Настройка внешнего вида
  QHeaderView *header = mTreeWidget->header();
  header->setStretchLastSection( true );
  header->setSectionResizeMode( QHeaderView::ResizeToContents );

  mainLayout->addWidget( mTreeWidget );

  // Создаем кнопку Build
  mBuildButton = new QPushButton( tr("Build"), this );
  mBuildButton->setMinimumHeight( 30 );
  mainLayout->addWidget( mBuildButton );

  // Настройка свойств диалога
  setWindowTitle( tr("The script") );
  setMinimumSize( 800, 300 );

  // Подключаем сигналы
  connect( mTreeWidget, &QTreeWidget::itemClicked, this, &SdGuiderDialog::onItemClicked );
  connect( mBuildButton, &QPushButton::clicked, this, &SdGuiderDialog::onBuildClicked );
  }








bool SdGuiderDialog::setScenaFile(const QString &fname)
  {
  QFileInfo info(fname);
  SvDir dir(info.absolutePath());
  mScriptPath = dir.slashedPath();

  QFile file(fname);
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
    qDebug() << "Cannot open file:" << fname;
    return false;
    }

  QTextStream in(&file);
  QString currentLine;

  SdGuiderScena currentScena;
  SdGuiderStep currentStep;
  QString currentStepText;
  bool inStep = false;

  mScenaList.clear();

  while( !in.atEnd() ) {
    currentLine = in.readLine();

    // Убираем лишние пробелы в начале и конце, но сохраняем внутренние
    QString trimmedLine = currentLine.trimmed();

    // Пропускаем пустые строки
    if( trimmedLine.isEmpty() )
      continue;

    // Проверяем начало строки
    if( trimmedLine.startsWith("--") ) {
      // Это шаг (начинается с двух тире)

      // Сохраняем предыдущий шаг, если он был
      if( inStep ) {
        currentStep.mTiter = currentStepText.trimmed();
        currentScena.mSteps.append(currentStep);
        currentStepText.clear();
        }

      // Убираем два тире в начале
      QString stepContent = trimmedLine.mid(2).trimmed();

      // Создаем новый шаг
      currentStep = SdGuiderStep();
      currentStep.mDuration = 3; // Значение по умолчанию

      // Проверяем, есть ли число после тире (длительность)
      int spacePos = stepContent.indexOf(' ');
      if (spacePos > 0) {
        // Пробуем распарсить число в начале
        QString firstPart = stepContent.left(spacePos);
        bool ok;
        int duration = firstPart.toInt(&ok);
        if (ok) {
          // Нашли число - это длительность
          currentStep.mDuration = duration;
          // Остаток строки - текст шага
          currentStepText = stepContent.mid(spacePos + 1);
          }
        else {
          // Нет числа - вся строка текст шага
          currentStepText = stepContent;
          }
        }
      else {
        // Нет пробела - возможно только число или только текст
        bool ok;
        int duration = stepContent.toInt(&ok);
        if (ok) {
          // Только число - это длительность без текста
          currentStep.mDuration = duration;
          currentStepText = "";
          }
        else {
          // Только текст без длительности
          currentStepText = stepContent;
          }
        }

      inStep = true;
      }
    else if( trimmedLine.startsWith("-") ) {
      // Это название сцены (начинается с одного тире)

      // Сохраняем предыдущую сцену, если она не пустая
      if( !currentScena.mTitle.isEmpty() || !currentScena.mSteps.isEmpty() ) {
        // Сохраняем последний шаг текущей сцены
        if( inStep ) {
          currentStep.mTiter = currentStepText.trimmed();
          currentScena.mSteps.append(currentStep);
          currentStepText.clear();
          inStep = false;
          }
        mScenaList.append(currentScena);
        }

      // Начинаем новую сцену
      currentScena = SdGuiderScena();
      // Убираем один дефис в начале
      currentScena.mTitle = trimmedLine.mid(1).trimmed();
      currentScena.mSteps.clear();
      inStep = false;
      }
    else {
      // Это продолжение предыдущего шага (многострочный текст)
      if( inStep ) {
        if( !currentStepText.isEmpty() )
          currentStepText += " ";
        currentStepText += trimmedLine;
        }
      else {
        // Строка без маркера, но не в шаге - игнорируем или можно добавить в заголовок?
        // По спецификации таких строк быть не должно
        qDebug() << "Warning: Line without marker outside step:" << trimmedLine;
        }
      }
    }

  // Сохраняем последний шаг, если он был
  if( inStep ) {
    currentStep.mTiter = currentStepText.trimmed();
    currentScena.mSteps.append(currentStep);
    }

  // Сохраняем последнюю сцену
  if( !currentScena.mTitle.isEmpty() || !currentScena.mSteps.isEmpty() ) {
    mScenaList.append(currentScena);
    }

  file.close();

  mTreeWidget->clear();

  QTreeWidgetItem *scenaFirst = nullptr;

  // Заполняем дерево
  for( const auto &scena : std::as_const(mScenaList) ) {
    // Создаем элемент сцены (верхний уровень)
    QTreeWidgetItem *scenaItem = new QTreeWidgetItem( mTreeWidget );
    if( scenaFirst == nullptr ) scenaFirst = scenaItem;
    scenaItem->setText( 0, scena.mTitle );
    scenaItem->setData( 0, Qt::UserRole, "scena" );

    // Запрещаем сворачивание этого элемента
    scenaItem->setFlags( scenaItem->flags() & ~Qt::ItemIsUserCheckable );

    // Добавляем шаги
    for( const auto &step : std::as_const(scena.mSteps) ) {
      QTreeWidgetItem *stepItem = new QTreeWidgetItem( scenaItem );

      // Формируем текст шага с длительностью
      QString stepText = step.mTiter;
      if( !stepText.isEmpty() )
        stepText = QString( tr("[%1 sec] %2") ).arg( step.mDuration ).arg( step.mTiter );
      else
        stepText = QString( tr("[%1 sec]") ).arg( step.mDuration );

      stepItem->setText( 0, stepText );
      stepItem->setData( 0, Qt::UserRole, "step" );

      // Шаги нельзя выбирать
      stepItem->setFlags( stepItem->flags() & ~Qt::ItemIsSelectable );
      }
    }

  // Разворачиваем все элементы
  mTreeWidget->expandAll();

  // Обновляем подсказку
  mHintLabel->setText( "F12 - save snapshot" );

  //Устанавливаем активным первую сцену
  mScenaIndex = mStepIndex = 0;
  mTreeWidget->setCurrentItem( scenaFirst );

  return true;
  }



int SdGuiderDialog::snapshotIndex() const
  {
  int scena = getCurrentScenaIndex();
  if( scena >= 0 ) {
    int step = getCurrentStepIndex();
    if( step == 0 ) return scena;
    return scena + 1;
    }
  return -1;
  }





void SdGuiderDialog::goToNextStep() {
  if( !mCurrentStepItem )
    return;

  QTreeWidgetItem *parent = mCurrentStepItem->parent();
  if( !parent )
    return;

  int currentIndex = parent->indexOfChild( mCurrentStepItem );
  int nextIndex = currentIndex + 1;

  // Снимаем выделение с текущего шага
  if( mCurrentStepItem ) {
    mCurrentStepItem->setSelected( false );
    mCurrentStepItem->setBackground( 0, QBrush() ); // Сбрасываем фон
    }

  if( nextIndex < parent->childCount() ) {
    // Переходим к следующему шагу
    mCurrentStepItem = parent->child( nextIndex );
    mCurrentStepItem->setSelected( true );

    // Визуально выделяем текущий шаг
    mCurrentStepItem->setBackground( 0, QBrush( QColor( 200, 230, 255 ) ) );

    // Прокручиваем до текущего шага
    mTreeWidget->scrollToItem( mCurrentStepItem );
    }
  else {
    // Это был последний шаг, снимаем выделение
    mCurrentStepItem = nullptr;
    clearSelection();
    }
  }





void SdGuiderDialog::clearSelection() {
  if( mCurrentStepItem ) {
    mCurrentStepItem->setBackground( 0, QBrush() );
    mCurrentStepItem = nullptr;
    }
  mTreeWidget->clearSelection();
  }





void SdGuiderDialog::onItemClicked( QTreeWidgetItem *item, int column ) {
  Q_UNUSED( column );

  if( !item ) return;

  // Проверяем, является ли элемент сценой
  if( item->data( 0, Qt::UserRole ).toString() == "scena" ) {
    // Находим индекс сцены
    int index = mTreeWidget->indexOfTopLevelItem( item );
    if( index >= 0 ) {
      // Снимаем выделение с шага при выборе новой сцены
      clearSelection();

      // Вызываем сигнал
      onScenaSelected( index );

      qDebug() << "Scene selected:" << index << item->text( 0 );
      }
    }
  else {
    // Если попытались выбрать шаг - снимаем выделение
    item->setSelected( false );
    }
  }



void SdGuiderDialog::onBuildClicked() {
  // Здесь можно добавить логику для кнопки Build
  qDebug() << "Build button clicked";

  // Например, начать выполнение сценария с текущей выбранной сцены
  int scenaIndex = getCurrentScenaIndex();
  if( scenaIndex >= 0 )
    qDebug() << "Building scena:" << scenaIndex;
  else
    qDebug() << "No scene selected";
  }




void SdGuiderDialog::onScenaSelected(int index)
  {
  mScenaIndex = index;
  emit snapshotLoad( mScriptPath, mScenaIndex );
  }
