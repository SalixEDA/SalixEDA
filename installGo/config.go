// config.go
// Пакет для работы с конфигурацией приложения
// Данные хранятся в JSON-файле в системной директории настроек:
//   Windows: %APPDATA%\CompanyName\ApplicationName\config.json
//   Linux:   ~/.config/CompanyName/ApplicationName/config.json
//   macOS:   ~/Library/Application Support/CompanyName/ApplicationName/config.json

package main

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"runtime"
	"time"
)

// ========== ГЛОБАЛЬНЫЕ КОНСТАНТЫ ==========

const (
	// CompanyName - название компании (используется в пути к конфигурации)
	CompanyName = "YourCompany"

	// ApplicationName - название приложения (используется в пути к конфигурации)
	ApplicationName = "YourApp"
)

// ZipFiles - список ZIP-файлов для мониторинга и обновления
// Это константы, так как набор файлов известен на этапе компиляции
var ZipFiles = []string{
	"core.zip",
	"assets.zip",
	"plugins.zip",
}

// ========== СТРУКТУРА КОНФИГУРАЦИИ ==========

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
}

// ========== ФУНКЦИИ ДЛЯ РАБОТЫ С ПУТЯМИ ==========

// getConfigDir возвращает путь к директории конфигурации для текущей ОС
func getConfigDir() (string, error) {
	var configDir string

	switch runtime.GOOS {
	case "windows":
		// Windows: %APPDATA%\CompanyName\ApplicationName
		appData := os.Getenv("APPDATA")
		if appData == "" {
			return "", fmt.Errorf("APPDATA environment variable not set")
		}
		configDir = filepath.Join(appData, CompanyName, ApplicationName)

	case "linux":
		// Linux: ~/.config/CompanyName/ApplicationName
		configHome := os.Getenv("XDG_CONFIG_HOME")
		if configHome == "" {
			home := os.Getenv("HOME")
			if home == "" {
				return "", fmt.Errorf("HOME environment variable not set")
			}
			configHome = filepath.Join(home, ".config")
		}
		configDir = filepath.Join(configHome, CompanyName, ApplicationName)

	case "darwin":
		// macOS: ~/Library/Application Support/CompanyName/ApplicationName
		home := os.Getenv("HOME")
		if home == "" {
			return "", fmt.Errorf("HOME environment variable not set")
		}
		configDir = filepath.Join(home, "Library", "Application Support", CompanyName, ApplicationName)

	default:
		return "", fmt.Errorf("unsupported operating system: %s", runtime.GOOS)
	}

	return configDir, nil
}

// getConfigPath возвращает полный путь к файлу конфигурации
func getConfigPath() (string, error) {
	configDir, err := getConfigDir()
	if err != nil {
		return "", err
	}
	return filepath.Join(configDir, "config.json"), nil
}

// ensureConfigDir создает директорию конфигурации, если её нет
func ensureConfigDir() error {
	configDir, err := getConfigDir()
	if err != nil {
		return err
	}

	// Создаем директорию с правами 0755 (rwxr-xr-x)
	if err := os.MkdirAll(configDir, 0755); err != nil {
		return fmt.Errorf("failed to create config directory: %w", err)
	}

	return nil
}

// ========== ФУНКЦИИ ЗАГРУЗКИ/СОХРАНЕНИЯ ==========

// LoadConfig загружает конфигурацию из файла
// Если файл не существует, возвращает конфигурацию по умолчанию
func LoadConfig() (*Config, error) {
	configPath, err := getConfigPath()
	if err != nil {
		return nil, err
	}

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

// DefaultConfig возвращает конфигурацию по умолчанию
func DefaultConfig() *Config {
	return &Config{
		CheckPeriodDays:  7,  // проверка раз в неделю
		LastCheck:        "", // никогда не проверяли
		InstallPath:      "",
		CreateShortcuts:  true,
	}
}

// Save сохраняет конфигурацию в файл
func (c *Config) Save() error {
	// Убеждаемся, что директория существует
	if err := ensureConfigDir(); err != nil {
		return err
	}

	configPath, err := getConfigPath()
	if err != nil {
		return err
	}

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

