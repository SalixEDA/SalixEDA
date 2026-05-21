# solidTrapezoid

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidTrapezoid( float lenghtTop, float lenghtBot, float width, float height, bool addBottom )`

## Описание
Строит призму с основанием в виде трапеции.

## Параметры
- lenghtTop - длина верхнего основания трапеции
- lenghtBot - длина нижнего основания трапеции
- width - высота трапеции
- height - высота призмы
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidTrapezoid( 2, 4, 2, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidTrapezoid.png)
