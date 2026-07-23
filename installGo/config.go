// config.go
// Пакет для работы с конфигурацией приложения
// Данные хранятся в JSON-файле прямо в директории с программой
// Имя файла конфигурации - update.cnf

package main

import (
  "encoding/json"
  "fmt"
  "os"
  "os/user"
  "path/filepath"
  "runtime"
  "time"
  "unicode"
)

// ========== ГЛОБАЛЬНЫЕ КОНСТАНТЫ ==========

const (
  // CompanyName - название компании (используется в пути к конфигурации)
  CompanyName = "SalixEDA"

  // ApplicationName - название приложения (используется в пути к конфигурации)
  ApplicationName = "SalixEDA"

  // ServerURL - базовый URL сервера с файлами для скачивания
  ServerURL = "https://salixeda.org/data"

  //Имя файла конфигурации
  ConfigFileName = "update.cnf"
)

// ZipFiles - список ZIP-файлов для мониторинга и обновления
// Это константы, так как набор файлов известен на этапе компиляции
// На сервере эти файлы должны иметь имена winCore.zip или linCore.zip
// в зависимости от операционной системы. Если имя начинается с маленькой буквы
// то префикс операционной системы не добавляется, т.е. такие архивы общие для
// всех ос, например videos.zip
var ZipFiles = []string{
	"Core.zip",
	"Libs.zip",
        "Plugins.zip",
        "compStore.zip",
        "guide.zip",
	"examples.zip",
}

// ========== СТРУКТУРА КОНФИГУРАЦИИ ==========

// FileInfo структура для хранения информации о загруженных файлах
type FileInfo struct {
  Name    string    `json:"name"`
  ModTime time.Time `json:"mod_time"`
}

// Config структура конфигурации установки приложения
// Сохраняется в JSON-файл и доступна как установщику, так и основной программе
type Config struct {
  // Период проверки обновлений в днях
  CheckPeriodDays int `json:"check_period_days"`

  // Время последней проверки обновлений (RFC3339)
  LastCheck string `json:"last_check"`

  // Путь к установленной программе
  InstallPath string `json:"install_path"`

  // Создавать ли ярлыки (только для первой установки)
  CreateShortcuts bool `json:"create_shortcuts"`

  // Список загруженных файлов с их датами
  DownloadedFiles []FileInfo `json:"downloaded_files,omitempty"`
}



// ========== ФУНКЦИИ ДЛЯ РАБОТЫ С ПУТЯМИ ==========

func addOSPrefix() {
  prefix := "lin"
  if runtime.GOOS == "windows" {
    prefix = "win"
    }

  for i, file := range ZipFiles {
    if len(file) > 0 && unicode.IsUpper(rune(file[0])) {
      ZipFiles[i] = prefix + file
      }
    }
  }





// ========== ФУНКЦИИ ЗАГРУЗКИ/СОХРАНЕНИЯ ==========

// LoadConfig загружает конфигурацию из файла
// Если файл не существует, возвращает конфигурацию по умолчанию
func LoadConfig() (*Config, error) {
  // Получаем путь к исполняемому файлу
  executablePath, err := os.Executable()
  if err != nil {
    return nil, fmt.Errorf("failed to get executable path: %w", err)
    }

  // Получаем каталог исполняемого файла
  executableDir := filepath.Dir(executablePath)

  // Формируем полный путь к файлу конфигурации
  configPath := filepath.Join(executableDir, ConfigFileName)

  // Пытаемся прочитать существующий файл
  data, err := os.ReadFile(configPath)
  if err != nil {
    if os.IsNotExist(err) {
      // Файл не существует - возвращаем конфиг по умолчанию
      return DefaultConfig(), nil
      }
    return nil, fmt.Errorf("failed to read config file: %w", err)
    }

  // Парсим JSON
  var config Config
  if err := json.Unmarshal(data, &config); err != nil {
    return nil, fmt.Errorf("failed to parse config JSON: %w", err)
    }

  return &config, nil
  }




// getHomeDirFromUser получает домашний каталог через пакет os/user
func getHomeDirFromUser() (string, error) {
  usr, err := user.Current()
  if err != nil {
    return "", fmt.Errorf("не удалось получить текущего пользователя: %v", err)
    }
  return usr.HomeDir, nil
  }




// buildMyPath строит путь "myPath" относительно домашнего каталога
func buildSubPath( subPath string ) string {
  home, err := getHomeDirFromUser()
  if err != nil {
    return ""
    }

  myPath := filepath.Join(home, subPath)
  return myPath
  }




// DefaultConfig возвращает конфигурацию по умолчанию
func DefaultConfig() *Config {
  // Создаем список файлов из глобального списка ZipFiles с нулевыми датами
  downloadedFiles := make([]FileInfo, len(ZipFiles))
  for i, name := range ZipFiles {
    downloadedFiles[i] = FileInfo{
      Name:    name,
      ModTime: time.Time{}, // нулевое время
    }
  }

  return &Config{
    CheckPeriodDays:  7,
    LastCheck:        "",
    InstallPath:      buildSubPath("SalixEDA/"),
    CreateShortcuts:  true,
    DownloadedFiles:  downloadedFiles,
  }
}




// Save сохраняет конфигурацию в файл
func (c *Config) Save() error {
  // Проверяем, что InstallPath не пустой
  if c.InstallPath == "" {
    return fmt.Errorf("install_path is empty, cannot determine config location")
    }

  // Убеждаемся, что директория установки существует
  if err := os.MkdirAll(c.InstallPath, 0755); err != nil {
    return fmt.Errorf("failed to create install directory: %w", err)
    }

  // Формируем полный путь к файлу конфигурации
  configPath := filepath.Join(c.InstallPath, ConfigFileName)

  // Сериализуем в JSON с отступами для читаемости
  data, err := json.MarshalIndent(c, "", "  ")
  if err != nil {
    return fmt.Errorf("failed to marshal config: %w", err)
    }

  // Записываем файл с правами 0644 (rw-r--r--)
  if err := os.WriteFile(configPath, data, 0644); err != nil {
    return fmt.Errorf("failed to write config file: %w", err)
    }

  return nil
  }



// ========== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ==========

// UpdateLastCheck обновляет время последней проверки на текущее
func (c *Config) UpdateLastCheck() {
  c.LastCheck = time.Now().Format(time.RFC3339)
  }



// NeedCheck определяет, нужно ли проверить обновления
func (c *Config) NeedCheck() bool {
  if c.LastCheck == "" {
    return true // никогда не проверяли
    }

  last, err := time.Parse(time.RFC3339, c.LastCheck)
  if err != nil {
    return true // ошибочная дата - проверяем
    }

  daysSince := int(time.Since(last).Hours() / 24)
  return daysSince >= c.CheckPeriodDays
  }



// Reset сбрасывает конфигурацию к значениям по умолчанию
func (c *Config) Reset() {
  *c = *DefaultConfig()
  }

