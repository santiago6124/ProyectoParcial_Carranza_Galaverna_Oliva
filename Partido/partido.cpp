// partido.cpp
#include "partido.h"

// Constructor del partido
Partido::Partido(const string &j, const string &f,
                 const string &local, int golesL, int golesV,
                 const string &visitante, const string &comp)
    : jornada(j), fecha(f), equipo_local(local), goles_local(golesL),
      goles_visitante(golesV), equipo_visitante(visitante), competicion(comp) {}
