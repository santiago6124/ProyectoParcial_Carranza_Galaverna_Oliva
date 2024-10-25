#ifndef SHELLSORT_H_
#define SHELLSORT_H_

#include <iostream>
using namespace std;
// Metodo para ordenar un arreglo de enteros utilizando el algoritmo Shell Sort.
// Parametros:
//   arr: arreglo de enteros a ordenar.
//   size: numero de elementos en el arreglo.
//
// Descripcion:
//   Shell Sort es un algoritmo de ordenamiento que generaliza el metodo de insercion.
//   Comienza ordenando elementos que estan lejos unos de otros y va reduciendo la
//   distancia entre los elementos a medida que avanza. Este enfoque permite que el
//   algoritmo se comporte de manera mas eficiente en comparacion con la ordenacion
//   por insercion simple, especialmente en arreglos mas grandes.
//
// Funcionamiento:
//   - Se define un intervalo, o "gap", que se inicializa como la mitad del tamaño del arreglo.
//   - Se realizan pasadas sobre el arreglo, y en cada pasada, se compara e intercambia elementos
//     que estan separados por el "gap".
//   - Despues de cada pasada, el "gap" se reduce, y el proceso se repite hasta que "gap" es 0,
//     momento en el cual el arreglo esta completamente ordenado.
//
// Complejidad Temporal:
//   - El peor caso: O(n²) (cuando el gap es muy grande al principio, pero las mejoras con
//     gaps mas pequeños permiten que la complejidad se acerque a O(n log n) en algunos casos).
//   - El caso promedio: O(n log n) (para arreglos aleatorios, dependiendo de la secuencia de gaps).
//   - El mejor caso: O(n) (cuando el arreglo ya esta casi ordenado).
//
// Complejidad Espacial:
//   - O(1) ya que Shell Sort es un algoritmo in-place y no requiere espacio adicional significativo.
//
// Ventajas:
//   - Shell Sort es mas rapido que otros algoritmos de ordenacion por insercion para arreglos grandes.
//   - Permite un intercambio de elementos que esta mas cerca del lugar donde deberian estar, lo que
//     mejora su eficiencia.
//
// Desventajas:
//   - La eleccion de la secuencia de gaps puede influir significativamente en la eficiencia del algoritmo.
//   - No es estable, lo que significa que no garantiza mantener el orden relativo de los elementos iguales.
//
void shellSort(int *arr, int size)
{
    // Se inicia con un gap inicial que es la mitad del tamaño del arreglo.
    for (int gap = size / 2; gap > 0; gap /= 2) // gap se reduce dividiendolo entre 2 en cada iteracion
    {
        // Recorremos el arreglo desde el indice 'gap' hasta el final.
        for (int i = gap; i < size; i += 1)
        {
            int temp = arr[i]; // Guardamos el elemento actual en 'temp'.
            int j;
            // Comparamos e intercambiamos elementos que estan separados por 'gap'.
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap]; // Desplazamos los elementos hacia la derecha.
            arr[j] = temp; // Colocamos el elemento guardado en la posicion correcta.
        }
    }
}


#endif // SHELLSORT_H_
