# solidCylinderWithCone

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidCylinderWithCone( float radius, float height, float coneHeight, bool addBottom )`

## Описание
Строит цилиндр и сверху конус.

## Параметры
- radius - радиус основания
- height - высота цилиндр
- coneHeight - высота конуса. Если число положительное, то строится крыша, если отрицательное,
то строится впадина
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidCylinderWithCone( 2, 5, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidCylinderWithCone.png)
