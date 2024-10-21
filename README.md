# Analisis del proyecto
## Conceptos DSA del curso
1. Recursividad
2. Listas
3. Pilas
4. Colas
5. Arboles
6. Grafos
7. Hash
8. Ordenamiento
9. Busqueda

## Requerimientos
### 1. Eficiencia
Conmesurar la eficiencia del codigo. Contar cantidad de condicionales y tiempo de ejecucion. Mostrar dichos calculos por pantalla, indicando proceso, algortimo usado y estructura usada. 
```
Ej. Procesamiento de datos resultados principales1 - Tiempo 1ms - Cantidad de if: 30 -
estructuras y/o algoritmos usados. 
```

### 2. Librerias a disposición
Librerías internas del curso (/Librerias)
Cualquier Librería de C++ (en caso de usar una librería externa al curso, defender su implementacion)

### 3. Menu
Navegacion del Usuario en las diferentes funcionalidades

### 4. Carga de datos desde archivo CSV
Archivo csv contiene filas de partidos con los siguientes datos:
* Jornada
* Fecha
* Equipo Local
* Goles Local
* Goles Visitante
* Equipo Visitante
* Competicion

### 5. Procesamiento de datos durante lectura archivo csv
#### a.  Mayor Cantidad de Goles
```
Primeros 5 partidos con mayor cantidad de goles de cada competición.De haber una misma cantidad de goles en diferentes partidos: tomar fecha mas reciente, de haber similitud de fechas, utilizar un criterio a eleccion.
```
#### b. GF y GC de cada Equipo por Competicion
```
 Usuario pedirá que se muestren por pantalla de un equipo por vez y discriminando por competicion
```
#### c. Promedio de GF y GC por Competicion
```
 Usuario pedirá que se muestren por pantalla de un equipo por vez y discriminando por competicion
```
#### d. D y V de cada Equipo por Competicion
```
 Usuario pedirá que se muestren por pantalla de un equipo por vez y discriminando por competicion
```
#### e. Fecha con mas goles y Fecha con menos goles por Competicion
```
 Usuario pedirá que se muestren por pantalla de un equipo por vez y discriminando por competicion
```
#### f. Competicion con mas goles convertidos
#### g. Equipo con mas GF y Equipo con menos GF de Todas las Competiciones y por Competicion

### 6. Modificacion de Datos y Procesamiento
Usuario podrá
* Agregar un partido
* Eliminar un partido completo
* Modificar un partido
El punto 5 debera ejecutarse automaticamente cada vez que se haga una modificacion

### 7. Consultar Dinamicas
* Todos los Resultados de un equipo en una competicion especifica
* Resultado de un equipo entre dos fechas
* Comparacion de Rendimiento General(GF y GC) entre dos equipos
* Comparacion de rendimiento particular de dos equipos: enfrentamiento entre dichos equipos - partidos jugados, Victorias, Empates
* Umbral de goles convertidos: Programa debera filtrar a los quipos que hayan convertidos mas o menos goles que dicho Umbral
