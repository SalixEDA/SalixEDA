# solidTubeDifColor

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidTubeDifColor( flat outProfile, flat inProfile, float height, bool addBottom, color bodyColor )`

## Описание
Строит трубу с заданными внешним и внутренним профилями, в общем случае - разными.
Задается цвет внутренней поверхности трубы.

## Параметры
- outProfile - выпуклый контур внешней поверхности трубы, формируемый одной из функций flatXXX
- inProfile - выпуклый контур внутренней поверхности трубы, формируется одной из функций
flatXXX. Внутренний профиль должен быть строго внутри внешнего
- height - высота трубы
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется
- bodyColor - цвет внутренней поверхности трубы

## Пример использования

```
f = flatRectangle( 6, 4 )
i = flatCircle( 1.5 )

body = solidTubeDifColor( f, i, 5, true,  selectColor("#008000") )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidTubeDifColor.png)

