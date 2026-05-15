# flatRectangleBevel

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatRectangleBevel( float lenght, float width, float bevelSize, float count )`

## Описание
Формирует 2D контур (проекцию) прямоугольника со скошенными углами. Размер скоса и
количество скошенных углов - задаются.

## Параметры
- lenght - длина прямоугольника (X)
- width - ширина прямоугольника (Y)
- bevelSize - размер скоса
- count - количество скошенных углов 1-4

## Пример использования

```
f = flatRectangleBevel( 10, 5, 1, 2 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatRectangleBevel.png)
