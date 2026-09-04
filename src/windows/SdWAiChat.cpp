#include "SdWAiChat.h"
#include "ai/SdAiGateway.h"

#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextBlock>
#include <QVariantAnimation>
#include <QTimer>
#include <QApplication>
#include <QMainWindow>


SdWAiChat::SdWAiChat(QWidget *parent)
  : QWidget(parent, Qt::Window | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
//  : QWidget(parent, Qt::Window | Qt::WindowStaysOnTopHint) // Set as top-level window staying above modal dialogs
  , mChatLog(nullptr)
  , mInputFields(nullptr)
  , mSendButton(nullptr)
  {
  setupUi();

  // Initialize the 10-second delay timer
  mFadeDelayTimer = new QTimer(this);
  mFadeDelayTimer->setSingleShot(true);
  mFadeDelayTimer->setInterval(10000); // 10 seconds

  // Connect timer and animation signals
  connect(mFadeDelayTimer, &QTimer::timeout, this, &SdWAiChat::onFadeTimeout);

  // Install the event filter to monitor focus shifts on the window and its inputs
  installEventFilter(this);
  mInputFields->installEventFilter(this);
  mChatLog->installEventFilter(this);

  connect( this, &SdWAiChat::question, SdAiGateway::instance(), &SdAiGateway::question, Qt::QueuedConnection );
  connect( SdAiGateway::instance(), &SdAiGateway::answer, this, &SdWAiChat::appendAiMessage, Qt::QueuedConnection );
  }



SdWAiChat* SdWAiChat::instance()
  {
  static SdWAiChat *sInstance = nullptr;
  if (!sInstance)
    {
    sInstance = new SdWAiChat();
    }
  return sInstance;
  }



void SdWAiChat::closeEvent(QCloseEvent *event)
  {
  // Intercept the close action, ignore the destruction, and simply hide the UI
  event->ignore();
  this->hide();
  }




void SdWAiChat::showEvent(QShowEvent *event)
  {
  QWidget::showEvent(event);

  // Make sure we clear any running animations or opacity distortions upon appearing
  abortFadingAndRestore();

  // Find the active main window of the application to position against it
  QWidget *mainWindow = nullptr;
  for( QWidget *widget : QApplication::topLevelWidgets() ) {
    if( qobject_cast<QMainWindow*>(widget) && widget->isVisible() ) {
      mainWindow = widget;
      break;
      }
    }

  // Fallback to active window if no QMainWindow was explicitly discovered
  if( !mainWindow )
    mainWindow = QApplication::activeWindow();

  if( mainWindow ) {
    // Calculate coordinates: bottom-left of main window, lifted up by 32 pixels
    QPoint mainBotLeft = mainWindow->geometry().bottomLeft();
    int targetX = mainBotLeft.x();
    int targetY = mainBotLeft.y() - height() - 32;

    move(targetX, targetY);
    }
  }





bool SdWAiChat::eventFilter(QObject *watched, QEvent *event)
  {
  // Detect when focus enters this window or any of its internal focusable widgets
  if( event->type() == QEvent::FocusIn )
    abortFadingAndRestore();

  // Detect when focus leaves
  else if( event->type() == QEvent::FocusOut ) {
    // Check if the newly focused widget is still a part of our chat window hierarchy
    QWidget *focusedWidget = QApplication::focusWidget();
    if( !focusedWidget || (focusedWidget != this && !isAncestorOf(focusedWidget)) ) {
      // Focus has completely exited the chat system scope; start the countdown
      if( !mFadeDelayTimer->isActive() ) {
        mFadeDelayTimer->start();
        }
      }
    }

  return QWidget::eventFilter(watched, event);
  }



void SdWAiChat::setupUi()
  {
  // Configure the window properties
  this->setWindowTitle( tr("AI Assistant") );
  this->resize(450, 600); // More vertical aspect ratio suitable for chat overlays

  // Create the main vertical layout directly for this widget
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(10, 10, 10, 10);
  mainLayout->setSpacing(10);

  // Initialize the chat log area
  mChatLog = new QTextBrowser(this);

  // Crucial settings for link interaction:
  mChatLog->setOpenExternalLinks(false); // Stop Qt from opening links in a desktop browser
  mChatLog->setOpenLinks(false);         // Prevent internal navigation jumps

  mainLayout->addWidget(mChatLog, 1);

  // Create a horizontal layout for the input bar at the bottom
  QHBoxLayout *inputLayout = new QHBoxLayout();
  inputLayout->setSpacing(8);

  // Initialize the multi-line input field
  mInputFields = new QTextEdit(this);
  mInputFields->setPlaceholderText("Ask me anything...");
  mInputFields->setMaximumHeight(80);
  inputLayout->addWidget(mInputFields, 1);

  // Initialize the submission button
  mSendButton = new QPushButton("Send", this);
  mSendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  inputLayout->addWidget(mSendButton);

  // Integrate the input bar into the main layout
  mainLayout->addLayout(inputLayout);

  // Establish signal-slot connections
  connect(mSendButton, &QPushButton::clicked, this, &SdWAiChat::onSendButtonClicked);
  connect(mChatLog, &QTextBrowser::anchorClicked, this, &SdWAiChat::onAnchorClicked);
  }





void SdWAiChat::onSendButtonClicked()
  {
  QString queryText = mInputFields->toPlainText().trimmed();
  if( queryText.isEmpty() ) return;

  if( mDialog.size() & 1 )
    mDialog.append( mAnswer );

  mDialog.append( queryText );

  appendUserMessage( queryText );
  mInputFields->clear();

  emit question( QString{}, mDialog );
  }








void SdWAiChat::appendUserMessage(const QString &msg)
  {
  // 1. Создаем временный документ и загружаем Markdown
  QTextDocument tempDoc;
  tempDoc.setPlainText(msg);

  // 4. Теперь безопасно оборачиваем чистый фрагмент в единый визуальный контейнер <div>
  QString finalMessageHtml = QString(
                               "<table width=\"100%\">"
                               "  <tr>"
                               "    <td width=\"5%\"></td>"
                               "    <td bgcolor=\"#D1E8FF\" style=\"padding: 8px;\">"
                               "      <b>%1:</b><br>%2"
                               "    </td>"
                               "  </tr>"
                               "</table>").arg( tr("You"), extractBody(tempDoc.toHtml()) ) ;

  // 5. Добавляем монолитный блок в лог
  mChatLog->append(finalMessageHtml);
  }



#if 0
void SdWAiChat::appendAiMessage(const QString &msg)
  {
  // 1. Создаем временный документ и загружаем Markdown
  QTextDocument tempDoc;
  tempDoc.setMarkdown(msg);

  // 4. Теперь безопасно оборачиваем чистый фрагмент в единый визуальный контейнер <div>
  QString finalMessageHtml = QString(
      "<table width=\"100%\">"
      "  <tr>"
//      "    <td bgcolor=\"#EAEAEA\" style=\"padding: 8px;\">"
      "    <td style=\"padding: 8px;\">"
      "      <b>%1:</b><br>%2"
      "    </td>"
      "    <td width=\"5%\"></td>"
      "  </tr>"
      "</table>").arg( tr("System"), extractBody(tempDoc.toHtml()) ) ;

  // 5. Добавляем монолитный блок в лог
  mChatLog->append(finalMessageHtml);
  }
#endif

void SdWAiChat::appendAiMessage(int answerId, const QString &answer) {
  bool isNext = answerId == mAnswerId;
  //Prepare final message
  if( isNext ) {
    // If the token is empty, generation for this ID has completed naturally
    if(answer.isEmpty())
      return;
    mAnswer.append( answer );
    }
  else {
    mAnswer = answer;
    mAnswerId = answerId;
    }

  // Convert Markdown source to raw HTML layout
  QTextDocument tempDoc;
  tempDoc.setMarkdown(mAnswer);

  QString body = extractBody(tempDoc.toHtml());

  // Wrap with boundary markers using the current response transaction ID
  QString finalMessageHtml = QString(
                               "<table width=\"100%\">"
                               "  <tr>"
                               "    <td style=\"padding: 8px;\">"
                               "      <b>%1:</b><br>%2"
                               "    </td>"
                               "    <td width=\"5%\"></td>"
                               "  </tr>"
                               "</table>")
                             .arg( tr("System"), body);

  // Check if this is a completely new response sequence
  if( isNext ) {
    auto childList = mChatLog->document()->rootFrame()->childFrames();
    QTextFrame *lastFrame = childList.last();
    // 1. Получаем позиции начала и конца из фрейма таблицы
    int startPos = childList.at(childList.count() - 2)->lastPosition() + 1;
    int endPos = lastFrame->lastPosition() + 1;

    // 2. Создаем курсор, привязанный к документу лога
    QTextCursor cursor(mChatLog->document());

    // 3. Перемещаем курсор в начальную позицию фрейма
    cursor.setPosition(startPos);

    // 4. Выделяем фрагмент до конечной позиции фрейма
    // Флаг QTextCursor::KeepAnchor указывает, что нужно именно выделить текст (вытащить якорь)
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);

    // 5. Удаляем выделенный фрейм вместе со всей его служебной HTML-структурой
    cursor.removeSelectedText();

    }
  // Append the clean atomic chunk down into the log interface
  mChatLog->append(finalMessageHtml);
  }





QString SdWAiChat::extractBody(const QString &fullHtml)
  {
  // 2. Чисто вырезаем содержимое между <body> и </body>
  // Это гарантирует отсутствие глобальных тегов <html> и <style>, ломающих верстку
  int bodyStart = fullHtml.indexOf("<body");
  if( bodyStart != -1 ) {
    // Находим закрывающую угловую скобку тега <body ...>
    bodyStart = fullHtml.indexOf('>', bodyStart) + 1;
    int bodyEnd = fullHtml.indexOf("</body>", bodyStart);

    if( bodyEnd != -1 )
      return fullHtml.mid(bodyStart, bodyEnd - bodyStart).trimmed();
    }

  return QString{};
  }



void SdWAiChat::onAnchorClicked(const QUrl &url)
  {
  // Intercept the link path/text and push it directly into the chat's text input bar
  QString linkText = url.toString();
  }




void SdWAiChat::onFadeTimeout()
  {
  mFadeDelayTimer->stop();
  hide();
  }





void SdWAiChat::abortFadingAndRestore()
  {
  // Cease all active countdown configurations and force complete window visibility
  mFadeDelayTimer->stop();
  }

