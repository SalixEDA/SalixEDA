# solidBoxWithCone

[Содержание](contents.htm)=>[Скрипт 3d](script3d.md)=>[Построение 3d моделей](script2Root.md)

---

## Формат вызова
`faceList solidBoxWithCone( float lenght, float width, float height, float coneHeight, bool addBottom )`

## Описание
Строит параллелепипед с конусной крышей.

## Параметры
- lenght - длина основания (X)
- width - ширина основания (Y)
- height - высота
- coneHeight - высота конусной крыши. Положительные числа формируют крышу, отрицательные
 формируют впадину
- addBottom - когда true добавляется нижняя грань, в противном случае - не добавляется

## Пример использования

```
body = solidBoxWithCone( 6, 4, 3, 2, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

Этот код сформирует такое изображение:

![pic](scriptFunSolidBoxWithCone.png)
