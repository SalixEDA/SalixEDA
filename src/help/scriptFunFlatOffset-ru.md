# flatOffset

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatOffset( flat profile, float offsetX, float offsetY )`

## Описание
Все функции формирования контуров выполняют построения относительно центра. Поэтому
геометрический центр фигуры будет в точке 0,0. Данная функция позволяет сдвинуть центр
построения контура, сместив его на плоскости.

## Параметры
- profile - исходный контур
- offsetX - смещение по оси X
- offsetY - смещение по оси Y

## Пример использования

```
f = flatCircle( 5 )
f = flatOffset( f, 5, -2 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatOffset.png)
