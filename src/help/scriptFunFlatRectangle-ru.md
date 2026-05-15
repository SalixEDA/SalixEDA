# flatRectangle

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatRectangle( float lenght, float width )`

## Описание
Формирует 2D контур (проекцию) прямоугольника.

## Параметры
- lenght - длина прямоугольника (X)
- width - ширина прямоугольника (Y)

## Пример использования

```
f = flatRectangle( 10, 5 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatRect.png)