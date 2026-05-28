# solidAddRoofBevel

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidAddRoofBevel( faceList solid, float bevelSize )`

## Описание
Добавляет на последнюю грань фигуры крышу с фаской.

Последняя грань обычно верхняя, но далеко не всегда. Например, у стакана это дно внутренней
поверхности стакана.

## Параметры
- solid исходная фигура
- bevelSize размер фаски. Если положительный, то размер крыши меньше размера последней
грани фигуры, если отрицательный - то больше.


## Пример использования

```
body = solidPlygedronInner( 2, 4, 6, true )

body = solidAddRoofBevel( body, 1 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidAddRoofBevel.png)

