package main

import (
  "log"
  "os"
  "time"

  "fyne.io/fyne/v2"
  "fyne.io/fyne/v2/app"
  "fyne.io/fyne/v2/container"
  "fyne.io/fyne/v2/dialog"
  "fyne.io/fyne/v2/widget"
)

// ========== СТРУКТУРА ИНТЕРФЕЙСА ==========

// Константы индексов экранов
const (
  ScreenSetup = iota
  ScreenProgress
  ScreenComplete
  ScreenError
  ScreenUpdate
)

type InstallUI struct {
  app    fyne.App
  window fyne.Window
  texts  *Texts
  cfg    *Config
  stack  *fyne.Container  // Стек для переключения экранов

  // Экран установки
  setupScreen struct {
    view           fyne.CanvasObject
    pathLabel      *widget.Label
    selectFolderBtn *widget.Button
    cancelBtn      *widget.Button
    installBtn     *widget.Button
    infoText       *widget.Label
    pathRow        *fyne.Container
    mainContent    *fyne.Container
    buttonBox      *fyne.Container
    content        *fyne.Container
  }

  // Экран прогресса
  progressScreen struct {
    view             fyne.CanvasObject
    progressBar       *widget.ProgressBar
    percentLabel      *widget.Label
    stageLabel        *widget.Label
    downloadInfoLabel *widget.Label
    cancelBtn         *widget.Button
    container         *fyne.Container
  }

  // Экран завершения
  completeScreen struct {
    view           fyne.CanvasObject
    completeLabel *widget.Label
    finishBtn     *widget.Button
    container     *fyne.Container
  }

  // Экран ошибки
  errorScreen struct {
    view          fyne.CanvasObject
    titleLabel    *widget.Label
    messageLabel  *widget.Label
    closeBtn      *widget.Button
    container     *fyne.Container
  }

  // Экран запроса обновления
  updateScreen struct {
    view          fyne.CanvasObject
    titleLabel    *widget.Label
    messageLabel  *widget.Label
    cancelBtn     *widget.Button
    laterBtn      *widget.Button
    updateBtn     *widget.Button
    container     *fyne.Container
  }
}

func NewInstallUI(cfg *Config) *InstallUI {
  ui := &InstallUI{
    cfg: cfg,
  }

  // Выбор языка
  ui.texts = TransStringRu() // По умолчанию русский
  // Здесь будет логика выбора языка по системе

  // Создаем приложение
  ui.app = app.NewWithID("com.yourcompany.installer")
  ui.window = ui.app.NewWindow(ui.texts.AppTitle)
  ui.window.Resize(fyne.NewSize(600, 500))

  // Создаем все экраны
  ui.createSetupScreen()
  ui.createProgressScreen()
  ui.createCompleteScreen()
  ui.createErrorScreen()
  ui.createUpdateScreen()

  // Создаем стек и добавляем все экраны
  ui.stack = container.NewStack(
    ui.setupScreen.view,
    ui.progressScreen.view,
    ui.completeScreen.view,
    ui.errorScreen.view,
    ui.updateScreen.view,
  )

  ui.window.SetContent(ui.stack)

  return ui
}

// Метод для переключения экранов по индексу
func (ui *InstallUI) ShowScreen(index int) {
  fyne.Do(func() {
    for i, obj := range ui.stack.Objects {
      if i == index {
        obj.Show()
      } else {
        obj.Hide()
      }
    }
    ui.stack.Refresh()
  })
}





