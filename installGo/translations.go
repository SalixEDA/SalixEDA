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
  ProgressCopy        string
  ProgressShortcuts   string
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

  // Экран подтверждения удаления
  DeleteTitle      string
  DeleteMessage    string
  DeleteCancelBtn  string
  DeleteAccBtn     string

  DeleteProgress   string
  DeleteFile       string

  DeleteInfo       string
  DeleteComplete   string
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
    ProgressCopy:        "Этап 3: Копирование установщика",
    ProgressShortcuts:   "Этап 4: Создание ярлыков",
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

    DeleteTitle:      "Подтверждение удаления",
    DeleteMessage:    "Вы действительно хотите удалить программу SalixEDA и все ее компоненты и данные?",
    DeleteCancelBtn:  "Отмена",
    DeleteAccBtn:     "Да, удалить",

    DeleteProgress:   "Удаление SalixEDA",
    DeleteFile:       "Удаляется %s",

    DeleteInfo:       "Информация",
    DeleteComplete:   "Программа SalixEDA успешно удалена с вашего компьютера",
  }
}

func TransStringEn() *Texts {
  return &Texts{
    // Общие
    AppTitle: "Program Installation",

    // Setup screen
    SetupTitle:       "Program Installation",
    SetupInfo:        "This program will install the application on your computer." +
                      "Please enter the installation folder and click the 'Install' button." +
                      "You can change the folder using the keyboard.",
    SetupFolderLabel: "Installation folder:",
    SetupBrowseBtn:   "Browse...",
    SetupExitBtn:     "Exit",
    SetupInstallBtn:  "Install",

    // Progress screen
    ProgressTitle:       "Installation",
    ProgressPrepare:     "Preparing...",
    ProgressCancelBtn:   "Cancel",
    ProgressDownload:    "Step 1: Downloading %s",
    ProgressExtract:     "Step 2: Extracting %s",
    ProgressCopy:        "Step 3: Copying installer",
    ProgressShortcuts:   "Step 4: Creating shortcuts",
    ProgressDownloadOf:  "Downloaded %s of %s",
    ProgressExtractedOf: "Extracted %s of %s",

    // Completion screen
    CompleteTitle:       "Installation Complete",
    CompleteMessage:     "Installation completed successfully!",
    CompleteFinishBtn:   "Finish",

    // Error screen
    ErrorTitle:       "Error",
    ErrorCloseBtn:    "Close",

    // Update prompt screen
    UpdateTitle:      "Attention",
    UpdateMessage:    "An update is available. We can download and update now or do it later.",
    UpdateCancelBtn:  "Cancel",
    UpdateLaterBtn:   "Later",
    UpdateNowBtn:     "Update",

    DeleteTitle:      "Confirm Deletion",
    DeleteMessage:    "Are you sure you want to delete SalixEDA program and all its components and data?",
    DeleteCancelBtn:  "Cancel",
    DeleteAccBtn:     "Yes, delete",

    DeleteProgress:   "Removing SalixEDA",
    DeleteFile:       "Deleting %s",

    DeleteInfo:       "Information",
    DeleteComplete:   "SalixEDA has been successfully removed from your computer",
  }
}

