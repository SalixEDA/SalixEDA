#include "SdWGuiderRef.h"

#include "SdWGuiderRef.h"
#include <QPainter>
#include <QPen>

SdWGuiderRef::SdWGuiderRef(QWidget* parent)
  : QWidget(parent),
    mAnimation(new QVariantAnimation(this)),
    mCurrentScale(1.0)
  {

  // Ensure this overlay is a standalone window layer on top of parent
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_DeleteOnClose, false);

  // Setup the breathing/pulsing animation configuration
  mAnimation->setDuration(600);
  mAnimation->setStartValue(1.0);
  mAnimation->setKeyValueAt(0.5, 1.2); // Inflates outwards at peak
  mAnimation->setEndValue(1.0);
  mAnimation->setLoopCount(2); // Pulse twice before vanishing

  // Connect animation signals to target slots
  connect(mAnimation, &QVariantAnimation::valueChanged, this, &SdWGuiderRef::handleAnimationUpdate);
  connect(mAnimation, &QVariantAnimation::finished, this, &SdWGuiderRef::handleAnimationFinished);

  hide();
  }




void SdWGuiderRef::highlightWidget(const QString& objectName)
  {
  QWidget* mainWindow = parentWidget();
  if( !mainWindow ) {
    return;
    }

  // Find all child elements matching the unique objectName identifier
  QList<QWidget*> candidates = mainWindow->findChildren<QWidget*>(objectName);
  QWidget* targetWidget = nullptr;

  // Isolate and identify the active visible widget from the collection
  for( QWidget* widget : std::as_const(candidates) ) {
    if( widget->isVisible() && !widget->rect().isEmpty() ) {
      targetWidget = widget;
      break;
      }
    }

  // Abort if no visible target element matches the requested name
  if( !targetWidget ) {
    hide();
    return;
    }

  // Instantly synchronize overlay size and position with the main window snapshot
  setGeometry( mainWindow->geometry() );

  // Calculate target coordinates relative to the absolute top-level window layout
  QPoint globalPos = targetWidget->mapToGlobal(QPoint(0, 0));
  QPoint overlayGlobalPos = mainWindow->mapToGlobal(QPoint(0, 0));
  QPoint relativePos = globalPos - overlayGlobalPos;

  mTargetRect = QRect(relativePos, targetWidget->size());

  // Activate the view layer and trigger the visualization sequence
  show();
  raise();
  mAnimation->start();
  }





void SdWGuiderRef::paintEvent(QPaintEvent* event)
  {
  Q_UNUSED(event);
  if( mTargetRect.isEmpty() ) {
    return;
    }

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Calculate dynamic dimensions for the primitive inflation animation
  double widthDiff = mTargetRect.width() * (mCurrentScale - 1.0);
  double heightDiff = mTargetRect.height() * (mCurrentScale - 1.0);

  // Expand the framing border outwards evenly from the center point
  QRectF inflatedRect = QRectF(mTargetRect).adjusted(
                          -widthDiff / 2.0,
                          -heightDiff / 2.0,
                          widthDiff / 2.0,
                          heightDiff / 2.0
                          );

  // Draw a bright red, thick highlight contour around the targeted layout area
  QPen pen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter.setPen(pen);
  painter.drawRect(inflatedRect);
  }




void SdWGuiderRef::handleAnimationUpdate(const QVariant& value)
  {
  mCurrentScale = value.toDouble();
  update(); // Schedule repaint to visualize the scaled layout frame shift
  }




void SdWGuiderRef::handleAnimationFinished()
  {
  hide(); // Hide the translucent canvas to free performance resources
  }
