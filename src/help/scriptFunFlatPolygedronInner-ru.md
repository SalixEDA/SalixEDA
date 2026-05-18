# flatPlygedronInner

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatPlygedronInner( float radius, float sideCount )`

## Описание
Формирует 2D контур (проекцию) правильного многоугольника заданием вписанной окружности.

## Параметры
- radius - радиус вписанной окружности (расстояние от центра до стороны)
- sideCount - количество сторон правильного многоугольника, должно быть три и более

## Пример использования

```
f = flatPlygedronInner(  5, 6 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatPlygedronInner.png)
