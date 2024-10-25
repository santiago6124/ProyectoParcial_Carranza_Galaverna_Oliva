#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <iostream>
using namespace std;
// Metodo para ordenar un arreglo de enteros utilizando el algoritmo QuickSort.
// Parametros:
//   arr: arreglo de enteros a ordenar.
//   inicio: indice inicial del subarreglo a ordenar.
//   fin: indice final del subarreglo a ordenar.
//
// Descripcion:
//   QuickSort es un algoritmo de ordenamiento eficiente basado en el principio de divide y venceras.
//   Funciona seleccionando un elemento como pivote y reorganizando el arreglo de tal manera que todos
//   los elementos menores que el pivote se colocan a su izquierda y los mayores a su derecha. Luego,
//   se aplica el mismo proceso recursivamente a las sublistas de la izquierda y la derecha del pivote.
//
// Funcionamiento:
//   - Se selecciona el elemento en la posicion media como pivote.
//   - Se utilizan dos indices, 'i' y 'j', que comienzan en los extremos del subarreglo y se mueven hacia
//     el centro. 'i' se mueve hacia la derecha buscando elementos mayores al pivote, mientras que 'j' se
//     mueve hacia la izquierda buscando elementos menores.
//   - Cuando se encuentran dos elementos fuera de lugar (uno mayor que el pivote en la izquierda y otro
//     menor en la derecha), se intercambian. El proceso continua hasta que los indices 'i' y 'j' se cruzan.
//   - Despues del intercambio, QuickSort se llama recursivamente para ordenar las dos mitades del arreglo.
//
// Complejidad Temporal:
//   - Peor caso: O(n²), cuando el pivote es el menor o mayor elemento en cada particion, lo que puede
//     suceder si el arreglo ya esta ordenado o en orden inverso.
//   - Caso promedio: O(n log n), la complejidad esperada en la mayoria de los casos cuando el pivote
//     divide el arreglo de manera razonablemente equilibrada.
//   - Mejor caso: O(n log n), ocurre cuando el pivote divide el arreglo en partes iguales o casi iguales.
//
// Complejidad Espacial:
//   - O(log n), ya que QuickSort es un algoritmo in-place y utiliza espacio adicional en la pila para las
//     llamadas recursivas. En el peor caso, el uso de espacio puede ser O(n).
//
// Ventajas:
//   - QuickSort es uno de los algoritmos mas rapidos en la practica para ordenar grandes arreglos.
//   - Es un algoritmo in-place, lo que significa que no requiere memoria adicional significativa.
//
// Desventajas:
//   - El rendimiento puede degradarse a O(n²) si no se implementa una buena estrategia para la eleccion
//     del pivote.
//   - El uso de recursion puede ser una limitacion en sistemas con pilas limitadas.
//
// Recomendaciones:
//   - Para evitar el peor caso, se recomienda utilizar una estrategia mejorada de seleccion del pivote,
//     como la "mediana de tres" o un pivote aleatorio.
//
void quickSort(int *arr, int inicio, int fin)
{
    int i, j, medio;
    int pivot, aux;

    medio = (inicio + fin) / 2;
    pivot = arr[medio]; // Se elige el pivote como el elemento medio
    i = inicio;
    j = fin;

    do
    {
        // Encuentra el primer elemento que deberia ir a la derecha del pivote
        while (arr[i] < pivot) i++;
        // Encuentra el primer elemento que deberia ir a la izquierda del pivote
        while (arr[j] > pivot) j--;

        // Intercambia los elementos si estan en posiciones incorrectas
        if (i <= j)
        {
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
            j--;
        }
    } while (i <= j);

    // Ordena recursivamente las subporciones izquierda y derecha
    if (inicio < j)
        quickSort(arr, inicio, j); // Subarreglo izquierdo
    if (i < fin)
        quickSort(arr, i, fin);     // Subarreglo derecho
}




#endif // QUICKSORT_H_
