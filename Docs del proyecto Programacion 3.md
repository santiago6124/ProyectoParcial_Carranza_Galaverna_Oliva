### **Funciones de Modificación en `menu_modificacion.cpp`**

#### **1\. Agregar Partido**

* **Descripción**: Esta función permite al usuario ingresar un nuevo partido a la base de datos, proporcionando detalles como fecha, equipo local, equipo visitante, goles de cada equipo y la competición.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** (`std::vector<Partido>`) \- Este vector almacena todos los partidos. Cada vez que se agrega un partido, se inserta un nuevo objeto `Partido` en este vector.  
* **Fuente de información**: Los datos ingresados por el usuario (fecha, equipos, goles, competición) se recogen a través de la entrada de consola.  
* **Destino de la información**:  
  * El nuevo partido se añade al **vector de partidos**, incrementando el tamaño de la base de datos.  
  * Además, se actualizan las estadísticas en los **mapas** que registran el rendimiento de cada equipo por competición.  
* **Proceso posterior**: Una vez agregado el partido, la función llama a un procedimiento de **reprocesamiento de estadísticas**, el cual actualiza:  
  * **Goles totales y promedios** (a favor y en contra) para cada equipo en cada competición.  
  * **Número de triunfos y derrotas** de los equipos implicados.  
  * Esta actualización permite que las estadísticas reflejen el partido recién agregado sin errores.

#### **2\. Eliminar Partido**

* **Descripción**: Permite al usuario seleccionar un partido específico y eliminarlo de la base de datos, removiéndolo del vector de partidos.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** (`std::vector<Partido>`) \- El partido se elimina de este vector mediante la función de eliminación.  
* **Fuente de información**: La identificación del partido a eliminar se toma del vector de partidos, utilizando los detalles de cada partido (fecha, equipos) que el usuario selecciona.  
* **Destino de la información**:  
  * El partido se elimina permanentemente del **vector de partidos**.  
  * Las estadísticas asociadas al partido eliminado se **actualizan en los mapas** para eliminar la contribución de ese partido a los totales.  
* **Proceso posterior**: Después de la eliminación, se inicia un **reprocesamiento de estadísticas** que recorre nuevamente el vector de partidos, recalculando todas las métricas afectadas, como:  
  * **Promedios de goles**.  
  * **Conteos de victorias, derrotas y empates**.  
  * **Ranking de fechas con mayor o menor cantidad de goles**, ajustándose si el partido eliminado estaba en alguno de esos extremos.

#### **3\. Modificar Partido**

* **Descripción**: Permite al usuario modificar cualquiera de los campos de un partido existente (como equipos, goles, fecha o competición).  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** (`std::vector<Partido>`) \- El vector se actualiza con el nuevo valor de los campos modificados.  
* **Fuente de información**: Los datos originales del partido se obtienen del vector de partidos, y los nuevos valores se ingresan a través de la consola por el usuario.  
* **Destino de la información**:  
  * Los cambios se guardan en el **vector de partidos**, sustituyendo los valores antiguos con los nuevos.  
  * Las estadísticas de rendimiento y resultados también se ajustan en los **mapas**, considerando los nuevos valores introducidos.  
* **Proceso posterior**: La función lanza el **reprocesamiento de estadísticas** tras la modificación, que incluye:  
  * **Actualización de goles** a favor y en contra de los equipos involucrados en la competición específica.  
  * **Reevaluación de fechas y competiciones** si el partido modificado cambia alguno de estos criterios.  
  * **Ajustes de victorias, empates y derrotas** si la modificación afecta los resultados del partido.

---

### **Detalles adicionales**

1. **Reprocesamiento Automático de Estadísticas**  
   * Cada vez que se realiza una modificación (agregar, eliminar o modificar), se activa un proceso de reprocesamiento. Este proceso:  
     * **Recalcula goles y resultados** en mapas que categorizan a los equipos por competición, permitiendo que las consultas reflejen los datos actualizados inmediatamente.  
     * **Actualiza listas ordenadas de partidos** con más y menos goles si el partido agregado, eliminado o modificado afecta estos rangos.  
   * Este reprocesamiento se realiza iterando sobre el vector completo de partidos, asegurando que cada consulta futura obtenga datos precisos.  
