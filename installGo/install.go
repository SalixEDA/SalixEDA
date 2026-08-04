// install.go
package main

import (
  "archive/zip"
  "fmt"
  "io"
  "log"
  "net/http"
  "os"
  "path/filepath"
  "time"
  "sync/atomic"
  )

var isInstalling int32 // 0 - свободен, 1 - процесс идет

func showErrorAndExit(ui *InstallUI, msg string) {
  //Блокирующий вызов модального окна
  ui.messageBox.Message( ui.texts.ErrorTitle, msg )
  //Закрываем визуальную подсистему
  SgWinClose()
  //Выход из программы
  os.Exit(0)
  }




// doSetup выполняет установку приложения с графическим интерфейсом
func doSetup(cfg *Config) {
  // Проверяем поле с каталогом установки
  if cfg.InstallPath == "" {
    // Если пустое, заменяем ВСЮ конфигурацию на конфигурацию по умолчанию
    *cfg = *DefaultConfig()
    log.Printf("Путь установки не указан, использую конфигурацию по умолчанию")
  }

  // Создаем интерфейс
  ui := NewInstallUI(cfg)

  // Показываем экран установки
  ui.stack.SetCurrent( 0 )
  ui.setupScreen.pathLabel.SetFocus()

  // Запускаем окно (блокируется до закрытия)
  SgWinStart( 300, 600, 800, 300, "SalixEDA setup program" )
  }








