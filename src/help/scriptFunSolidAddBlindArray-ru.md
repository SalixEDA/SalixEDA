# solidAddBlindArray

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidAddBlindArray( faceList solid, flat holeProfile, float depth, float rowDistance, floatList rows[horzDist, count, horzOffset, vertOffset...] )`

## Описание
Добавляет на последнюю грань фигуры массив глухих отверстий.

Последняя грань обычно верхняя, но далеко не всегда. Например, у стакана это дно внутренней
поверхности стакана.

Массив отверстий формируется через ряды. Каждый ряд описывается в массиве описания
рядов

## Параметры
- solid исходная фигура
- holeProfile профиль одного отверстия массива. Все профили массива должны быть полностью
внутри профиля последней грани
- depth глубина отверстия. С положительными числами получается отверстие, с 
отрицательными - торчащая тонкостенная трубка
- rowDistance расстояние между рядами
- bodyColor цвет надстройки

## Блок описания ряда


## Пример использования

```
body = solidPlygedronInner( 2, 4, 6, false )

hole = flatCircle( 1.5 )

body = solidAddHoleDifColor( body, hole, 8, selectColor("#008000") )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidAddHoleDifColor.png)

