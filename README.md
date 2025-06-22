## AMC Wireframe

### Author:

Anton Chumak aka [@AMChumak](https://github.com/AMChumak). Student of FIT NSU, programmer,
3D modeler, cyclist, just a nice guy!

## About program

AMC Ray Tracing - modern render machine. It allows you to create
scenes of different objects and render it. Choose your favourite and export it in png!

## Structure

The application consists of 2 parts: [3D Viewer](#3d-viewer) and [Settings](#settings).
>Notice! Click on chosen part to make focus there.

## 3D Viewer
This viewer allows you to view objects. Click on it to go in interactive mode.
> Use keys: WASD for moving camera.

> Use wheel, wheel + Shift to rotate camera

> Use wheel + Control to zoom in and zoom out

> Use Enter (try numlock enter) to go in render mode. It copies function of render mode checkbox.

## Settings

At the top of settings you can see file settings:
1. **Load scene** - loads file with extension .scene (automaticaly uses file .render with the same name)
2. **Load config** - loads config file with extension .render
3. **Save config** - saves config file
4. **Export** - exports current render in png image
5. **Init** - returns camera to init position (set in config file)
6. **About** - opens this note. So maybe you know it :)
7. **Render mode** - switches between wireframe mode and render mode. You also can use Enter for this purpose

At the bottom of settings you can see settings of scene:
1. **Background** - Choose color of background
2. **Gamma** - Sets gamma of render image (image as bright as gamma is big)
3. **Tracing depth** - This parameter limits the recursion of ray tracing that is used in render


## Scene file format
``` 
Ar Ag Ab // рассеянный свет в пространстве RGB в диапазоне 0..255
NL // число точечных источников в сцене
// далее идёт NL строк, каждая из которых описывает точечный источник света
LX LY LZ LR LG LB // LX, LY, LZ – положение источника,
// LR, LG, LB – цвет источника в пространстве RGB в диапазоне 0..255

//Далее идёт некоторое количество описаний объектов в следующем виде:
// для сферы:
SPHERE CENTERx CENTERy CENTERz // координаты центра сферы
RADIUS // радиус сферы
KDr KDg KDb KSr KSg KSb Power // оптические характеристики
// для бокса (ребра параллельны осям):
BOX MINx MINy MINz // точка с минимальными координатами
MAXx MAXy MAXz // точка с максимальными координатами
KDr KDg KDb KSr KSg KSb Power // оптические характеристики
// для треугольника:
TRIANGLE POINT1x POINT1y POINT1z // координаты первой вершины
POINT2x POINT2y POINT2z // координаты второй вершины
POINT3x POINT3y POINT3z // координаты третьей вершины
KDr KDg KDb KSr KSg KSb Power // оптические характеристики
// для четырёхугольника:
QUADRANGLE POINT1x POINT1y POINT1z // координаты 1й вершины
POINT2x POINT2y POINT2z // координаты 2й вершины
POINT3x POINT3y POINT3z // координаты 3й вершины
POINT4x POINT4y POINT4z // координаты 4й вершины
KDr KDg KDb KSr KSg KSb Power // оптические характеристики
```
>KDr, KDg, KDb – коэффициенты диффузного и рассеянного (одни и те же) отражения для
красной, зелёной и синей составляющей света.
> KSr, KSg, KSb – коэффициенты зеркального отражения для красной, зелёной и синей
составляющей света.
> Power – показатель зеркальности по Блинну (определяет размер и силу блеска)

## Config file format
```
Br Bg Bb // цвет фона в формате 0..255
GAMMA // значение гаммы
DEPTH // глубина трассировки
QUALITY // rough – грубое, normal – среднее, fine – высокое
// позиция и характеристики камеры
EYEx EYEy EYEz // Точка камеры
VIEWx VIEWy VIEWz // Точка наблюдения (это центр бокса для начальной установки камеры)
UPx UPy UPz // вектор вверх, может быть не перпендикулярен вектору (EYE, VIEW)
ZN ZF // ближняя и дальняя граница видимости камеры
SW SH // ширина и высота матрицы камеры (через неё пускаются лучи),
// матрица расположение на расстояние ZN от точки EYE по направлению на точку VIEW
```

