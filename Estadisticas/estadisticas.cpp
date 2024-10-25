// estadisticas.cpp
#include "estadisticas.h"
#include "../Globals/globals.h"

void actualizarEstadisticas(const string &equipo, const string &competicion,
                            int goles_a_favor, int goles_en_contra, bool triunfo, bool empate)
{
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
}
