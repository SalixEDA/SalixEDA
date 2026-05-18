# flatTrapezoid

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatTrapezoid( float lenghtTop, float lenghtBot, float width )`

## Описание
Формирует 2D контур (проекцию) трапеции.

## Параметры
- lenghtTop - длина верхнего основания
- lenghtBot - длина нижнего основания
- width - высота трапеции

## Пример использования

```
f = flatTrapezoid(  3, 6, 2 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatTrapezoid.png)