// performInstallation выполняет установку или обновление с визуальным интерфейсом
func performInstallation(ui *InstallUI, cfg *Config, filesToUpdate []string) {
  // Пытаемся установить флаг в 1. Если он уже 1, значит, горутина уже работает — выходим
  if !atomic.CompareAndSwapInt32(&isInstalling, 0, 1) {
    return
  }

  // Гарантируем сброс флага обратно в 0 при любом выходе из функции (успех или ошибка)
  defer atomic.StoreInt32(&isInstalling, 0)

  // Определяем режим работы
  isInstall := len(filesToUpdate) == 0

  if isInstall {
    // Если установка, берем все файлы из глобального списка
    filesToUpdate = ZipFiles
    // Создаем каталог установки, если его нет
    if err := os.MkdirAll(cfg.InstallPath, 0755); err != nil {
      showErrorAndExit(ui, fmt.Sprintf("Не удалось создать каталог установки: %v", err))
      return
    }
    log.Println("Режим: установка")
  } else {
    log.Printf("Режим: обновление (%d файлов)", len(filesToUpdate))
  }

  // Показываем экран прогресса
  ui.stack.SetCurrent(1)
  ui.progressScreen.stageLabel.TextSet(ui.texts.ProgressPrepare)
  //ui.progressScreen.percentLabel.SetText("0%")
  ui.progressScreen.progressBar.SetValue(0)
  ui.progressScreen.downloadInfoLabel.TextSet("")

  // Переменная для отслеживания прогресса
  var currentProgress float64 = 0

  // ===== ЭТАП 1: Скачивание ZIP файлов (40%) =====
  // Сколько процентов на каждый файл
  percentPerFile := 40.0 / float64(len(filesToUpdate))

  for _, fileName := range filesToUpdate {
    // Обновляем информацию о текущем файле
    ui.progressScreen.stageLabel.TextSet(fmt.Sprintf(ui.texts.ProgressDownload, fileName))

    // Формируем URL для скачивания
    fileURL := fmt.Sprintf("%s/%s", ServerURL, fileName)
    destPath := filepath.Join(cfg.InstallPath, fileName)

    // Получаем информацию о файле на сервере
    client := &http.Client{Timeout: 30 * time.Second}
    resp, err := client.Head(fileURL)
    if err != nil {
      showErrorAndExit(ui, fmt.Sprintf("Ошибка подключения к серверу: %v", err))
      return
    }

    // Получаем размер файла и дату модификации
    fileSize := resp.ContentLength
    var modTime time.Time
    if lastMod := resp.Header.Get("Last-Modified"); lastMod != "" {
      modTime, _ = http.ParseTime(lastMod)
    }
    resp.Body.Close()

    if fileSize <= 0 {
      showErrorAndExit(ui, fmt.Sprintf("Не удалось определить размер файла %s", fileName))
      return
    }

    // Создаем файл для записи
    outFile, err := os.Create(destPath)
    if err != nil {
      showErrorAndExit(ui, fmt.Sprintf("Не удалось создать файл: %v", err))
      return
    }

    // Скачиваем с отображением прогресса
    resp, err = client.Get(fileURL)
    if err != nil {
      outFile.Close()
      showErrorAndExit(ui, fmt.Sprintf("Ошибка скачивания: %v", err))
      return
    }

    // Создаем reader с отслеживанием прогресса
    var downloaded int64
    buf := make([]byte, 256*1024) // 256KB буфер

    for {
      n, err := resp.Body.Read(buf)
      if n > 0 {
        _, writeErr := outFile.Write(buf[:n])
        if writeErr != nil {
          resp.Body.Close()
          outFile.Close()
          showErrorAndExit(ui, fmt.Sprintf("Ошибка записи: %v", writeErr))
          return
          }

        downloaded += int64(n)

        // Обновляем прогресс
        progress := currentProgress + (float64(downloaded)/float64(fileSize))*percentPerFile

        // Обновляем информацию о скачивании
        downloadInfo := fmt.Sprintf(ui.texts.ProgressDownloadOf,
          formatBytes(downloaded), formatBytes(fileSize))

        ui.progressScreen.progressBar.SetValue(progress/100)
        //  ui.progressScreen.percentLabel.SetText(fmt.Sprintf("%d%%", int(progress)))
        ui.progressScreen.downloadInfoLabel.TextSet(downloadInfo)
        }

      if err != nil {
        if err.Error() == "EOF" {
          break
          }
        resp.Body.Close()
        outFile.Close()
        showErrorAndExit(ui, fmt.Sprintf("Ошибка чтения: %v", err))
        return
        }
      }

    resp.Body.Close()
    outFile.Close()

    // Обновляем время в конфигурации для этого файла
    for i := range cfg.DownloadedFiles {
      if cfg.DownloadedFiles[i].Name == fileName {
        cfg.DownloadedFiles[i].ModTime = modTime
        break
        }
      }

    // Обновляем общий прогресс
    currentProgress += percentPerFile
    ui.progressScreen.progressBar.SetValue(currentProgress/100)
//    ui.progressScreen.percentLabel.SetText(fmt.Sprintf("%d%%", int(currentProgress)))
    }

  // Сохраняем конфигурацию с обновленными временами файлов
  if err := cfg.Save(); err != nil {
    showErrorAndExit(ui, fmt.Sprintf("Ошибка сохранения конфигурации: %v", err))
    return
    }

  // ===== ЭТАП 2: Распаковка ZIP файлов (40%) =====
  ui.progressScreen.stageLabel.TextSet(ui.texts.ProgressExtract)
  percentPerFile = 40.0 / float64(len(filesToUpdate))
  currentProgress = 40.0

  for _, fileName := range filesToUpdate {
    zipPath := filepath.Join(cfg.InstallPath, fileName)
    ui.progressScreen.stageLabel.TextSet(fmt.Sprintf(ui.texts.ProgressExtract, fileName))

    // Открываем ZIP архив
    zipReader, err := zip.OpenReader(zipPath)
    if err != nil {
      showErrorAndExit(ui, fmt.Sprintf("Не удалось открыть ZIP архив: %v", err))
      return
      }

    // Подсчитываем общий размер распаковываемых файлов для прогресса
    var totalUnpackSize int64
    for _, f := range zipReader.File {
      totalUnpackSize += int64(f.UncompressedSize64)
      }

    var unpacked int64
    for _, f := range zipReader.File {
      // Распаковываем файл
      fpath := filepath.Join(cfg.InstallPath, f.Name)

      // Создаем директории
      if f.FileInfo().IsDir() {
        os.MkdirAll(fpath, f.Mode())
        continue
        }

      os.MkdirAll(filepath.Dir(fpath), os.ModePerm)

      // Открываем файл в архиве
      rc, err := f.Open()
      if err != nil {
        zipReader.Close()
        showErrorAndExit(ui, fmt.Sprintf("Не удалось открыть файл в архиве: %v", err))
        return
        }

      // Проверяем, является ли файл символической ссылкой
      if f.Mode()&os.ModeSymlink != 0 {
        // Читаем цель ссылки
        linkTarget, err := io.ReadAll(rc)
        rc.Close()
        if err != nil {
          zipReader.Close()
          showErrorAndExit(ui, fmt.Sprintf("Ошибка чтения ссылки: %v", err))
          return
          }

        // Создаем символическую ссылку
        if err := os.Symlink(string(linkTarget), fpath); err != nil {
          zipReader.Close()
          showErrorAndExit(ui, fmt.Sprintf("Ошибка создания ссылки: %v", err))
          return
          }
        } else {
        // Обычный файл - создаем и копируем
        outFile, err := os.OpenFile(fpath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, f.Mode())
        if err != nil {
          rc.Close()
          zipReader.Close()
          showErrorAndExit(ui, fmt.Sprintf("Не удалось создать файл: %v", err))
          return
          }

        // Копируем с подсчетом прогресса
        written, err := io.Copy(outFile, rc)
        outFile.Close()
        rc.Close()

        if err != nil {
          zipReader.Close()
          showErrorAndExit(ui, fmt.Sprintf("Ошибка при распаковке: %v", err))
          return
          }

        unpacked += written
        }

      // Обновляем прогресс
      fileProgress := float64(unpacked) / float64(totalUnpackSize) * percentPerFile
      totalProgress := currentProgress + fileProgress

      ui.progressScreen.progressBar.SetValue(totalProgress/100)
//        ui.progressScreen.percentLabel.SetText(fmt.Sprintf("%d%%", int(totalProgress)))
      ui.progressScreen.downloadInfoLabel.TextSet(fmt.Sprintf(ui.texts.ProgressExtractedOf,
          formatBytes(unpacked), formatBytes(totalUnpackSize)))
      }

    zipReader.Close()

    // Удаляем ZIP файл после распаковки
    os.Remove(zipPath)

    // Обновляем прогресс после завершения файла
    currentProgress += percentPerFile
    ui.progressScreen.progressBar.SetValue(currentProgress/100)
//      ui.progressScreen.percentLabel.SetText(fmt.Sprintf("%d%%", int(currentProgress)))
    }

  // Если это обновление, то заканчиваем здесь
  if !isInstall {
    SgWinClose()
    return
    }



  // ===== ЭТАП 3: Копирование установщика (10%) =====
  ui.progressScreen.stageLabel.TextSet(ui.texts.ProgressCopy)

  // Получаем путь к текущему исполняемому файлу
  execPath, err := os.Executable()
  if err != nil {
    showErrorAndExit(ui, fmt.Sprintf("Ошибка копирования установщика: %v", err))
    return
    }

  // Определяем имя установщика в целевой папке
  installerName := filepath.Base(execPath)
  destInstallerPath := filepath.Join(cfg.InstallPath, installerName)

  // Копируем файл
  sourceData, err := os.ReadFile(execPath)
  if err != nil {
    showErrorAndExit(ui, fmt.Sprintf("Ошибка чтения установщика: %v", err))
    return
    }

  if err := os.WriteFile(destInstallerPath, sourceData, 0755); err != nil {
    showErrorAndExit(ui, fmt.Sprintf("Ошибка записи установщика: %v", err))
    return
    }

  currentProgress += 10
  ui.progressScreen.progressBar.SetValue(currentProgress/100)
//    ui.progressScreen.percentLabel.SetText("100%")


  // ===== ЭТАП 4: Создание ярлыков (10%) =====
  ui.progressScreen.stageLabel.TextSet(ui.texts.ProgressShortcuts)
  ui.progressScreen.downloadInfoLabel.TextSet("")

  if cfg.CreateShortcuts {
    if err := InstallShortcuts(cfg.InstallPath); err != nil {
      showErrorAndExit(ui, fmt.Sprintf("Ошибка создания ярлыков: %v", err))
      return
      }
    }

  currentProgress += 10
  ui.progressScreen.progressBar.SetValue(currentProgress/100)
//  ui.progressScreen.percentLabel.SetText(fmt.Sprintf("%d%%", int(currentProgress)))


  // ===== ЗАВЕРШЕНИЕ =====
  ui.messageBox.Message( ui.texts.CompleteTitle, ui.texts.CompleteMessage )
  SgWinClose()
  }




// Вспомогательные функции
func formatBytes(bytes int64) string {
  const unit = 1024
  if bytes < unit {
    return fmt.Sprintf("%d B", bytes)
  }
  div, exp := int64(unit), 0
  for n := bytes / unit; n >= unit; n /= unit {
    div *= unit
    exp++
  }
  return fmt.Sprintf("%.1f %cB", float64(bytes)/float64(div), "KMGTPE"[exp])
}


