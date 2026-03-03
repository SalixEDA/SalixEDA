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



// processFile проверяет первую строку файла и добавляет шаблон если нужно
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

	// Проверяем первую строку
	firstLine := getFirstLine(data)
	if strings.HasPrefix(strings.TrimSpace(firstLine), "/*") {
		return false, nil // Комментарий уже есть
	}


	// Вставляем шаблон в начало файла
	newData := append([]byte(headerTemplate), data...)

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

