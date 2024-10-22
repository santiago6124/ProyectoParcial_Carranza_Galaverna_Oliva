// globals.cpp
#include "globals.h"

// Definición de las variables globales
std::unordered_map<std::string, std::unordered_map<std::string, Estadisticas>> estadisticas;
std::unordered_map<std::string, std::multimap<int, Partido*>> goles_por_competicion;
std::unordered_map<std::string, int> goles_totales_por_competicion;
std::vector<std::unique_ptr<Partido>> partidos;
