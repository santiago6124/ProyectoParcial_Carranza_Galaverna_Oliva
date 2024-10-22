// partido.h
#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>

// Estructura que representa un partido
struct Partido {
    std::string jornada;
    std::string fecha;  
    std::string equipo_local;
    std::string equipo_visitante;
    int goles_local;
    int goles_visitante;
    std::string competicion;

    // Constructor
    Partido(const std::string& j, const std::string& f,
            const std::string& local, int golesL, int golesV,
            const std::string& visitante, const std::string& comp);
};

#endif  // PARTIDO_H
