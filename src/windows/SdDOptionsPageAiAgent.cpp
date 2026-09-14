#include "SdConfig.h"
#include "SdDOptionsPageAiAgent.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QRegularExpressionValidator>




SdDOptionsPageAiAgent::SdDOptionsPageAiAgent(QWidget *parent) :
  QWidget(parent)
  {
  setWindowTitle( tr("Ai Agent") );

  QSettings s;

  QFormLayout *form = new QFormLayout();
  form->addRow( tr("LLM Provider URL (OpenAI API)"), mAiUrl = new QLineEdit(s.value(SDK_AI_URL).toString()) );
  form->addRow( tr("API Access Key"),                mAiKey = new QLineEdit(s.value(SDK_AI_KEY).toString()) );
  form->addRow( tr("Exact name of LLM Model"),       mAiModel = new QLineEdit(s.value(SDK_AI_MODEL).toString()) );
  form->addRow( tr("Embedder model agent ip"),       mAiAgentIp = new QLineEdit(s.value(SDK_AI_AGENT_IP).toString()) );

  // Регулярное выражение для IP4
  QRegularExpression regex(
      "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
      "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
  );
  mAiAgentIp->setValidator( new QRegularExpressionValidator(regex, mAiAgentIp ) );

  // Assign unique names for the interactive help system
  mAiUrl->setObjectName("SdDOptionsPageAiAgent.mAiUrl");
  mAiKey->setObjectName("SdDOptionsPageAiAgent.mAiKey");
  mAiModel->setObjectName("SdDOptionsPageAiAgent.mAiModel");
  mAiAgentIp->setObjectName("SdDOptionsPageAiAgent.mAiAgentIp");

  }



void SdDOptionsPageAiAgent::accept()
  {
  //Apply current params
  QSettings s;
  s.setValue(SDK_AI_URL, mAiUrl->text() );
  s.setValue(SDK_AI_KEY, mAiKey->text() );
  s.setValue(SDK_AI_MODEL, mAiModel->text() );
  s.setValue(SDK_AI_AGENT_IP, mAiAgentIp->text() );
  }
