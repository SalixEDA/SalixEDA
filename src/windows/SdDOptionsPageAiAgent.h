/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  AI Agent
*/
#ifndef SDDOPTIONSPAGEAIAGENT_H
#define SDDOPTIONSPAGEAIAGENT_H


#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>

class SdDOptionsPageAiAgent : public QWidget
  {
    Q_OBJECT
    QLineEdit *mAiUrl;     //!< URL to OpenAI API provider to LLM
    QLineEdit *mAiKey;     //!< Key to provider access
    QLineEdit *mAiModel;   //!< Exact name of LLM Model
    QLineEdit *mAiAgentIp; //!< Embedder model agent ip

  public:
    explicit SdDOptionsPageAiAgent(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGEAIAGENT_H