2. **Ventajas de las Estructuras de Datos Seleccionadas**  
   * **Vector de Partidos**: La elección de un vector permite la modificación dinámica y el acceso directo a elementos específicos por índice, lo cual es óptimo para el almacenamiento de datos de partidos.  
   * **Mapas para Estadísticas**: Los mapas permiten búsquedas y actualizaciones rápidas por equipo y competición, garantizando que el reprocesamiento sea eficiente.

### **Funciones de Consultas en `menu_consultas.cpp`**

#### **1\. Consultar Todos los Resultados de un Equipo en una Competición Específica**

* **Descripción**: Esta consulta muestra todos los resultados de un equipo específico en una competición específica, basándose en los datos actuales de la base de datos.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** (`std::vector<Partido>`) \- Almacena todos los partidos registrados. La función recorre este vector para encontrar los partidos del equipo en la competición especificada.  
* **Fuente de información**: Se obtiene del vector de partidos; para cada partido, se verifica que el equipo y la competición coincidan con los parámetros ingresados por el usuario.  
* **Destino de la información**: Los resultados se muestran directamente en la consola, sin modificar estructuras de datos.  
* **Proceso**: La función filtra el vector de partidos para los encuentros donde el equipo y la competición coinciden con los ingresados. Luego, los resultados de cada partido se imprimen, mostrando la fecha, el marcador y el resultado (victoria, derrota o empate).

#### **2\. Consultar Resultados de un Equipo entre Dos Fechas**

* **Descripción**: Permite obtener todos los resultados de un equipo en un rango específico de fechas, mostrando todos los encuentros de dicho equipo dentro del intervalo.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** \- La función recorre este vector, buscando los partidos que cumplan con el rango de fechas y el equipo especificado.  
* **Fuente de información**: Todos los partidos registrados se obtienen del vector, y la función filtra aquellos en los que el equipo coincide y la fecha del partido está dentro del rango.  
* **Destino de la información**: La salida se envía a la consola, detallando cada partido dentro del rango especificado.  
* **Proceso**:  
  * Se convierte la fecha de cada partido y las fechas ingresadas por el usuario a un formato comparable.  
  * La función verifica si el equipo participó en cada partido dentro de ese rango y muestra el resultado (ganó, perdió, empató) junto con el marcador y la fecha.

#### **3\. Comparación de Rendimiento General entre Dos Equipos**

* **Descripción**: Compara la cantidad de goles a favor y en contra de dos equipos en todas las competiciones en las que han participado, sin necesidad de que se hayan enfrentado directamente.  
* **Estructura de datos utilizada**:  
  * **Mapas de Estadísticas** \- Se utilizan mapas que almacenan estadísticas acumuladas de goles a favor y en contra para cada equipo y competición.  
* **Fuente de información**: Los datos se obtienen directamente del mapa de estadísticas, que ha sido actualizado previamente por otras funciones de procesamiento o modificación.  
* **Destino de la información**: La información se presenta en la consola, comparando los resultados de ambos equipos.  
* **Proceso**:  
  * La función accede al mapa de estadísticas para ambos equipos en cada competición.  
  * Imprime los goles a favor y en contra en cada competición, permitiendo comparar el rendimiento general de ambos equipos.

#### **4\. Comparación de Rendimiento Particular entre Dos Equipos (Solo Partidos Jugados entre Ellos)**

* **Descripción**: Evalúa el rendimiento específico entre dos equipos en los partidos en los que se han enfrentado directamente, contando victorias, derrotas, empates y mostrando quién tiene mejor rendimiento.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** \- Se recorre el vector para encontrar todos los enfrentamientos directos entre los dos equipos.  
* **Fuente de información**: La información proviene del vector de partidos, seleccionando solo aquellos en los que ambos equipos son el equipo local o visitante.  
* **Destino de la información**: La consola muestra la cantidad de partidos jugados, empates y la cantidad de victorias de cada equipo.  
* **Proceso**:  
  * La función filtra el vector para obtener los partidos donde ambos equipos participaron.  
  * Se cuenta cada resultado (victoria de uno, del otro o empate) y se presenta un resumen de los enfrentamientos.

#### **5\. Filtrar Equipos según Umbral de Goles Promedio por Partido**

* **Descripción**: Filtra equipos en todas las competiciones que tengan un promedio de goles por partido superior o inferior a un umbral especificado.  
* **Estructura de datos utilizada**:  
  * **Mapas de Estadísticas** \- Los promedios de goles por equipo y competición se calculan previamente y se almacenan en un mapa.  
