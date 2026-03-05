// install.go
package main

import (
  "fmt"
  "log"
  "net/http"
  "time"
)

func doUpdate(cfg *Config) {
  // Получаем текущее время для последующего обновления
  now := time.Now()

  // Список файлов для обновления
  var filesToUpdate []string

  // Проверяем каждый файл из глобального списка
  for _, fileName := range ZipFiles {
    // Формируем URL для проверки
    fileURL := fmt.Sprintf("%s/%s", ServerURL, fileName)

    // Делаем HEAD запрос для получения даты модификации
    client := &http.Client{Timeout: 10 * time.Second}
    resp, err := client.Head(fileURL)
    if err != nil {
      // При любой ошибке просто возвращаемся
      log.Printf("Ошибка проверки файла %s: %v", fileName, err)
      return
    }

    // Получаем дату модификации
    var serverModTime time.Time
    if lastMod := resp.Header.Get("Last-Modified"); lastMod != "" {
      serverModTime, err = http.ParseTime(lastMod)
      if err != nil {
        resp.Body.Close()
        log.Printf("Ошибка парсинга даты для %s: %v", fileName, err)
        return
      }
    }
    resp.Body.Close()

    // Ищем этот файл в конфигурации
    var localModTime time.Time
    found := false
    for _, df := range cfg.DownloadedFiles {
      if df.Name == fileName {
        localModTime = df.ModTime
        found = true
        break
      }
    }

    // Если файла нет в конфигурации или дата на сервере новее
    if !found || serverModTime.After(localModTime) {
      filesToUpdate = append(filesToUpdate, fileName)
    }
  }

  // Если обновлений нет
  if len(filesToUpdate) == 0 {
    cfg.LastCheck = now.Format(time.RFC3339)
    cfg.Save()
    return
  }

  // Создаем интерфейс для запроса обновления
  ui := NewInstallUI(cfg)

  // Настраиваем кнопку "Обновить"
  //oldOnTapped := ui.updateScreen.updateBtn.OnTapped
  ui.updateScreen.updateBtn.OnTapped = func() {
    // Запускаем обновление с найденными файлами
    go performInstallation(ui, cfg, filesToUpdate)
  }

  // Настраиваем кнопку "Позднее"
  ui.updateScreen.laterBtn.OnTapped = func() {
    // Сдвигаем время проверки на период+1 день
    lastCheck, _ := time.Parse(time.RFC3339, cfg.LastCheck)
    newLastCheck := lastCheck.AddDate(0, 0, cfg.CheckPeriodDays+1)
    cfg.LastCheck = newLastCheck.Format(time.RFC3339)
    cfg.Save()
    ui.window.Close()
  }

  // Показываем экран обновления
  ui.ShowScreen(ScreenUpdate)

  // Запускаем окно (блокируется до закрытия)
  ui.window.ShowAndRun()
}

