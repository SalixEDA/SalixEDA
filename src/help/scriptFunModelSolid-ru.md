# modelSolid

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`model modelSolid( color bodyColor, faceList faces, float angX, float angY, float angZ, float offX, float offY, float offZ )`

## Описание
Создает модель на основе тела, его цвета и положения и ориентации в
пространстве.

## Параметры
- bodyColor цвет тела
- faces тело, полученное одной или несколькими функциями solidXXX
- angX, angY, angZ углы поворота соответственно вокруг осей X, Y и Z в градусах. Поворот
выполняется последовательно: сначала по X, затем - по Y и, наконец, - по Z
- offX, offY, offZ смещение тела в пространстве


## Пример использования

```
body = solidBox( 6, 4, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidBox.png)