* **Fuente de información**: El mapa de estadísticas de promedio de goles de cada equipo por competición se consulta para filtrar equipos que cumplan con el umbral.  
* **Destino de la información**: La salida se presenta en la consola, mostrando los equipos que cumplen con el criterio de promedio de goles en cada competición.  
* **Proceso**:  
  * Se consulta el mapa para cada equipo y competición, verificando si el promedio cumple con el umbral ingresado (por encima o por debajo).  
  * Los equipos que cumplan con el criterio se imprimen con su promedio de goles y la competición en la que participan.

---

### **Consideraciones Generales y Ventajas de las Estructuras de Datos**

1. **Vector de Partidos**:  
   * Ideal para el almacenamiento y recorrido secuencial de partidos, permitiendo consultas basadas en filtros específicos (por equipo, competición, fechas).  
   * La flexibilidad del vector permite actualizar la lista completa sin reorganizar la estructura, facilitando consultas rápidas.  
2. **Mapas de Estadísticas**:  
   * Perfectos para mantener estadísticas agregadas y de fácil acceso por equipo y competición.  
   * Permiten consultas rápidas de información acumulada y calculada, como promedio de goles, totales de victorias y derrotas.  
3. **Rendimiento y Eficiencia**:  
   * El uso combinado de vectores y mapas permite obtener tanto listas de datos detallados como datos agregados de forma eficiente.  
   * Cada consulta accede a estructuras preprocesadas, lo que reduce el tiempo de respuesta, manteniendo la precisión en los datos presentados.

Esta documentación específica proporciona una visión detallada del flujo de datos en cada consulta en `menu_consultas.cpp`, garantizando claridad sobre cómo se obtienen, filtran y presentan los resultados al usuario.

### **Funciones de Procesamiento en `menu_procesamiento.cpp`**

#### **1\. Procesar los Primeros 5 Partidos con Mayor Cantidad de Goles por Competición**

* **Descripción**: Esta función identifica los cinco partidos con mayor cantidad de goles (suma de goles de ambos equipos) en cada competición.  
* **Estructura de datos utilizada**:  
  * **Vector de Partidos** (`std::vector<Partido>`) \- La función recorre este vector para contar los goles en cada partido y selecciona los de mayor cantidad de goles.  
  * **Mapas para Ranking de Competición** (`std::map<std::string, std::vector<Partido>>`) \- Almacena los cinco partidos con más goles para cada competición.  
* **Fuente de información**: Los datos se obtienen del vector de partidos, extrayendo la competición y el total de goles para cada uno.  
* **Destino de la información**: El resultado se almacena en un mapa que guarda los cinco partidos con mayor puntaje para cada competición, permitiendo un acceso rápido a estos partidos.  
* **Proceso**:  
  * La función recorre el vector de partidos, calcula la suma de goles de cada partido y ordena los partidos por competición.  
  * Se mantiene solo el top 5 de partidos con mayor cantidad de goles en cada competición.  
  * En caso de empate, se considera la fecha y otros criterios predefinidos para decidir la inclusión.

#### **2\. Procesar Goles Totales a Favor y en Contra de Cada Equipo por Competición**

* **Descripción**: Calcula y almacena los goles totales a favor y en contra de cada equipo en cada competición.  
* **Estructura de datos utilizada**:  
  * **Mapas de Estadísticas por Competición** (`std::map<std::string, std::map<std::string, EstadisticasEquipo>>`) \- Donde cada equipo tiene estadísticas de goles a favor y en contra en cada competición.  
* **Fuente de información**: Los datos de goles se extraen del vector de partidos.  
* **Destino de la información**: Los goles totales a favor y en contra se almacenan en el mapa de estadísticas de equipos por competición.  
* **Proceso**:  
  * La función recorre cada partido en el vector, sumando los goles a favor para el equipo local y visitante, y asignando estos valores al mapa de estadísticas.  
  * Los datos almacenados en el mapa permiten acceder rápidamente a las estadísticas agregadas por equipo y competición.

#### **3\. Calcular el Promedio de Goles a Favor y en Contra de Cada Equipo por Competición**

* **Descripción**: Calcula el promedio de goles a favor y en contra para cada equipo en todas las competiciones en las que participa.  
* **Estructura de datos utilizada**:  
  * **Mapas de Estadísticas por Competición** \- Estos mapas también almacenan el conteo de partidos jugados para cada equipo, facilitando el cálculo de promedios.  
