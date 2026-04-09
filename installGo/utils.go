// utils.go
package main

import (
  "fmt"
  "os"
  "path/filepath"
  "runtime"
)




// InstallShortcuts создает ярлыки в нужных местах для текущей ОС
func InstallShortcuts(installPath string) error {

  //Application name executable and uninstall executable
  exeName := ApplicationName
  uninstallName := "uninstall"
  if runtime.GOOS == "windows" {
    exeName += ".exe"
    uninstallName += ".exe"
  }

  //Application full path
  targetPath := filepath.Join(installPath, exeName)
  //Uninstall full path
  uninstallPath := filepath.Join(installPath, uninstallName)

  // Проверяем, существует ли исполняемый файл
  if _, err := os.Stat(targetPath); err != nil {
    return fmt.Errorf("executable not found: %s", targetPath)
  }

  // Путь к иконке (если есть)
  iconPath := filepath.Join(installPath, "icon.png")
  if _, err := os.Stat(iconPath); err != nil {
    iconPath = "" // иконки нет, используем стандартную
  }

  uninstallIconPath := ""

  switch runtime.GOOS {
  case "windows":
    // Ярлык в меню Пуск
    startMenu := filepath.Join(os.Getenv("APPDATA"),
      "Microsoft", "Windows", "Start Menu", "Programs",
      CompanyName, ApplicationName+".lnk")

    if err := CreateShortcut(targetPath, startMenu, ApplicationName+" schematic and pcb EDA", iconPath); err != nil {
      return err
      }

    //Добавляем ярлык удалятора
    uninstallMenu := filepath.Join(os.Getenv("APPDATA"),
      "Microsoft", "Windows", "Start Menu", "Programs",
      CompanyName, "uninstall"+ApplicationName+".lnk")

    if err := CreateShortcut(uninstallPath, uninstallMenu, "Uninstall "+ApplicationName, uninstallIconPath); err != nil {
      return err
      }


    // Ярлык на рабочем столе
    desktop := filepath.Join(os.Getenv("USERPROFILE"), "Desktop", ApplicationName+".lnk")
    return CreateShortcut(targetPath, desktop, "", iconPath)

  case "linux":
    // Ярлык в меню приложений (системный)
    systemMenu := filepath.Join(os.Getenv("HOME"), ".local", "share", "applications",
      CompanyName+"-"+ApplicationName+".desktop")

    if err := CreateShortcut(targetPath, systemMenu, ApplicationName+" schematic and pcb EDA", iconPath); err != nil {
      return err
      }

    //Ярлык удалятора
    uninstallMenu := filepath.Join(os.Getenv("HOME"), ".local", "share", "applications",
      CompanyName+"-uninstall"+ApplicationName+".desktop")

    if err := CreateShortcut(uninstallPath, uninstallMenu, "Uninstall "+ApplicationName, uninstallIconPath); err != nil {
      return err
      }

    // Ярлык на рабочем столе (опционально)
    desktop := filepath.Join(os.Getenv("HOME"), "Desktop", ApplicationName+".desktop")

    // На многих Linux системах рабочий стол может быть в другом месте
    if _, err := os.Stat(filepath.Dir(desktop)); err == nil {
      return CreateShortcut(targetPath, desktop, "", iconPath)
      }
    return nil

  case "darwin":
    // Для macOS можно создать .app bundle или просто ярлык
    // Пока возвращаем nil или заглушку
    return nil

  default:
    return fmt.Errorf("unsupported OS: %s", runtime.GOOS)
  }
}


