# solidAddCurveVector

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidAddCurveVector( faceList solid, float radius, int sideCount, float x, float y, float z )`

## Описание
Продлевает тело с последней грани выполняя поворот в направлении заданного вектора.

Последняя грань обычно верхняя, но далеко не всегда. Например, у стакана это дно внутренней
поверхности стакана.

## Параметры
- solid исходная фигура
- radius радиус изгиба
- sideCount изгиб интерполируется кусочной линией. Этот параметр показывает на сколько
кусочков нужно разделить изгиб
- x,y,z направление вектора изгиба


## Пример использования

```
body = solidCylinder( 1, 5, true )

body = solidAddCurveVector( body, 3, 12, 1, 1, 0 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidAddCurveVector.png)

