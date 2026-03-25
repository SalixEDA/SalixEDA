package main

import (
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
)

func main() {
  addOSPrefix()

  // ===== 1. ЗАГРУЗКА КОНФИГУРАЦИИ =====
  loadedConfig, loadConfigErr := LoadConfig()

  // ===== 2. ДИРЕКТОРИЯ ИСПОЛНЯЕМОГО ФАЙЛА УСТАНОВЩИКА =====
  execPath, err := os.Executable()
  if err != nil {
    log.Fatalf("Критическая ошибка: не удалось путь к исполняемому файлу: %v", err)
    }
  execDir := filepath.Dir(execPath)

  log.Printf("Директория установщика: %s", execDir)

  // ===== 3. ПРОВЕРКА НЕОБХОДИМОСТИ УСТАНОВКИ =====
  if loadConfigErr != nil || !strings.EqualFold(filepath.Clean(execDir), filepath.Clean(loadedConfig.InstallPath)) {
    log.Println("=== НАЧАЛО УСТАНОВКИ ===")
    doSetup(loadedConfig)
    log.Println("=== УСТАНОВКА ЗАВЕРШЕНА ===")
  } else {
    // ===== 4. ПРОВЕРКА ОБНОВЛЕНИЙ =====
    if loadedConfig.NeedCheck() {
      log.Println("=== НАЧАЛО ПРОВЕРКИ ОБНОВЛЕНИЙ ===")
      doUpdate(loadedConfig)
      log.Println("=== ПРОВЕРКА ОБНОВЛЕНИЙ ЗАВЕРШЕНА ===")
      }
    }

  // ===== 5. ЗАПУСК ОСНОВНОГО ПРИЛОЖЕНИЯ =====
  appName := ApplicationName + "int"
  if runtime.GOOS == "windows" {
    appName += ".exe"
    }
  appPath := filepath.Join(loadedConfig.InstallPath, appName)

  if _, err := os.Stat(appPath); os.IsNotExist(err) {
    log.Fatalf("Критическая ошибка: исполняемый файл приложения не найден: %s", appPath)
    }

  // ===== 6. УСТАНОВКА ПЕРЕМЕННЫХ ОКРУЖЕНИЯ =====
  env := os.Environ()

  libPath := filepath.Join(loadedConfig.InstallPath, "lib")

  if runtime.GOOS == "linux" {
    currentLDLibraryPath := os.Getenv("LD_LIBRARY_PATH")
    newLDLibraryPath := libPath
    if currentLDLibraryPath != "" {
      newLDLibraryPath = currentLDLibraryPath + ":" + libPath
      }
    env = append(env, "LD_LIBRARY_PATH="+newLDLibraryPath)
    println( "Path lib", newLDLibraryPath )
    }

  pluginsPath := filepath.Join(loadedConfig.InstallPath, "plugins")
  env = append(env, "QT_PLUGIN_PATH="+pluginsPath)
  println( "Path plugins", pluginsPath )

  if runtime.GOOS == "windows" {
    currentPath := os.Getenv("PATH")
    newPath := loadedConfig.InstallPath + ";" + libPath + ";" + currentPath
    env = append(env, "PATH="+newPath)
    }

  if runtime.GOOS == "darwin" {
    currentDyldPath := os.Getenv("DYLD_LIBRARY_PATH")
    newDyldPath := libPath
    if currentDyldPath != "" {
      newDyldPath = currentDyldPath + ":" + libPath
      }
    env = append(env, "DYLD_LIBRARY_PATH="+newDyldPath)
    }

  // ===== 7. ЗАПУСК ПРИЛОЖЕНИЯ =====
  args := os.Args[1:]

  cmd := exec.Command(appPath, args...)
  cmd.Dir = loadedConfig.InstallPath
  cmd.Env = env
  cmd.Stdin = os.Stdin
  cmd.Stdout = os.Stdout
  cmd.Stderr = os.Stderr

  cmd.Run()
  }
