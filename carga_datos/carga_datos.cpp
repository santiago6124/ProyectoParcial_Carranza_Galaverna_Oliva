// carga_datos.cpp
#include "../carga_datos/carga_datos.h"
#include "../Partido/partido.h"
#include "../Utilidades/utilidades.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono> // Para medir el tiempo de ejecucion
#include "../Globals/globals.h"

using namespace std;
using namespace std::chrono;

// Cargar datos desde el archivo CSV con el formato proporcionado
void cargarDatosDesdeCSV(const string &archivo) {
    // Iniciar la medicion del tiempo de ejecucion
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    ifstream archivo_csv(archivo);

    if (!archivo_csv.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << archivo << "\n";
        contador_ifs++; // if para comprobar si el archivo esta abierto
        return;
    }

    string linea;
    // Saltar la primera linea (encabezados)
    getline(archivo_csv, linea);

    while (getline(archivo_csv, linea)) {
        istringstream ss(linea);
        string jornada, fecha, local, golesL, golesV, visitante, competicion;

        getline(ss, jornada, ',');
        getline(ss, fecha, ',');
        getline(ss, local, ',');
        getline(ss, golesL, ',');
        getline(ss, golesV, ',');
        getline(ss, visitante, ',');
        getline(ss, competicion, ',');

        // Verificar si los goles son numaricos
        if (!esNumerico(golesL) || !esNumerico(golesV)) {
            cerr << "Error: Datos invalidos en la linea: " << linea << "\n";
            contador_ifs++; // if que verifica los datos numaricos
            continue;
        }

        // Crear el partido y almacenar el puntero
        auto partido = make_unique<Partido>(
            jornada, fecha, local, stoi(golesL), stoi(golesV), visitante, competicion);
        Partido *ptr = partido.get();
        partidos.push_back(move(partido));

        int total_goles = ptr->goles_local + ptr->goles_visitante;

        // Evaluar condiciones de empate y triunfo local
        bool empate = (ptr->goles_local == ptr->goles_visitante);
        contador_ifs++; // if implicito en la comparacion de empate

        bool triunfo_local = (ptr->goles_local > ptr->goles_visitante);
        contador_ifs++; // if implicito en la comparacion de triunfo local

        actualizarEstadisticas(local, competicion, ptr->goles_local, ptr->goles_visitante,
                               triunfo_local, empate);
        actualizarEstadisticas(visitante, competicion, ptr->goles_visitante, ptr->goles_local,
                               !triunfo_local && !empate, empate);

        // Insertar en el mapa de partidos por goles y por competicion
        goles_por_competicion[competicion].emplace(total_goles, ptr);

        // Actualizar el total de goles por competicion
        goles_totales_por_competicion[competicion] += total_goles;
    }

    // Finalizar la medicion del tiempo de ejecucion
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duracion en segundos

    // Mostrar resultados de ejecucion
    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}