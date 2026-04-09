package main

import (
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"fmt"
        "time"
  //      "syscall"
)


func removeSelf(cfg *Config) {
  // Получаем путь к текущему исполняемому файлу (удалитель)
  exe, err := os.Executable()
  if err != nil {
    log.Printf("Failed to get executable path: %v", err)
    return
    }

  installPath := cfg.InstallPath
  if installPath == "" {
    // Если путь не указан в конфиге, то ничего не делаем
    return
    }

  // 1. Собираем списки файлов и каталогов для удаления
  var filesToDelete []string

  // Добавляем ярлыки в зависимости от ОС
  switch runtime.GOOS {
    case "windows":
      startMenu := filepath.Join(os.Getenv("APPDATA"),
        "Microsoft", "Windows", "Start Menu", "Programs",
        CompanyName, ApplicationName+".lnk")
      filesToDelete = append(filesToDelete, startMenu)

      uinstallMenu := filepath.Join(os.Getenv("APPDATA"),
        "Microsoft", "Windows", "Start Menu", "Programs",
        CompanyName, "uninstall"+ApplicationName+".lnk")
      filesToDelete = append(filesToDelete, uinstallMenu)


      desktop := filepath.Join(os.Getenv("USERPROFILE"), "Desktop", ApplicationName+".lnk")
      filesToDelete = append(filesToDelete, desktop)

    case "linux":
      systemMenu := filepath.Join(os.Getenv("HOME"), ".local", "share", "applications",
        CompanyName+"-"+ApplicationName+".desktop")
      filesToDelete = append(filesToDelete, systemMenu)

      uninstallMenu := filepath.Join(os.Getenv("HOME"), ".local", "share", "applications",
        CompanyName+"-uninstall"+ApplicationName+".desktop")
      filesToDelete = append(filesToDelete, uninstallMenu)


      desktop := filepath.Join(os.Getenv("HOME"), "Desktop", ApplicationName+".desktop")
      filesToDelete = append(filesToDelete, desktop)

    case "darwin":
      // Для macOS можно добавить логику позже
    }

  // Добавляем файл конфигурации
  configPath, _ := getConfigPath()
  if configPath != "" {
    filesToDelete = append(filesToDelete, configPath)
    }

  // Обходим каталог установки и собираем файлы и подкаталоги
  filepath.Walk(installPath, func(path string, info os.FileInfo, err error) error {
    if err != nil {
      return nil
      }

    // Пропускаем корневой каталог
    if path == installPath {
      return nil
      }

    // Пропускаем текущий исполняемый файл
    if path == exe {
      return nil
      }

    if !info.IsDir() {
      // Это файл, добавляем в список файлов
      filesToDelete = append(filesToDelete, path)
      }

    return nil
    })

  totalFiles := float64(len(filesToDelete))
  log.Printf("Found %d files to delete", len(filesToDelete) )





  // Создаем интерфейс для запроса удаления
  ui := NewInstallUI(cfg)

  // Настраиваем кнопку "Удалить"
  ui.deleteScreen.deleteBtn.OnClick = func(item *SgItem, localX int, localY int) {
    // Запускаем удаление с найденными файлами
    go func() {
      ui.stack.SetCurrent( 5 )

      // 2. Удаляем все файлы
      var count float64
      count = float64(0)
      log.Println("Deleting files...")
      for _, file := range filesToDelete {
        // Обновляем информацию о текущем файле
        ui.deleteProgress.stageLabel.TextSet(fmt.Sprintf(ui.texts.DeleteFile, file))

        if err := os.Remove(file); err != nil {
          log.Printf("Failed to delete file %s: %v", file, err)
          } else {
          log.Printf("Deleted file: %s", file)
          }
        ui.deleteProgress.progressBar.SetValue(count/totalFiles)
        count += 1.0
        }

      //Выдаем сообщение, что все удалилось
      ui.messageBox.Message( ui.texts.DeleteInfo, ui.texts.DeleteComplete )

      // 3. Самоудаление через bat/sh скрипт (он удалит каталоги и сам файл программы)
      log.Println("Preparing self-deletion...")

      switch runtime.GOOS {
        case "windows":
          batPath := filepath.Join(os.TempDir(), fmt.Sprintf("uninstall_%d.bat", time.Now().Unix()))
          batContent := fmt.Sprintf(`@echo off
:loop
timeout /t 1 /nobreak > nul
rd /s /q "%s" 2>nul
if exist "%s" goto loop
del /f /q "%%~f0" 2>nul
`, installPath, installPath)

          if err := os.WriteFile(batPath, []byte(batContent), 0644); err != nil {
            ui.messageBox.Message( "Error", "Failed to create bat file" )
            SgWinClose()
            os.Exit(0)
            }

          cmd := exec.Command("cmd", "/c", "start", "/b", batPath)
//          cmd.SysProcAttr = &syscall.SysProcAttr{HideWindow: true}
          cmd.Start()
          log.Printf("Self-deletion scheduled via bat file")

        case "linux", "darwin":
          shPath := filepath.Join(os.TempDir(), fmt.Sprintf("uninstall_%d.sh", time.Now().Unix()))
          shContent := fmt.Sprintf(`#!/bin/sh
sleep 2
rm -rf "%s"
rm -f "$0"
`, installPath)

          if err := os.WriteFile(shPath, []byte(shContent), 0755); err != nil {
            ui.messageBox.Message( "Error", "Failed to create sh file" )
            SgWinClose()
            os.Exit(0)
            }

          cmd := exec.Command("sh", shPath)
          cmd.Start()
          log.Printf("Self-deletion scheduled via sh file")
        }
      SgWinClose()
      os.Exit(0)
      }()
    }

  ui.stack.SetCurrent( 4 )

  // Запускаем окно (блокируется до закрытия)
  SgWinStart( 300, 600, 800, 300, "SalixEDA delete program" )
  }
