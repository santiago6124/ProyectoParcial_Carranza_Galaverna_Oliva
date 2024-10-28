// globals.cpp
#include "globals.h"

// Definicion de las variables globales
unordered_map<string, unordered_map<string, Estadisticas>> estadisticas;
unordered_map<string, multimap<int, Partido *>> goles_por_competicion;
unordered_map<string, int> goles_totales_por_competicion;
vector<unique_ptr<Partido>> partidos;
std::unordered_map<std::string, std::vector<Partido>> partidosPorCompeticion;
