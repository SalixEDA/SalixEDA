/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#include "SdGuiderPlayer.h"

SdGuiderPlayer::SdGuiderPlayer(QWidget *parent) : QWidget(parent)
  {

  }

void SdGuiderPlayer::onTime()
  {

  }

void SdGuiderPlayer::play(const QString &fname)
  {
  Q_UNUSED(fname)
  }


void SdGuiderPlayer::paintEvent(QPaintEvent *event)
  {
  Q_UNUSED(event)
  }
