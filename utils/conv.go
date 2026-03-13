package main

import (
    "fmt"
    "os"
)

func main() {
    // Имена файлов задаем прямо здесь
    inputFile := "DejaVuSans.ttf"
    outputFile := "sans.h"

    // Читаем весь входной файл
    data, _ := os.ReadFile(inputFile)

    // Создаем выходной файл
    out, _ := os.Create(outputFile)
    defer out.Close()

    // Пишем заголовок массива
    fmt.Fprintf(out, "unsigned char data[] = {\n")

    // Выводим байты по 16 в строке
    for i := 0; i < len(data); i++ {
        if i%16 == 0 {
            fmt.Fprintf(out, "    ")
        }

        fmt.Fprintf(out, "0x%02x", data[i])

        if i != len(data)-1 {
            fmt.Fprintf(out, ", ")
        }

        if i%16 == 15 || i == len(data)-1 {
            fmt.Fprintf(out, "\n")
        }
    }

    // Закрываем массив
    fmt.Fprintf(out, "};\n")
    fmt.Fprintf(out, "unsigned int data_len = %d;\n", len(data))
}

