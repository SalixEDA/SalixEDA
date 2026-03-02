// SalixEDA.go
package main

import (
	"archive/zip"
	"fmt"
	"io"
	"net/http"
	"os"
	"path/filepath"
	"time"

	"github.com/lxn/walk"
	. "github.com/lxn/walk/declarative"
)

// Основная структура приложения
type Updater struct {
	InstallDir  string        // где установлена программа
	ServerURL   string        // базовый URL репозитория
	ZipFiles    []string      // список zip-файлов для проверки
	CheckPeriod time.Duration // период проверки (из настроек)
	LastCheck   time.Time     // время последней проверки
}

type Config struct {
	CheckPeriodDays int      `json:"check_period_days"`
	LastCheck       string   `json:"last_check"`
	ZipFiles        []string `json:"zip_files"`
}

// GetFileModTime возвращает дату последнего изменения файла на сервере
func GetFileModTime(url string) (time.Time, error) {
	resp, err := http.Head(url)
	if err != nil {
		return time.Time{}, err
	}
	defer resp.Body.Close()

	// Парсим заголовок Last-Modified
	if modTime := resp.Header.Get("Last-Modified"); modTime != "" {
		return http.ParseTime(modTime)
	}

	// Если сервер не отдает Last-Modified, используем Content-Length как триггер
	// или возвращаем текущее время (тогда обновление по Content-Length)
	return time.Time{}, fmt.Errorf("server doesn't provide Last-Modified")
}

func DownloadFile(url, destPath string) error {
	// Создаем HTTP-клиент с таймаутом
	client := &http.Client{Timeout: 30 * time.Second}

	resp, err := client.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("bad status: %s", resp.Status)
	}

	// Создаем файл
	out, err := os.Create(destPath)
	if err != nil {
		return err
	}
	defer out.Close()

	// Копируем с прогрессом (можно добавить callback для UI)
	_, err = io.Copy(out, resp.Body)
	return err
}

func UnzipWithSymlinks(srcZip, destDir string) error {
	r, err := zip.OpenReader(srcZip)
	if err != nil {
		return err
	}
	defer r.Close()

	for _, f := range r.File {
		fpath := filepath.Join(destDir, f.Name)

		// Проверка на directory traversal
		if !strings.HasPrefix(fpath, filepath.Clean(destDir)+string(os.PathSeparator)) {
			return fmt.Errorf("illegal file path: %s", f.Name)
		}

		if f.FileInfo().IsDir() {
			os.MkdirAll(fpath, os.ModePerm)
			continue
		}

		// Создаем родительскую директорию
		os.MkdirAll(filepath.Dir(fpath), os.ModePerm)

		// Проверяем, является ли файл символической ссылкой
		if f.Mode()&os.ModeSymlink != 0 {
			// Читаем цель ссылки из содержимого файла в архиве
			rc, err := f.Open()
			if err != nil {
				return err
			}
			linkTarget, err := io.ReadAll(rc)
			rc.Close()
			if err != nil {
				return err
			}
			// Создаем символическую ссылку
			return os.Symlink(string(linkTarget), fpath)
		}

		// Обычный файл
		rc, err := f.Open()
		if err != nil {
			return err
		}
		defer rc.Close()

		outFile, err := os.Create(fpath)
		if err != nil {
			return err
		}
		defer outFile.Close()

		_, err = io.Copy(outFile, rc)
		if err != nil {
			return err
		}
	}
	return nil
}

func ZipWithSymlinks(sourceDir, zipPath string) error {
	zipFile, err := os.Create(zipPath)
	if err != nil {
		return err
	}
	defer zipFile.Close()

	zipWriter := zip.NewWriter(zipFile)
	defer zipWriter.Close()

	return filepath.Walk(sourceDir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}

		// Получаем относительный путь для заголовка
		relPath, err := filepath.Rel(sourceDir, path)
		if err != nil {
			return err
		}
		if relPath == "." {
			return nil
		}

		header, err := zip.FileInfoHeader(info)
		if err != nil {
			return err
		}
		header.Name = relPath

		if info.Mode()&os.ModeSymlink != 0 {
			// Читаем цель символической ссылки
			linkTarget, err := os.Readlink(path)
			if err != nil {
				return err
			}
			// Для симлинков данные - это путь цели
			header.Method = zip.Store // не сжимаем ссылки
			writer, err := zipWriter.CreateHeader(header)
			if err != nil {
				return err
			}
			_, err = writer.Write([]byte(linkTarget))
			return err
		}

		// Обычный файл
		if !info.Mode().IsRegular() {
			return nil // игнорируем спец. файлы
		}

		file, err := os.Open(path)
		if err != nil {
			return err
		}
		defer file.Close()

		writer, err := zipWriter.CreateHeader(header)
		if err != nil {
			return err
		}
		_, err = io.Copy(writer, file)
		return err
	})
}

