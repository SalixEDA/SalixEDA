//go:build windows
// +build windows

package main

import (
  "fmt"
  "os"
  "os/exec"
  "path/filepath"
  "syscall"
  "strings"
  "unsafe"
)

func createWindowsShortcut( target, shortcutPath, description, iconPath string) error {
  // Убеждаемся, что расширение .lnk
  if filepath.Ext(shortcutPath) != ".lnk" {
    shortcutPath += ".lnk"
  }

  // Создаем директорию для ярлыка, если её нет
  if err := os.MkdirAll(filepath.Dir(shortcutPath), 0755); err != nil {
    return fmt.Errorf("не удалось создать директорию для ярлыка: %w", err)
  }

  // Экранируем пути для PowerShell
  escapedShortcutPath := strings.ReplaceAll(shortcutPath, "'", "''")
  escapedTarget := strings.ReplaceAll(target, "'", "''")
  escapedWorkingDir := strings.ReplaceAll(filepath.Dir(target), "'", "''")
  escapedDescription := strings.ReplaceAll(description, "'", "''")

  // PowerShell скрипт для создания ярлыка
  psScript := fmt.Sprintf(`
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut('%s')
$Shortcut.TargetPath = '%s'
$Shortcut.WorkingDirectory = '%s'
$Shortcut.Description = '%s'
$Shortcut.Save()
`, escapedShortcutPath, escapedTarget, escapedWorkingDir, escapedDescription)

  // Если указана иконка, добавляем её
  if iconPath != "" {
    escapedIconPath := strings.ReplaceAll(iconPath, "'", "''")
    psScript = fmt.Sprintf(`
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut('%s')
$Shortcut.TargetPath = '%s'
$Shortcut.WorkingDirectory = '%s'
$Shortcut.Description = '%s'
$Shortcut.IconLocation = '%s, 0'
$Shortcut.Save()
`, escapedShortcutPath, escapedTarget, escapedWorkingDir, escapedDescription, escapedIconPath)
  }

  // Создаем команду PowerShell
  cmd := exec.Command("powershell.exe", "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", psScript)

  // Скрываем окно PowerShell
  cmd.SysProcAttr = &syscall.SysProcAttr{
    HideWindow: true,
  }

  // Запускаем и ожидаем завершения
  if err := cmd.Run(); err != nil {
    return fmt.Errorf("ошибка выполнения PowerShell: %w", err)
  }

  return nil
}


func CreateShortcut( title, target, shortcutPath, description, iconPath string) error {
  return createWindowsShortcut(target, shortcutPath, description, iconPath)
}


func windowsLanguage() string {
  var kernel32 = syscall.NewLazyDLL("kernel32.dll")
  var procGetUserDefaultLocaleName = kernel32.NewProc("GetUserDefaultLocaleName");
  buf := make([]uint16, 85)

  r, _, _ := procGetUserDefaultLocaleName.Call( uintptr(unsafe.Pointer(&buf[0])), uintptr(len(buf)) )

  if r != 0 {
    return syscall.UTF16ToString(buf)
  }

  return ""
  }
