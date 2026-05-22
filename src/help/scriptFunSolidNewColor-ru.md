# solidNewColor

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidNewColor( flat profile, float height, bool addBottom, color bodyColor )`

## Описание
Строит призму с основанием, заданным контуром.

## Параметры
- profile - выпуклый контур основания, формируемый одной из функций flatXXX
- height - высота призмы
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется
- bodyColor - цвет граней призмы

## Пример использования

```
f = flatRectangle( 6, 4 )

body = solidNewColor( f, 3, true, selectColor("#008000") )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidNewColor.png)

Здесь зеленый цвет, заданный в функции solidNewColor "перебивает" цвет, задаваемый
при создании модели.