* **Fuente de información**: Los datos de goles de cada partido, almacenados en el vector de partidos, y el conteo de partidos jugados.  
* **Destino de la información**: Los promedios de goles se guardan en el mismo mapa de estadísticas de cada equipo y competición.  
* **Proceso**:  
  * Para cada partido en el vector, se suman los goles a favor y en contra y se incrementa el contador de partidos jugados.  
  * Finalmente, el promedio se calcula dividiendo los goles totales por el número de partidos, almacenando los resultados en el mapa de estadísticas para accesos futuros.

#### **4\. Contar Total de Derrotas y Victorias de Cada Equipo por Competición**

* **Descripción**: Cuenta las victorias y derrotas de cada equipo en cada competición.  
* **Estructura de datos utilizada**:  
  * **Mapas de Resultados de Equipos** \- Se usa un mapa para llevar un registro de victorias y derrotas de cada equipo en cada competición.  
* **Fuente de información**: Los datos de cada partido se obtienen del vector, evaluando el marcador para determinar el ganador y perdedor.  
* **Destino de la información**: El conteo de victorias y derrotas de cada equipo se almacena en el mapa de estadísticas.  
* **Proceso**:  
  * Para cada partido, se determina el ganador (si no hubo empate) y se incrementa el conteo de victorias del equipo correspondiente y derrotas del equipo perdedor.  
  * Estos datos se actualizan en el mapa de estadísticas, quedando listos para ser consultados.

#### **5\. Determinar la Fecha con Más y Menos Goles de Cada Equipo por Competición**

* **Descripción**: Identifica la fecha en la que un equipo específico anotó el mayor y menor número de goles en una competición.  
* **Estructura de datos utilizada**:  
  * **Mapas de Estadísticas por Fecha y Competición** \- Almacena las fechas con la cantidad de goles para cada equipo.  
* **Fuente de información**: Se obtiene del vector de partidos, revisando los goles anotados en cada fecha y competición.  
* **Destino de la información**: La fecha con más y menos goles se almacena en el mapa de estadísticas para cada equipo y competición.  
* **Proceso**:  
  * La función revisa cada partido del equipo y actualiza la fecha de mayor y menor goles, permitiendo un acceso directo a esta información en futuras consultas.

#### **6\. Competición con Mayor Cantidad de Goles Convertidos**

* **Descripción**: Determina la competición con el mayor número de goles en total.  
* **Estructura de datos utilizada**:  
  * **Mapas de Conteo de Goles por Competición** \- Cada competición se registra con su total de goles acumulados.  
* **Fuente de información**: Los datos de goles por partido se obtienen del vector de partidos.  
* **Destino de la información**: El total de goles por competición se almacena en un mapa.  
* **Proceso**:  
  * Se suman los goles de cada partido a la competición correspondiente en el mapa.  
  * Una vez procesado el vector, la función identifica la competición con el valor máximo en el mapa de goles acumulados.

#### **7\. Equipos con Mayor y Menor Cantidad de Goles por Competición**

* **Descripción**: Identifica el equipo con más y el equipo con menos goles en cada competición.  
* **Estructura de datos utilizada**:  
  * **Mapas de Goles Totales por Equipo y Competición** \- Almacena el total de goles para cada equipo en cada competición.  
* **Fuente de información**: La información se obtiene del vector de partidos y se acumula por equipo y competición.  
* **Destino de la información**: Los resultados se guardan en un mapa, con el equipo de mayor y menor puntaje para cada competición.  
* **Proceso**:  
  * La función recorre el vector de partidos, actualizando el total de goles para cada equipo en el mapa.  
  * Luego se identifica el equipo con el valor máximo y mínimo de goles en cada competición y se guarda en el mapa.

---

### **Consideraciones Generales y Ventajas de las Estructuras de Datos**

1. **Vector de Partidos**:  
   * Permite un recorrido secuencial eficiente de todos los partidos, fundamental para procesar datos como conteos de goles, victorias y derrotas.  
2. **Mapas de Estadísticas y Resultados**:  
   * Los mapas permiten un acceso rápido a estadísticas específicas de equipos y competiciones, lo cual es clave para optimizar el rendimiento en consultas frecuentes y filtrar datos por competición o equipo.  
3. **Eficiencia del Procesamiento**:  
   * La combinación de vectores y mapas permite dividir la lógica de procesamiento y almacenamiento, lo que facilita el mantenimiento de los datos y minimiza el costo computacional de búsquedas y cálculos repetitivos.