func CreateShortcut(target, shortcutPath, description, iconPath string) error {
	// Создаем объект ярлыка через OLE
	ole32 := walk.MustGetClass("WScript.Shell")
	defer ole32.Release()

	obj := ole32.MustCreateObject("WScript.Shell")
	defer obj.Release()

	shortcut := obj.MustQueryInterface(walk.IID_IDispatch)
	defer shortcut.Release()

	// Создаем ярлык
	scut := shortcut.MustGetProperty("CreateShortcut", shortcutPath).ToIDispatch()
	defer scut.Release()

	scut.PutProperty("TargetPath", target)
	scut.PutProperty("WorkingDirectory", filepath.Dir(target))
	if description != "" {
		scut.PutProperty("Description", description)
	}
	if iconPath != "" {
		scut.PutProperty("IconLocation", iconPath+", 0")
	}

	// Сохраняем
	scut.CallMethod("Save")
	return nil
}

// Использование
func InstallShortcuts(installDir string) error {
	exePath := filepath.Join(installDir, "myapp.exe")

	// Ярлык в меню Пуск
	startMenu := filepath.Join(os.Getenv("APPDATA"),
		"Microsoft", "Windows", "Start Menu", "Programs", "MyApp.lnk")
	CreateShortcut(exePath, startMenu, "Моя программа", exePath)

	// Ярлык на рабочем столе
	desktop := filepath.Join(os.Getenv("USERPROFILE"), "Desktop", "MyApp.lnk")
	CreateShortcut(exePath, desktop, "", exePath)

	return nil
}

// Проверка обновлений (без скачивания)
func (u *Updater) CheckUpdates() ([]string, error) {
	updates := []string{}

	for _, zipName := range u.ZipFiles {
		serverURL := u.ServerURL + "/" + zipName
		localPath := filepath.Join(u.InstallDir, "updates", zipName)

		// Получаем дату на сервере
		serverTime, err := GetFileModTime(serverURL)
		if err != nil {
			continue // пропускаем, если файл недоступен
		}

		// Проверяем локальную копию
		localInfo, err := os.Stat(localPath)
		if err == nil && localInfo.ModTime().After(serverTime) {
			continue // локальная копия новее (странно, но ок)
		}

		if err != nil || localInfo.ModTime().Before(serverTime) {
			updates = append(updates, zipName)
		}
	}

	return updates, nil
}

// Скачать и применить обновления
func (u *Updater) ApplyUpdates(updates []string, showUI bool) error {
	// Создаем бэкап текущего состояния
	backupPath := filepath.Join(u.InstallDir, "backup",
		time.Now().Format("20060102_150405")+".zip")

	if err := ZipWithSymlinks(u.InstallDir, backupPath); err != nil {
		return err // не критично, но логируем
	}

	// Для каждого обновления
	for _, zipName := range updates {
		// Скачиваем
		tempZip := filepath.Join(os.TempDir(), zipName)
		err := DownloadFile(u.ServerURL+"/"+zipName, tempZip)
		if err != nil {
			return err
		}

		// Распаковываем поверх установленной программы
		if err := UnzipWithSymlinks(tempZip, u.InstallDir); err != nil {
			return err
		}

		// Сохраняем копию обновления
		os.MkdirAll(filepath.Join(u.InstallDir, "updates"), 0755)
		os.Rename(tempZip, filepath.Join(u.InstallDir, "updates", zipName))
	}

	return nil
}

// Первая установка (если нет каталога)
func (u *Updater) FreshInstall() error {
	os.MkdirAll(u.InstallDir, 0755)

	// Скачиваем все zip-файлы и распаковываем
	for _, zipName := range u.ZipFiles {
		tempZip := filepath.Join(os.TempDir(), zipName)
		DownloadFile(u.ServerURL+"/"+zipName, tempZip)
		UnzipWithSymlinks(tempZip, u.InstallDir)

		// Сохраняем копию
		os.MkdirAll(filepath.Join(u.InstallDir, "updates"), 0755)
		os.Rename(tempZip, filepath.Join(u.InstallDir, "updates", zipName))
	}

	// Создаем ярлыки
	if err := InstallShortcuts(u.InstallDir); err != nil {
		return err
	}

	return nil
}

func LoadConfig(path string) (*Config, error) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		// Конфиг по умолчанию
		return &Config{
			CheckPeriodDays: 7,
			ZipFiles:        []string{"core.zip", "assets.zip"},
		}, nil
	}
	var cfg Config
	err = json.Unmarshal(data, &cfg)
	return &cfg, err
}
