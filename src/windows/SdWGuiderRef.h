/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdWGuidorRef Overlay subsystem for dynamic UI control highlighting and user guidance.

  This class creates a transparent, click-through, top-level window layer aligned
  perfectly with the application's main window geometry. It listens for highlight
  requests containing a specific object name, dynamically searches the widget
  hierarchy to isolate the active visible target, and renders an animated pulsing
  contour around it. To optimize system composition and rendering performance,
  the overlay automatically hides itself immediately after the animation sequence finishes.
*/
#ifndef SDWGUIDERREF_H
#define SDWGUIDERREF_H

#include <QWidget>
#include <QVariantAnimation>
#include <QRect>

//! \brief Transparent overlay window for highlighting specific UI controls.
class SdWGuiderRef : public QWidget
  {
    Q_OBJECT

    QVariantAnimation* mAnimation;     //!< Animation controller for the pulsing effect.
    QRect              mTargetRect;    //!< Screen boundaries of the currently highlighted widget.
    double             mCurrentScale;  //!< Current inflation scale value from the animation.
  public:
    /// @brief
    /// @param parent

    //!
    //! \brief SdWGuiderRef Constructor initializing the overlay over the main window.
    //! \param parent       Pointer to the main application window.
    //!
    explicit SdWGuiderRef(QWidget* parent);

    ~SdWGuiderRef() override = default;

    //!
    //! \brief highlightWidget Starts the highlight animation for a widget with the given object name.
    //! \param objectName      Unique name of the target widget to highlight.
    //!
    void highlightWidget(const QString& objectName);

  protected:
    //!
    //! \brief paintEvent Paints the animated pulsing highlight contour.
    //! \param event      Paint event data.
    //!
    virtual void paintEvent(QPaintEvent* event) override;

  private slots:
    //!
    //! \brief handleAnimationUpdate Handles updates from the animation timeline.
    //! \param value                 Current animation step value (scale/inflation factor).
    //!
    void handleAnimationUpdate(const QVariant& value);

    //!
    //! \brief handleAnimationFinished Hides the overlay screen once the animation finishes.
    //!
    void handleAnimationFinished();
  };

#endif // SDWGUIDERREF_H
