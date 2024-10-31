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
#include <chrono> // Para medir el tiempo de ejecución


using namespace std::chrono;



void mostrarTop5PartidosTodasLasCompeticiones() {
    cout << "Top 5 partidos con más goles por competición:\n";

    // Iniciar la medición del tiempo de ejecución
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    for (const auto &[competicion, partidos] : goles_por_competicion) {
        cout << "\nCompetición: " << competicion << "\n";
        int contador = 0;

        // Iterar en reversa para obtener los partidos con más goles (ya están ordenados)
        for (auto it = partidos.rbegin(); it != partidos.rend() && contador < 5; ++it, ++contador) {
            contador_ifs++;  // Condición de límite de top 5

            const auto *partido = it->second;
            cout << partido->equipo_local << " vs " << partido->equipo_visitante
                 << " - " << it->first << " goles - " << partido->fecha
                 << " - " << competicion << " - ("
                 << partido->goles_local << "-" << partido->goles_visitante << ")\n";
        }
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}


void mostrarGolesPorEquipoYCompeticion() {
    // Iniciar la medición del tiempo de ejecución


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadísticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo está en el mapa
        return;
    }

    cout << "Goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        cout << "Competición: " << competicion << "\n";
        cout << "Goles a favor: " << stats.goles_a_favor << "\n";
        cout << "Goles en contra: " << stats.goles_en_contra << "\n";
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void mostrarPromedioGolesPorEquipoYCompeticion() {
    // Iniciar la medición del tiempo de ejecución


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadísticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo está en el mapa
        return;
    }

    cout << "Promedio de goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        int partidos = stats.triunfos + stats.derrotas + stats.empates;

        // Verificar si el equipo tiene partidos registrados
        if (partidos == 0) {
            cout << "Competición: " << competicion << " - Sin partidos registrados.\n";
            contador_ifs++; // Conteo del 'if' para verificar si hay partidos
            continue;
        }

        double promedio_favor = static_cast<double>(stats.goles_a_favor) / partidos;
        double promedio_contra = static_cast<double>(stats.goles_en_contra) / partidos;

        cout << "Competición: " << competicion << "\n";
        cout << "Promedio goles a favor: " << promedio_favor << "\n";
        cout << "Promedio goles en contra: " << promedio_contra << "\n";
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void mostrarTriunfosYDerrotasPorEquipoYCompeticion() {
    // Iniciar la medición del tiempo de ejecución


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadísticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++; // Conteo del 'if' para verificar si el equipo está en el mapa
        return;
    }

    cout << "Triunfos y derrotas para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        cout << "Competición: " << competicion << "\n";
        cout << "Triunfos: " << stats.triunfos << "\n";
        cout << "Derrotas: " << stats.derrotas << "\n";
        cout << "Empates: " << stats.empates << "\n";
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void mostrarFechasConMasYMenosGolesPorEquipoYCompeticion() {
    // Iniciar la medición del tiempo de ejecución


    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'
    // Verificar si el equipo tiene estadísticas
    if (estadisticas.find(equipo) == estadisticas.end()) {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        contador_ifs++;  // Conteo del 'if'
        return;
    }

    cout << "Fechas con más y menos goles para el equipo: " << equipo << "\n";

    for (const auto &[competicion, stats] : estadisticas[equipo]) {
        const Partido *partido_mas_goles = nullptr;
        const Partido *partido_menos_goles = nullptr;
        int max_goles = -1;
        int min_goles = INT_MAX;

        for (const auto &partido_ptr : partidos) {
            const Partido *partido = partido_ptr.get();
            // Verificar si el partido corresponde al equipo y competición
            if ((partido->equipo_local == equipo || partido->equipo_visitante == equipo) &&
                partido->competicion == competicion) {
                contador_ifs++;  // Conteo del 'if' que verifica condiciones del partido

                int total_goles = partido->goles_local + partido->goles_visitante;

                // Actualizar el partido con más goles
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

        cout << "Competición: " << competicion << "\n";
        if (partido_mas_goles) {
            cout << "Fecha con más goles: " << partido_mas_goles->fecha << " - "
                 << max_goles << " goles\n";
            contador_ifs++;  // Conteo del 'if' para verificar partido_mas_goles
        }
        if (partido_menos_goles) {
            cout << "Fecha con menos goles: " << partido_menos_goles->fecha << " - "
                 << min_goles << " goles\n";
            contador_ifs++;  // Conteo del 'if' para verificar partido_menos_goles
        }
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio);  // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}   
void mostrarCompeticionConMasGoles() {
    // Iniciar la medición del tiempo de ejecución
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    // Verificar si hay competiciones registradas
    if (estadisticas.empty()) {
        cout << "No hay competiciones registradas.\n";
        contador_ifs++; // Conteo del 'if'
        return;
    }

    // Mapa para acumular goles totales por competición
    unordered_map<string, int> golesPorCompeticion;

    // Recorrer las estadísticas globales
    for (const auto &[equipo, competiciones] : estadisticas) {
        for (const auto &[competicion, stats] : competiciones) {
            golesPorCompeticion[competicion] += stats.goles_a_favor;
        }
    }

    // Encontrar la competición con más goles
    auto max_goles_it = max_element(
        golesPorCompeticion.begin(), golesPorCompeticion.end(),
        [](const auto &a, const auto &b) {
            return a.second < b.second;
        });

    // Verificar si se encontró una competición con goles
    if (max_goles_it != golesPorCompeticion.end()) {
        cout << "La competición con más goles es: " << max_goles_it->first
             << " con " << max_goles_it->second << " goles.\n";
        contador_ifs++; // Conteo del 'if'
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}

void mostrarEquipoConMasYMenosGoles() {
    // Iniciar la medición del tiempo de ejecución
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0;  // Contador de 'if'

    // Variables para almacenar los equipos con más y menos goles a nivel global
    string equipo_max_global, equipo_min_global;
    int max_goles_global = -1, min_goles_global = INT_MAX;

    // Mapas para almacenar los equipos con más y menos goles por competición
    unordered_map<string, pair<string, string>> equipos_por_competicion;
    unordered_map<string, pair<int, int>> goles_por_competicion;

    // Recorrer cada equipo y sus estadísticas
    for (const auto &[equipo, competiciones] : estadisticas) {
        int goles_total_equipo = 0;

        for (const auto &[competicion, stats] : competiciones) {
            // Sumar los goles del equipo en esta competición
            goles_total_equipo += stats.goles_a_favor;

            // Inicializar los valores de comparación si es la primera vez
            if (goles_por_competicion[competicion].first == 0) {
                goles_por_competicion[competicion] = {stats.goles_a_favor, stats.goles_a_favor};
                equipos_por_competicion[competicion] = {equipo, equipo};
                contador_ifs++;  // Conteo del 'if' para la inicialización
            }

            // Comparar para más goles por competición
            if (stats.goles_a_favor > goles_por_competicion[competicion].first) {
                goles_por_competicion[competicion].first = stats.goles_a_favor;
                equipos_por_competicion[competicion].first = equipo;
                contador_ifs++;  // Conteo del 'if' para más goles
            }

            // Comparar para menos goles por competición
            if (stats.goles_a_favor < goles_por_competicion[competicion].second) {
                goles_por_competicion[competicion].second = stats.goles_a_favor;
                equipos_por_competicion[competicion].second = equipo;
                contador_ifs++;  // Conteo del 'if' para menos goles
            }
        }

        // Comparar para más goles a nivel global
        if (goles_total_equipo > max_goles_global) {
            max_goles_global = goles_total_equipo;
            equipo_max_global = equipo;
            contador_ifs++;  // Conteo del 'if' para más goles globales
        }

        // Comparar para menos goles a nivel global
        if (goles_total_equipo < min_goles_global) {
            min_goles_global = goles_total_equipo;
            equipo_min_global = equipo;
            contador_ifs++;  // Conteo del 'if' para menos goles globales
        }
    }

    // Mostrar los equipos con más y menos goles por competición
    cout << "Equipos con más y menos goles por competición:\n";
    for (const auto &[competicion, equipos] : equipos_por_competicion) {
        cout << "Competición: " << competicion << "\n";
        cout << "Equipo con más goles: " << equipos.first
             << " con " << goles_por_competicion[competicion].first << " goles\n";
        cout << "Equipo con menos goles: " << equipos.second
             << " con " << goles_por_competicion[competicion].second << " goles\n";
    }

    // Mostrar los equipos con más y menos goles en total
    cout << "\nEquipos con más y menos goles en total:\n";
    cout << "Equipo con más goles: " << equipo_max_global
         << " con " << max_goles_global << " goles\n";
    cout << "Equipo con menos goles: " << equipo_min_global
         << " con " << min_goles_global << " goles\n";

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio);  // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}