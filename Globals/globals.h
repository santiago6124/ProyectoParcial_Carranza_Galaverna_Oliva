// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include "../Partido/partido.h"
#include "../Estadisticas/estadisticas.h"

// Declaración de variables globales como extern
extern std::unordered_map<std::string, std::unordered_map<std::string, Estadisticas>> estadisticas;
extern std::unordered_map<std::string, std::multimap<int, Partido*>> goles_por_competicion;
extern std::unordered_map<std::string, int> goles_totales_por_competicion;
extern std::vector<std::unique_ptr<Partido>> partidos;

#endif  // GLOBALS_H
