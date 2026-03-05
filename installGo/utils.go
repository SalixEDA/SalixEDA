// utils.go
package main

import (
  "fmt"
  "os"
  "path/filepath"
  "runtime"
)

/*

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

*/




// InstallShortcuts создает ярлыки в нужных местах для текущей ОС
func InstallShortcuts(installPath string) error {
  exeName := ApplicationName
  if runtime.GOOS == "windows" {
    exeName += ".exe"
  }
  targetPath := filepath.Join(installPath, exeName)

  // Проверяем, существует ли исполняемый файл
  if _, err := os.Stat(targetPath); err != nil {
    return fmt.Errorf("executable not found: %s", targetPath)
  }

  // Путь к иконке (если есть)
  iconPath := filepath.Join(installPath, "icon.png")
  if _, err := os.Stat(iconPath); err != nil {
    iconPath = "" // иконки нет, используем стандартную
  }

  switch runtime.GOOS {
  case "windows":
    // Ярлык в меню Пуск
    startMenu := filepath.Join(os.Getenv("APPDATA"),
      "Microsoft", "Windows", "Start Menu", "Programs",
      CompanyName, ApplicationName+".lnk")

    if err := CreateShortcut(targetPath, startMenu, ApplicationName+" Application", iconPath); err != nil {
      return err
    }

    // Ярлык на рабочем столе
    desktop := filepath.Join(os.Getenv("USERPROFILE"), "Desktop", ApplicationName+".lnk")
    return CreateShortcut(targetPath, desktop, "", iconPath)

  case "linux":
    // Ярлык в меню приложений (системный)
    systemMenu := filepath.Join(os.Getenv("HOME"), ".local", "share", "applications",
      CompanyName+"-"+ApplicationName+".desktop")

    if err := CreateShortcut(targetPath, systemMenu, ApplicationName+" Application", iconPath); err != nil {
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


