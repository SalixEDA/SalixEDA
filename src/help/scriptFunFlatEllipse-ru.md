# flatEllipse

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`flat flatEllipse( float radiusx, float radiusy, float stepDegree )`

## Описание
Формирует 2D контур (проекцию) эллипса.

## Параметры
- radiusx - радиус эллипса по оси X
- radiusy - радиус эллипса по оси Y
- stepDegree - эллипс образуется полигоном. Этот параметр задает шаг по углу между
вершинами этого полигона

## Пример использования

```
f = flatEllipse( 10, 5, 12 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunFlatEllipse.png)