func (ui *InstallUI) createSetupScreen() {
  // Переменная для хранения выбранного пути
  selectedPath := ui.cfg.InstallPath
  ui.setupScreen.pathLabel = widget.NewLabel(selectedPath)

  // Кнопка выбора директории
  ui.setupScreen.selectFolderBtn = widget.NewButton(ui.texts.SetupBrowseBtn, func() {
    dialog.ShowFolderOpen(func(list fyne.ListableURI, err error) {
      if err != nil {
        dialog.ShowError(err, ui.window)
        return
      }
      if list == nil {
        return
      }
      selectedPath = list.Path()
      ui.setupScreen.pathLabel.SetText(selectedPath)
    }, ui.window)
  })

  // Поясняющий текст
  ui.setupScreen.infoText = widget.NewLabel(ui.texts.SetupInfo)
  ui.setupScreen.infoText.Wrapping = fyne.TextWrapWord

  // Контейнер для поля выбора пути
  ui.setupScreen.pathRow = container.NewBorder(
    nil, nil,
    widget.NewLabel(ui.texts.SetupFolderLabel),
    ui.setupScreen.selectFolderBtn,
    ui.setupScreen.pathLabel,
  )

  // Кнопки
  ui.setupScreen.cancelBtn = &widget.Button{
    Text: ui.texts.SetupExitBtn,
    OnTapped: func() {
      ui.app.Quit()
    },
  }

  ui.setupScreen.installBtn = &widget.Button{
    Text:       ui.texts.SetupInstallBtn,
    Importance: widget.HighImportance,
    OnTapped: func() {
      log.Printf("Начинаем установку в: %s", selectedPath)
      ui.cfg.InstallPath = selectedPath
      go performInstallation(ui, ui.cfg, []string{})
    },
  }

  // Основной контент
  ui.setupScreen.mainContent = container.NewVBox(
    ui.setupScreen.infoText,
    widget.NewSeparator(),
    ui.setupScreen.pathRow,
    widget.NewLabel(""),
  )

  // Контейнер для кнопок
  ui.setupScreen.buttonBox = container.NewVBox(
    widget.NewSeparator(),
    container.NewHBox(
      widget.NewLabel(""),
      ui.setupScreen.cancelBtn,
      widget.NewLabel("  "),
      ui.setupScreen.installBtn,
    ),
    widget.NewLabel(""),
  )

  // BorderLayout: кнопки внизу, основной контент заполняет остальное
  ui.setupScreen.content = container.NewBorder(
    nil,
    ui.setupScreen.buttonBox,
    nil,
    nil,
    ui.setupScreen.mainContent,
  )

  // В конце сохраняем view
  ui.setupScreen.view = ui.setupScreen.content
}






func (ui *InstallUI) createProgressScreen() {
  // Прогресс-бар
  ui.progressScreen.progressBar = widget.NewProgressBar()
  ui.progressScreen.progressBar.Min = 0
  ui.progressScreen.progressBar.Max = 100

  // Проценты
  ui.progressScreen.percentLabel = widget.NewLabel("0%")
  ui.progressScreen.percentLabel.Alignment = fyne.TextAlignCenter

  // Этап
  ui.progressScreen.stageLabel = widget.NewLabel(ui.texts.ProgressPrepare)
  ui.progressScreen.stageLabel.Alignment = fyne.TextAlignCenter

  // Информация о загрузке
  ui.progressScreen.downloadInfoLabel = widget.NewLabel("")
  ui.progressScreen.downloadInfoLabel.Alignment = fyne.TextAlignCenter

  // Кнопка прерывания
  ui.progressScreen.cancelBtn = &widget.Button{
    Text:       ui.texts.ProgressCancelBtn,
    Importance: widget.LowImportance,
    OnTapped: func() {
      ui.window.Close()
    },
  }

  // Собираем контейнер
  ui.progressScreen.container = container.NewVBox(
    widget.NewLabel(""),
    ui.progressScreen.percentLabel,
    ui.progressScreen.progressBar,
    ui.progressScreen.stageLabel,
    ui.progressScreen.downloadInfoLabel,
    widget.NewLabel(""),
    container.NewHBox(
      widget.NewLabel(""),
      ui.progressScreen.cancelBtn,
    ),
  )

  // В конце сохраняем view
  ui.progressScreen.view = ui.progressScreen.container
}





func (ui *InstallUI) createCompleteScreen() {
  // Финальное сообщение
  ui.completeScreen.completeLabel = widget.NewLabel(ui.texts.CompleteMessage)
  ui.completeScreen.completeLabel.TextStyle = fyne.TextStyle{Bold: true}
  ui.completeScreen.completeLabel.Alignment = fyne.TextAlignCenter

  // Кнопка завершения
  ui.completeScreen.finishBtn = &widget.Button{
    Text:       ui.texts.CompleteFinishBtn,
    Importance: widget.HighImportance,
    OnTapped: func() {
      ui.window.Close()
    },
  }

  // Собираем контейнер
  ui.completeScreen.container = container.NewVBox(
    widget.NewLabel(""),
    ui.completeScreen.completeLabel,
    widget.NewLabel(""),
    container.NewHBox(
      widget.NewLabel(""),
      ui.completeScreen.finishBtn,
    ),
  )

  // В конце сохраняем view
  ui.completeScreen.view = ui.completeScreen.container
}





