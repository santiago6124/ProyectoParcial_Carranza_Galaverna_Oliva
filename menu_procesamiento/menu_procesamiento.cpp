#include "menu_procesamiento.h"
#include "../Partido/partido.h"
#include "../Estadisticas/estadisticas.h"
#include "../Utilidades/utilidades.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <sstream> // Para istringstream
#include <iomanip> // Add this line to include <memory> for unique_ptr
#include "../Globals/globals.h"
#include <chrono> // Para medir el tiempo de ejecucion


using namespace std::chrono;


// Función principal que muestra el resultado almacenado en caché
void mostrarTop5PartidosTodasLasCompeticiones() {
    if (!cacheTop5Partidos.empty()) {
        std::cout << cacheTop5Partidos;
    } else {
        std::cout << "No se ha calculado el top 5 aún.\n";
    }
}


void mostrarGolesPorEquipoYCompeticion() {
    // Iniciar la medicion del tiempo de ejecucion


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadisticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo esta en el mapa
        return;
    }

    cout << "Goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        cout << "Competicion: " << competicion << "\n";
        cout << "Goles a favor: " << stats.goles_a_favor << "\n";
        cout << "Goles en contra: " << stats.goles_en_contra << "\n";
    }

    // Finalizar la medicion del tiempo de ejecucion
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duracion en segundos

    // Mostrar resultados de ejecucion
    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}

void mostrarPromedioGolesPorEquipoYCompeticion() {
    // Iniciar la medicion del tiempo de ejecucion


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadisticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo esta en el mapa
        return;
    }

    cout << "Promedio de goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        int partidos = stats.triunfos + stats.derrotas + stats.empates;

        // Verificar si el equipo tiene partidos registrados
        if (partidos == 0) {
            cout << "Competicion: " << competicion << " - Sin partidos registrados.\n";
            contador_ifs++; // Conteo del 'if' para verificar si hay partidos
            continue;
        }

        double promedio_favor = static_cast<double>(stats.goles_a_favor) / partidos;
        double promedio_contra = static_cast<double>(stats.goles_en_contra) / partidos;

        cout << "Competicion: " << competicion << "\n";
        cout << "Promedio goles a favor: " << promedio_favor << "\n";
        cout << "Promedio goles en contra: " << promedio_contra << "\n";
    }

    // Finalizar la medicion del tiempo de ejecucion
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duracion en segundos

    // Mostrar resultados de ejecucion
    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void mostrarTriunfosYDerrotasPorEquipoYCompeticion() {
    // Iniciar la medicion del tiempo de ejecucion


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadisticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo esta en el mapa
        return;
    }

    cout << "Triunfos y derrotas para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        cout << "Competicion: " << competicion << "\n";
        cout << "Triunfos: " << stats.triunfos << "\n";
        cout << "Derrotas: " << stats.derrotas << "\n";
        cout << "Empates: " << stats.empates << "\n";
    }

    // Finalizar la medicion del tiempo de ejecucion
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duracion en segundos

    // Mostrar resultados de ejecucion
    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void mostrarFechasConMasYMenosGolesPorEquipoYCompeticion() {
    // Iniciar la medicion del tiempo de ejecucion


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadisticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++;  // Conteo del 'if'
        return;
    }

    cout << "Fechas con mas y menos goles para el equipo: " << equipo << "\n";

    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        const Partido *partido_mas_goles = nullptr;
        const Partido *partido_menos_goles = nullptr;
        int max_goles = -1;
        int min_goles = INT_MAX;

        for (const auto &partido_ptr : partidos) {
            const Partido *partido = partido_ptr.get();
            // Verificar si el partido corresponde al equipo y competicion
            if ((partido->equipo_local == equipo || partido->equipo_visitante == equipo) &&
                partido->competicion == competicion) {
                contador_ifs++;  // Conteo del 'if' que verifica condiciones del partido

                int total_goles = partido->goles_local + partido->goles_visitante;

                // Actualizar el partido con mas goles
                if (total_goles > max_goles) {
                    max_goles = total_goles;
                    partido_mas_goles = partido;
                    contador_ifs++;  // Conteo del 'if' para actualizar max_goles
                }

                // Actualizar el partido con menos goles
                if (total_goles < min_goles) {
                    min_goles = total_goles;
                    partido_menos_goles = partido;
                    contador_ifs++;  // Conteo del 'if' para actualizar min_goles
                }
            }
        }

        cout << "Competicion: " << competicion << "\n";
        if (partido_mas_goles) {
            cout << "Fecha con mas goles: " << partido_mas_goles->fecha << " - "
                 << max_goles << " goles\n";
            contador_ifs++;  // Conteo del 'if' para verificar partido_mas_goles
        }
        if (partido_menos_goles) {
            cout << "Fecha con menos goles: " << partido_menos_goles->fecha << " - "
                 << min_goles << " goles\n";
            contador_ifs++;  // Conteo del 'if' para verificar partido_menos_goles
        }
    }

    // Finalizar la medicion del tiempo de ejecucion
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio);  // Duracion en segundos

    // Mostrar resultados de ejecucion
    cout << "\nTiempo de ejecucion: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}   

// Función para mostrar el resultado desde la caché
void mostrarCompeticionConMasGoles() {
    if (!cacheCompeticionConMasGoles.empty()) {
        std::cout << cacheCompeticionConMasGoles;
    } else {
        std::cout << "No se ha calculado la competición con más goles aún.\n";
    }
}

void mostrarEquipoConMasYMenosGoles() {
    if (!cacheEquipoConMasYMenosGoles.empty()) {
        std::cout << cacheEquipoConMasYMenosGoles;
    } else {
        std::cout << "No se ha calculado el equipo con más y menos goles aún.\n";
    }
}
