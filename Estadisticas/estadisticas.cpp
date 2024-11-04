// estadisticas.cpp
#include "estadisticas.h"
#include "../Globals/globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <map>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Variable para almacenar en caché el resultado de la competición con más goles
std::string cacheCompeticionConMasGoles;

// Función que calcula la competición con más goles y almacena el resultado en caché
void calcularCompeticionConMasGoles() {
    std::stringstream resultado;
    int contador_ifs = 0; // Contador de 'if'

    // Verificar si hay competiciones registradas
    if (estadisticas.empty()) {
        resultado << "No hay competiciones registradas.\n";
        contador_ifs++; // Conteo del 'if'
        cacheCompeticionConMasGoles = resultado.str();
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
        resultado << "La competición con más goles es: " << max_goles_it->first
                  << " con " << max_goles_it->second << " goles.\n";
        contador_ifs++; // Conteo del 'if'
    }

    // Guardar el resultado en la variable de caché
    cacheCompeticionConMasGoles = resultado.str();
}


// Variable para almacenar en caché los resultados del top 5
std::string cacheTop5Partidos;

// Función que calcula el top 5 partidos y almacena el resultado en caché
void calcularTop5PartidosTodasLasCompeticiones() {
    std::stringstream resultado;
    resultado << "Top 5 partidos con más goles por competición:\n";

    for (const auto &[competicion, partidos] : goles_por_competicion) {
        resultado << "\nCompetición: " << competicion << "\n";
        int contador = 0;

        for (auto it = partidos.rbegin(); it != partidos.rend() && contador < 5; ++it, ++contador) {
            const auto *partido = it->second;
            resultado << partido->equipo_local << " vs " << partido->equipo_visitante
                      << " - " << it->first << " goles - " << partido->fecha
                      << " - " << competicion << " - ("
                      << partido->goles_local << "-" << partido->goles_visitante << ")\n";
        }
    }

    // Guardar el resultado en la variable de caché
    cacheTop5Partidos = resultado.str();
    // cout << "Valor de cacheTop5Partidos:\n" << cacheTop5Partidos << "\n";
}


void actualizarEstadisticas(const std::string &equipo, const std::string &competicion,
                            int goles_a_favor, int goles_en_contra, bool triunfo, bool empate) {
    Estadisticas &stats = estadisticas[equipo][competicion];
    stats.goles_a_favor += goles_a_favor;
    stats.goles_en_contra += goles_en_contra;
    if (triunfo)
    {
        stats.triunfos++;
    }
    else if (empate)
    {
        stats.empates++;
    }
    else
    {
        stats.derrotas++;
    }

    // Llamar a la función para calcular el top 5 y actualizar el caché
    calcularTop5PartidosTodasLasCompeticiones();
    calcularCompeticionConMasGoles();
}
