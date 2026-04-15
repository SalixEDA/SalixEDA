package main

import (
  //"log"
  "os"
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
  texts      *Texts
  cfg        *Config
  stack      *SgItemContainer  // Стек для переключения экранов
  messageBox *SgItemMessageBox //Модальное окно сообщений

  // Экран установки
  setupScreen struct {
    view           *SgItemPage
    pathLabel      *SgItemInputLine
    cancelBtn      *SgItemButton
    installBtn     *SgItemButton
    infoText       *SgItemTextMulty
  }

  // Экран прогресса
  progressScreen struct {
    view              *SgItemPage
    progressBar       *SgItemProgressBar
    stageLabel        *SgItemText
    downloadInfoLabel *SgItemText
    cancelBtn         *SgItemButton
  }

  // Экран завершения
  completeScreen struct {
    view          *SgItemPage
    completeLabel *SgItemTextMulty
    finishBtn     *SgItemButton
  }

  // Экран запроса обновления
  updateScreen struct {
    view          *SgItemPage
    titleLabel    *SgItemText
    messageLabel  *SgItemTextMulty
    cancelBtn     *SgItemButton
    laterBtn      *SgItemButton
    updateBtn     *SgItemButton
  }

  //Экран подтверждения удаления
  deleteScreen struct {
    view          *SgItemPage
    messageLabel  *SgItemTextMulty
    cancelBtn     *SgItemButton
    deleteBtn     *SgItemButton
  }

  //Экран прогресса удаления
  deleteProgress struct {
    view              *SgItemPage
    progressBar       *SgItemProgressBar
    stageLabel        *SgItemText
    cancelBtn         *SgItemButton
  }

}




func NewInstallUI(cfg *Config) *InstallUI {
  ui := &InstallUI{
    cfg: cfg,
  }

  // Выбор языка
  switch(language) {
    case "ru" :
      ui.texts = TransStringRu() // По умолчанию русский

    default :
      ui.texts = TransStringEn()
    }

  // Создаем стек и добавляем все экраны
  ui.stack = NewSgItemContainer( 0,0, 300, 300 )
  ui.stack.AnchorFillDef( SgScreen )
  SgScreen.Add( ui.stack )

  // Создаем все экраны
  ui.createSetupScreen()       //0
  ui.createProgressScreen()    //1
  ui.createCompleteScreen()    //2
  ui.createUpdateScreen()      //3
  ui.createDeleteScreen()      //4
  ui.createDeleteProgress()    //5

  //Создаем модальное окно сообщений
  ui.messageBox = NewSgItemMessageBox( 500, 200 )
  ui.messageBox.AnchorCenterIn( SgScreen, 0, 0 )
  SgScreen.Add( ui.messageBox )

  return ui
  }






func (ui *InstallUI) createSetupScreen() {
  ui.setupScreen.view = NewSgItemPage( 0xf0f0f0, ui.texts.SetupTitle, 30, 0x202020 )

  column := NewSgItemColumn( 0, 0, 10, 10, 10 )

  //Текст приглашения
  ui.setupScreen.infoText = NewSgItemTextMulty( 0, 0, 0, ui.texts.SetupInfo, 20, 0x202020 )
  ui.setupScreen.infoText.Align = AlignLeft | AlignTop
  ui.setupScreen.infoText.AnchorHorzFill( column, 5, 5 )
  column.Add( ui.setupScreen.infoText )

  // Переменная для хранения выбранного пути
  ui.setupScreen.pathLabel = NewSgItemInputLine( 0, 0, 100, 30 )
  ui.setupScreen.pathLabel.AnchorHorzFill( column, 5, 5 )
  ui.setupScreen.pathLabel.TextSet( ui.cfg.InstallPath )
  column.Add( ui.setupScreen.pathLabel )

  ui.setupScreen.view.CentralSet( column )

  // Кнопки
  ui.setupScreen.cancelBtn = NewSgItemButton ( 0, 0, 100, 30, ui.texts.SetupExitBtn )
  ui.setupScreen.cancelBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    os.Exit(0)
    }
  ui.setupScreen.view.AddFooter( ui.setupScreen.cancelBtn )

  ui.setupScreen.installBtn = NewSgItemButton ( 0, 0, 100, 30, ui.texts.SetupInstallBtn )
  ui.setupScreen.installBtn.OnClick = func(item *SgItem, localX int, localY int) {
    ui.cfg.InstallPath = ui.setupScreen.pathLabel.TextGet()
    go performInstallation(ui, ui.cfg, []string{})
    }
  ui.setupScreen.view.AddFooter( ui.setupScreen.installBtn )

  // В конце сохраняем view
  ui.stack.Add( ui.setupScreen.view )
  }






func (ui *InstallUI) createProgressScreen() {
  ui.progressScreen.view = NewSgItemPage( 0xf0f0f0, ui.texts.ProgressTitle, 30, 0x202020 )

  column := NewSgItemColumn( 0, 0, 10, 10, 10 )

  // Прогресс-бар
  ui.progressScreen.progressBar = NewSgItemProgressBar( 0, 0, 100, 30 )
  ui.progressScreen.progressBar.AnchorHorzFill( column, 5, 5 )
  //ui.progressScreen.progressBar.Min = 0
  //ui.progressScreen.progressBar.Max = 100
  column.Add( ui.progressScreen.progressBar )

  // Проценты
  //ui.progressScreen.percentLabel = widget.NewLabel("0%")
  //ui.progressScreen.percentLabel.Alignment = fyne.TextAlignCenter

  // Этап
  ui.progressScreen.stageLabel = NewSgItemText( 0, 0, ui.texts.ProgressPrepare, 20, 0x202020 )
  ui.progressScreen.stageLabel.Align = AlignLeft | AlignBottom
  column.Add( ui.progressScreen.stageLabel )


  // Информация о загрузке
  ui.progressScreen.downloadInfoLabel = NewSgItemText( 0, 0, "Uli", 20, 0x202020 )
  ui.progressScreen.downloadInfoLabel.Align = AlignLeft | AlignBottom
  column.Add( ui.progressScreen.downloadInfoLabel )
  ui.progressScreen.view.CentralSet( column )


  // Кнопка прерывания
  ui.progressScreen.cancelBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.ProgressCancelBtn )
  ui.progressScreen.cancelBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    }
  ui.progressScreen.view.AddFooter( ui.progressScreen.cancelBtn )

  ui.stack.Add( ui.progressScreen.view )
  }





