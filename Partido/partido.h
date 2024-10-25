// partido.h
#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
using namespace std;

// Estructura que representa un partido
struct Partido
{
    string jornada;
    string fecha;
    string equipo_local;
    string equipo_visitante;
    int goles_local;
    int goles_visitante;
    string competicion;

    // Constructor
    Partido(const string &j, const string &f,
            const string &local, int golesL, int golesV,
            const string &visitante, const string &comp);
};

#endif // PARTIDO_H
