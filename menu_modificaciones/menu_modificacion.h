// menu_modificacion.h
#ifndef MENU_MODIFICACION_H
#define MENU_MODIFICACION_H

#include <vector>
#include "../Partido/partido.h"

// Funciones para agregar, eliminar y modificar partidos
void agregarPartido();
std::vector<Partido*> filtrarPartidosPorAnio();
void eliminarPartido();
void modificarPartido();

#endif  // MENU_MODIFICACION_H