func (ui *InstallUI) createCompleteScreen() {
  ui.completeScreen.view = NewSgItemPage( 0xf0f0f0, ui.texts.CompleteTitle, 30, 0x202020 )

  // Финальное сообщение
  ui.completeScreen.completeLabel = NewSgItemTextMulty( 0, 0, 0, ui.texts.CompleteMessage, 23, 0x202020 )
  ui.completeScreen.completeLabel.Align = AlignLeft | AlignVCenter

  ui.completeScreen.view.CentralSet( ui.completeScreen.completeLabel )

  // Кнопка завершения
  ui.completeScreen.finishBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.CompleteFinishBtn )
  ui.completeScreen.finishBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    }
  ui.completeScreen.view.AddFooter( ui.completeScreen.finishBtn )

  ui.stack.Add( ui.completeScreen.view )
  }








func (ui *InstallUI) createUpdateScreen() {
  ui.updateScreen.view = NewSgItemPage( 0xf0f0f0, ui.texts.UpdateTitle, 30, 0x202020 )

  ui.updateScreen.messageLabel = NewSgItemTextMulty( 0, 0, 0, ui.texts.UpdateMessage, 23, 0x202020 )
  ui.updateScreen.messageLabel.Align = AlignLeft | AlignVCenter

  ui.updateScreen.view.CentralSet( ui.updateScreen.messageLabel )


  // Кнопки
  ui.updateScreen.updateBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.UpdateNowBtn )
  ui.updateScreen.updateBtn.OnClick = func(item *SgItem, localX int, localY int) {
    //ui.ShowScreen( ScreenProgress )
    //  go performInstallation(ui, ui.cfg, ZipFiles) // Обновляем все файлы
    }
  ui.updateScreen.view.AddFooter( ui.updateScreen.updateBtn )

  ui.updateScreen.laterBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.UpdateLaterBtn )
  ui.updateScreen.view.AddFooter( ui.updateScreen.laterBtn )


  ui.updateScreen.cancelBtn = NewSgItemButton( 20, 160, 100, 30, ui.texts.UpdateCancelBtn )
  ui.updateScreen.cancelBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    os.Exit(0)
    }
  ui.updateScreen.view.AddFooter( ui.updateScreen.cancelBtn )

  ui.stack.Add( ui.updateScreen.view )
  }


func (ui *InstallUI) createDeleteScreen() {
  ui.deleteScreen.view = NewSgItemPage( 0xf0f0f0, ui.texts.DeleteTitle, 30, 0x202020 )

  ui.deleteScreen.messageLabel = NewSgItemTextMulty( 0, 0, 0, ui.texts.DeleteMessage, 23, 0x202020 )
  ui.deleteScreen.messageLabel.Align = AlignLeft | AlignVCenter

  ui.deleteScreen.view.CentralSet( ui.deleteScreen.messageLabel )


  // Кнопки
  ui.deleteScreen.deleteBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.DeleteAccBtn )
  ui.deleteScreen.view.AddFooter( ui.deleteScreen.deleteBtn )


  ui.deleteScreen.cancelBtn = NewSgItemButton( 20, 160, 100, 30, ui.texts.DeleteCancelBtn )
  ui.deleteScreen.cancelBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    os.Exit(0)
    }
  ui.deleteScreen.view.AddFooter( ui.deleteScreen.cancelBtn )

  ui.stack.Add( ui.deleteScreen.view )
  }



func (ui *InstallUI) createDeleteProgress() {
  ui.deleteProgress.view = NewSgItemPage( 0xf0f0f0, ui.texts.DeleteProgress, 30, 0x202020 )

  column := NewSgItemColumn( 0, 0, 10, 10, 10 )

  // Прогресс-бар
  ui.deleteProgress.progressBar = NewSgItemProgressBar( 0, 0, 100, 30 )
  ui.deleteProgress.progressBar.AnchorHorzFill( column, 5, 5 )
  column.Add( ui.deleteProgress.progressBar )

  // Удаляемый файл
  ui.deleteProgress.stageLabel = NewSgItemText( 0, 0, "Uli", 20, 0x202020 )
  ui.deleteProgress.stageLabel.Align = AlignLeft | AlignBottom
  column.Add( ui.deleteProgress.stageLabel )

  ui.deleteProgress.view.CentralSet( column )


  // Кнопка прерывания
  ui.deleteProgress.cancelBtn = NewSgItemButton( 0, 0, 100, 30, ui.texts.ProgressCancelBtn )
  ui.deleteProgress.cancelBtn.OnClick = func(item *SgItem, localX int, localY int) {
    SgWinClose()
    }
  ui.deleteProgress.view.AddFooter( ui.deleteProgress.cancelBtn )

  ui.stack.Add( ui.deleteProgress.view )
  }
