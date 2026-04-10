//go:build linux || darwin
// +build linux darwin

package main

import (
  "fmt"
  "os"
  "path/filepath"
  "strings"
)





// createLinuxShortcut создает .desktop файл (Linux)
func createLinuxShortcut( title, target, shortcutPath, description, iconPath string) error {
  // Убеждаемся, что расширение .desktop
  if !strings.HasSuffix(strings.ToLower(shortcutPath), ".desktop") {
    shortcutPath += ".desktop"
  }

  // Создаем директорию, если её нет
  if err := os.MkdirAll(filepath.Dir(shortcutPath), 0755); err != nil {
    return fmt.Errorf("failed to create directory for shortcut: %w", err)
  }

  // Формируем содержимое .desktop файла
  content := []string{
    "[Desktop Entry]",
    "Type=Application",
    "Name=" + title,
    "Exec=" + target,
    "Path=" + filepath.Dir(target),
    "Terminal=false",
  }

  if description != "" {
    content = append(content, "Comment="+description)
  }

  if iconPath != "" {
    content = append(content, "Icon="+iconPath)
  } else {
    // Если иконка не указана, пробуем найти рядом с target
    possibleIcon := filepath.Join(filepath.Dir(target), "icon.png")
    if _, err := os.Stat(possibleIcon); err == nil {
      content = append(content, "Icon="+possibleIcon)
    }
  }

  // Записываем файл
  data := strings.Join(content, "\n") + "\n"
  if err := os.WriteFile(shortcutPath, []byte(data), 0644); err != nil {
    return fmt.Errorf("failed to write .desktop file: %w", err)
  }

  // Делаем файл исполняемым (для .desktop это важно)
  if err := os.Chmod(shortcutPath, 0755); err != nil {
    return fmt.Errorf("failed to make .desktop executable: %w", err)
  }

  return nil
}


func CreateShortcut( title, target, shortcutPath, description, iconPath string) error {
  return createLinuxShortcut( title, target, shortcutPath, description, iconPath)
}

