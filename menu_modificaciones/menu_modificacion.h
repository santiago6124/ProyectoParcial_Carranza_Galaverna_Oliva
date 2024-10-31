// menu_modificacion.h
#ifndef MENU_MODIFICACION_H
#define MENU_MODIFICACION_H

#include <vector>
#include "../Partido/partido.h"
using namespace std;

// Funciones para agregar, eliminar y modificar partidos
void agregarPartido();
pair<vector<Partido *>, double> filtrarPartidosPorAnio();
void eliminarPartido();
void modificarPartido();

#endif // MENU_MODIFICACION_H
