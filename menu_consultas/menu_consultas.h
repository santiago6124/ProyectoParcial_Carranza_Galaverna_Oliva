// menu_consultas.h
#ifndef MENU_CONSULTAS_H
#define MENU_CONSULTAS_H

#include <vector>
#include "../Partido/partido.h"
using namespace std;

// Declaracion de funciones de consultas dinamicas
int convertirFecha(const string &fecha);
void listarResultadosEquipoCompeticion();
void listarResultadosEquipoEntreFechas();
void compararRendimientoEntreEquipos();
void compararRendimientoParticularEntreEquipos();
void filtrarEquiposPorUmbralDeGoles();

#endif // MENU_CONSULTAS_H
