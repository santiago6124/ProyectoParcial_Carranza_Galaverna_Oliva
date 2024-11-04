// estadisticas.cpp
#include "estadisticas.h"
#include "../Globals/globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

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
}
