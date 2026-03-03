package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
)

const headerTemplate = `/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
`

func main() {
  // Получаем текущую директорию
	currentDir, err := os.Getwd()
	if err != nil {
		fmt.Printf("Ошибка при получении текущей директории: %v\n", err)
		os.Exit(1)
	}

	fmt.Printf("Сканирование директории: %s\n", currentDir)

	err = processDirectory(currentDir)
	if err != nil {
		fmt.Printf("Ошибка: %v\n", err)
		os.Exit(1)
	}
}



// processDirectory рекурсивно обходит директорию и обрабатывает все .h и .cpp файлы
func processDirectory(root string) error {
	var processedCount int

	err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}

		// Пропускаем директории
		if info.IsDir() {
			return nil
		}

		// Проверяем расширение файла
		ext := strings.ToLower(filepath.Ext(path))
		if ext != ".h" && ext != ".cpp" {
			return nil
		}

		// Обрабатываем файл
		modified, err := processFile(path)
		if err != nil {
			fmt.Printf("Ошибка при обработке файла %s: %v\n", path, err)
			return nil // Продолжаем обработку других файлов
		}

		if modified {
			fmt.Printf("Добавлен шаблон в файл: %s\n", path)
			processedCount++
		}

		return nil
	})

	if err != nil {
		return fmt.Errorf("ошибка при обходе директории: %w", err)
	}

	fmt.Printf("\nОбработано файлов: %d\n", processedCount)
	return nil
}



// processFile проверяет наличие комментария и добавляет информацию о копирайте
func processFile(filename string) (bool, error) {
	// Читаем файл
	data, err := ioutil.ReadFile(filename)
	if err != nil {
		return false, fmt.Errorf("не удалось прочитать файл: %w", err)
	}

	// Проверяем, что файл не пустой
	if len(data) == 0 {
		return false, nil
	}

	// Разбиваем файл на строки
	lines := bytes.Split(data, []byte("\n"))

	// Ищем начало комментария /*
	commentStartIndex := -1
	for i, line := range lines {
		trimmedLine := bytes.TrimSpace(line)
		if bytes.HasPrefix(trimmedLine, []byte("/*")) {
			commentStartIndex = i
			break
		}
	}

	// Если нет открывающего комментария, ничего не делаем
	if commentStartIndex == -1 {
		return false, nil
	}

	// Ищем строку с "Project" после начала комментария
	projectIndex := -1
	for i := commentStartIndex + 1; i < len(lines); i++ {
		trimmedLine := bytes.TrimSpace(lines[i])
		if bytes.Contains(trimmedLine, []byte("Project")) {
			projectIndex = i
			break
		}
		// Если встретили закрывающий комментарий, прекращаем поиск
		if bytes.HasPrefix(trimmedLine, []byte("*/")) {
			break
		}
	}

	// Если не нашли "Project", ничего не делаем
	if projectIndex == -1 {
		return false, nil
	}

	// Создаем шаблон для вставки
	copyrightTemplate := []byte("Copyright (c) 2026 Alexander Sibilev\n\nSPDX-License-Identifier: GPL-3.0-or-later")

	// Вставляем шаблон после строки с "Project"
	newLines := make([][]byte, 0, len(lines)+1)
	newLines = append(newLines, lines[:projectIndex+1]...)
	newLines = append(newLines, copyrightTemplate)
	newLines = append(newLines, lines[projectIndex+1:]...)

	// Объединяем строки обратно в файл
	newData := bytes.Join(newLines, []byte("\n"))

	// Записываем измененный файл
	err = ioutil.WriteFile(filename, newData, 0644)
	if err != nil {
		return false, fmt.Errorf("не удалось записать файл: %w", err)
	}

	return true, nil
}


// getFirstLine извлекает первую строку из данных
func getFirstLine(data []byte) string {
	// Ищем конец первой строки
	newline := bytes.IndexByte(data, '\n')
	if newline == -1 {
		// Если нет перевода строки, возвращаем все данные
		return string(data)
	}
	return string(data[:newline])
}

