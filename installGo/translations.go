package main


// ========== СТРУКТУРА ТЕКСТОВЫХ СТРОК ==========

type Texts struct {
  // Общие
  AppTitle string

  // Экран установки
  SetupTitle       string
  SetupInfo        string
  SetupFolderLabel string
  SetupBrowseBtn   string
  SetupExitBtn     string
  SetupInstallBtn  string

  // Экран прогресса
  ProgressTitle       string
  ProgressPrepare     string
  ProgressCancelBtn   string
  ProgressDownload    string
  ProgressExtract     string
  ProgressShortcuts   string
  ProgressCopy        string
  ProgressDownloadOf  string
  ProgressExtractedOf string

  // Экран завершения
  CompleteTitle       string
  CompleteMessage     string
  CompleteFinishBtn   string

  // Экран ошибки
  ErrorTitle          string
  ErrorCloseBtn       string

  // Экран запроса обновления
  UpdateTitle      string
  UpdateMessage    string
  UpdateCancelBtn  string
  UpdateLaterBtn   string
  UpdateNowBtn     string
}

func TransStringRu() *Texts {
  return &Texts{
    // Общие
    AppTitle: "Установка программы",

    // Экран установки
    SetupTitle:       "Установка программы",
    SetupInfo:        "Эта программа установит приложение на ваш компьютер." +
                      "Пожалуйста, введите папку для установки и нажмите кнопку 'Установить'." +
                      "Вы можете изменить папку с помощью клавиатуры.",
    SetupFolderLabel: "Папка установки:",
    SetupBrowseBtn:   "Обзор...",
    SetupExitBtn:     "Выход",
    SetupInstallBtn:  "Установить",

    // Экран прогресса
    ProgressTitle:       "Установка",
    ProgressPrepare:     "Подготовка...",
    ProgressCancelBtn:   "Прервать",
    ProgressDownload:    "Этап 1: Скачивание %s",
    ProgressExtract:     "Этап 2: Распаковка %s",
    ProgressShortcuts:   "Этап 3: Создание ярлыков",
    ProgressCopy:        "Этап 4: Копирование установщика",
    ProgressDownloadOf:  "Скачано %s из %s",
    ProgressExtractedOf: "Распаковано %s из %s",

    // Экран завершения
    CompleteTitle:       "Установка завершена",
    CompleteMessage:     "Установка завершена успешно!",
    CompleteFinishBtn:   "Завершить",

    // Экран ошибки
    ErrorTitle:       "Ошибка",
    ErrorCloseBtn:    "Закрыть",

    // Экран запроса обновления
    UpdateTitle:      "Внимание",
    UpdateMessage:    "Имеется обновление. Можем скачать и обновить сейчас или сделать это позднее.",
    UpdateCancelBtn:  "Отмена",
    UpdateLaterBtn:   "Позднее",
    UpdateNowBtn:     "Обновить",
  }
}

func TransStringEn() *Texts {
  return &Texts{
    // Общие
    AppTitle: "Program Installation",

    // Экран установки
    SetupTitle:       "Program Installation",
    SetupInfo:        "This program will install the application on your computer.\n\n" +
                      "Please select the installation folder and click 'Install'.\n" +
                      "You can change the folder by clicking 'Browse...'.",
    SetupFolderLabel: "Installation folder:",
    SetupBrowseBtn:   "Browse...",
    SetupExitBtn:     "Exit",
    SetupInstallBtn:  "Install",

    // Экран прогресса
    ProgressPrepare:     "Preparing...",
    ProgressCancelBtn:   "Cancel",
    ProgressDownload:    "Stage 1: Downloading %s",
    ProgressExtract:     "Stage 2: Extracting %s",
    ProgressShortcuts:   "Stage 3: Creating shortcuts",
    ProgressCopy:        "Stage 4: Copying installer",
    ProgressDownloadOf:  "Downloaded %s of %s",
    ProgressExtractedOf: "Extracted %s of %s",

    // Экран завершения
    CompleteMessage:  "Installation completed successfully!",
    CompleteFinishBtn: "Finish",

    // Экран ошибки
    ErrorTitle:       "Error",
    ErrorCloseBtn:    "Close",

    // Экран запроса обновления
    UpdateTitle:      "Attention",
    UpdateMessage:    "An update is available. You can download and install now or do it later.",
    UpdateCancelBtn:  "Cancel",
    UpdateLaterBtn:   "Later",
    UpdateNowBtn:     "Update",
  }
}

