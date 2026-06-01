# modelSolidAdd

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`model modelSolidAdd( model src, color bodyColor, faceList faces )`

## Описание
Добавляет к модели дополнительное тело с произвольным цветом. Ориентация и положение
дополнительного тела в пространстве определяется моделью.

## Параметры
- src исходная модель
- bodyColor цвет тела
- faces тело, полученное одной или несколькими функциями solidXXX


## Пример использования

```
body = solidBox( 6, 4, 3, true )

md = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )

body = solidNew( flatOffset( flatCircle(1), 5, 0 ), 5, true )

partModel = modelSolidAdd( md, selectColor("#008000"), body )
```

Этот код сформирует такое изображение:

![pic](scriptFunModelSolidAdd.png)