func (ui *InstallUI) createErrorScreen() {
  // Заголовок
  ui.errorScreen.titleLabel = widget.NewLabel(ui.texts.ErrorTitle)
  ui.errorScreen.titleLabel.TextStyle = fyne.TextStyle{Bold: true}
  ui.errorScreen.titleLabel.Alignment = fyne.TextAlignCenter

  // Сообщение об ошибке
  ui.errorScreen.messageLabel = widget.NewLabel("")
  ui.errorScreen.messageLabel.Wrapping = fyne.TextWrapWord
  ui.errorScreen.messageLabel.Alignment = fyne.TextAlignCenter

  // Кнопка закрытия
  ui.errorScreen.closeBtn = &widget.Button{
    Text: ui.texts.ErrorCloseBtn,
    OnTapped: func() {
      os.Exit(1)
    },
  }

  // Собираем контейнер
  ui.errorScreen.container = container.NewVBox(
    widget.NewLabel(""),
    ui.errorScreen.titleLabel,
    widget.NewLabel(""),
    ui.errorScreen.messageLabel,
    widget.NewLabel(""),
    container.NewHBox(
      widget.NewLabel(""),
      ui.errorScreen.closeBtn,
    ),
  )

  // В конце сохраняем view
  ui.errorScreen.view = ui.errorScreen.container
}





func (ui *InstallUI) createUpdateScreen() {
  // Заголовок
  ui.updateScreen.titleLabel = widget.NewLabel(ui.texts.UpdateTitle)
  ui.updateScreen.titleLabel.TextStyle = fyne.TextStyle{Bold: true}
  ui.updateScreen.titleLabel.Alignment = fyne.TextAlignCenter

  // Сообщение
  ui.updateScreen.messageLabel = widget.NewLabel(ui.texts.UpdateMessage)
  ui.updateScreen.messageLabel.Wrapping = fyne.TextWrapWord
  ui.updateScreen.messageLabel.Alignment = fyne.TextAlignCenter

  // Кнопки
  ui.updateScreen.cancelBtn = &widget.Button{
    Text: ui.texts.UpdateCancelBtn,
    OnTapped: func() {
      os.Exit(0)
    },
  }

  ui.updateScreen.laterBtn = &widget.Button{
    Text: ui.texts.UpdateLaterBtn,
    OnTapped: func() {
      // Прибавляем день к времени последней проверки
      lastCheck, _ := time.Parse(time.RFC3339, ui.cfg.LastCheck)
      newLastCheck := lastCheck.AddDate(0, 0, ui.cfg.CheckPeriodDays+1)
      ui.cfg.LastCheck = newLastCheck.Format(time.RFC3339)
      ui.cfg.Save()
      ui.window.Close()
    },
  }

  ui.updateScreen.updateBtn = &widget.Button{
    Text:       ui.texts.UpdateNowBtn,
    Importance: widget.HighImportance,
    OnTapped: func() {
      ui.ShowScreen( ScreenProgress )
      go performInstallation(ui, ui.cfg, ZipFiles) // Обновляем все файлы
    },
  }

  // Собираем контейнер
  ui.updateScreen.container = container.NewVBox(
    widget.NewLabel(""),
    ui.updateScreen.titleLabel,
    widget.NewLabel(""),
    ui.updateScreen.messageLabel,
    widget.NewLabel(""),
    container.NewHBox(
      widget.NewLabel(""),
      ui.updateScreen.cancelBtn,
      widget.NewLabel("  "),
      ui.updateScreen.laterBtn,
      widget.NewLabel("  "),
      ui.updateScreen.updateBtn,
    ),
  )

  // В конце сохраняем view
  ui.updateScreen.view = ui.updateScreen.container
}

