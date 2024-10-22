// menu_consultas.h
#ifndef MENU_CONSULTAS_H
#define MENU_CONSULTAS_H

#include <vector>
#include "../Partido/partido.h"

// Declaración de funciones de consultas dinámicas
int convertirFecha(const std::string& fecha);
void listarResultadosEquipoCompeticion();
void listarResultadosEquipoEntreFechas();
void compararRendimientoEntreEquipos();
void compararRendimientoParticularEntreEquipos();
void filtrarEquiposPorUmbralDeGoles();

#endif  // MENU_CONSULTAS_H
