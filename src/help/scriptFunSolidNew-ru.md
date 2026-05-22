# solidNew

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidNew( flat profile, float height, bool addBottom )`

## Описание
Строит призму с основанием, заданным контуром.

## Параметры
- profile - выпуклый контур основания, формируемый одной из функций flatXXX
- height - высота призмы
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
f = flatRectangle( 6, 4 )

body = solidNew( f, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidBox.png)
