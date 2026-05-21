# solidCylinder

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidCylinder( float radius, float height, bool addBottom )`

## Описание
Строит цилиндр.

## Параметры
- radius - радиус основания
- height - высота цилиндра
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidCylinder( 2, 5, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidCylinder.png)
