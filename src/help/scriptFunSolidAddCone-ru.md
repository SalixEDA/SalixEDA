# solidAddCone

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidAddCone( faceList solid, float coneHeight )`

## Описание
Добавляет на последнюю грань фигуры конус.

Последняя грань обычно верхняя, но далеко не всегда. Например, у стакана это дно внутренней
поверхности стакана.

## Параметры
- solid исходная фигура
- coneHeight высота конуса. Положительные числа - конус имеет форму крыши, отрицательные
числа - конус имеет форму впадины

## Пример использования

```
body = solidPlygedronInner( 2, 4, 6, true )

body = solidAddCone( body, 3 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidAddCone.png)

