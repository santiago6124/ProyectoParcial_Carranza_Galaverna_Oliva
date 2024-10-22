// partido.cpp
#include "partido.h"

// Constructor del partido
Partido::Partido(const std::string& j, const std::string& f,
                 const std::string& local, int golesL, int golesV,
                 const std::string& visitante, const std::string& comp)
    : jornada(j), fecha(f), equipo_local(local), goles_local(golesL),
      goles_visitante(golesV), equipo_visitante(visitante), competicion(comp) {}
