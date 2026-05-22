# solidTube

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidTube( flat outProfile, float thickness, float height, bool addBottom )`

## Описание
Строит трубу с заданным профилем и толщиной стенки.

## Параметры
- outProfile - выпуклый контур внешней поверхности трубы, формируемый одной из функций flatXXX
- thickness - толщина стенки трубы
- height - высота трубы
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
f = flatRectangle( 6, 4 )

body = solidTube( f, 0.5, 5, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidTube.png)

