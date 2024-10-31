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
using namespace std;

// Declaracion de variables globales como extern
extern unordered_map<string, unordered_map<string, Estadisticas>> estadisticas;
extern unordered_map<string, multimap<int, Partido *>> goles_por_competicion;
extern unordered_map<string, int> goles_totales_por_competicion;
extern vector<unique_ptr<Partido>> partidos;


#endif // GLOBALS_H
