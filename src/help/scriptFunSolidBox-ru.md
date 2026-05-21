# solidBox

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidBox( float lenght, float width, float height, bool addBottom )`

## Описание
Строит параллелепипед.

## Параметры
- lenght - длина основания (X)
- width - ширина основания (Y)
- height - высота
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidBox( 6, 4, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidBox.png)
