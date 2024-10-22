// estadisticas.h
#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <string>
#include <unordered_map>

struct Estadisticas {
    int goles_a_favor = 0;
    int goles_en_contra = 0;
    int triunfos = 0;
    int derrotas = 0;
    int empates = 0;
};

void actualizarEstadisticas(const std::string& equipo, const std::string& competicion,
                            int goles_a_favor, int goles_en_contra, bool triunfo, bool empate);

#endif  // ESTADISTICAS_H
