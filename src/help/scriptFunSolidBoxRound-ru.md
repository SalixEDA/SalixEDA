# solidBoxRound

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidBoxRound( float lenght, float width, float height, float roundRadius, float roundCount, bool addBottom )`

## Описание
Строит параллелепипед. На вертикальные ребра добавляются скругления.

## Параметры
- lenght - длина основания (X)
- width - ширина основания (Y)
- height - высота
- roundRadius - радиус скругления
- roundCount - количество ребер со скруглениями (1-4)
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidBoxRound( 6, 4, 3, 0.5, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidBoxRound.png)
