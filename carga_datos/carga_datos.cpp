// carga_datos.cpp
#include "../carga_datos/carga_datos.h"
#include "../Partido/partido.h"
#include "../Utilidades/utilidades.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include "../Globals/globals.h"


// Cargar datos desde el archivo CSV con el formato proporcionado
void cargarDatosDesdeCSV(const std::string& archivo) {
    std::ifstream archivo_csv(archivo);

    if (!archivo_csv.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << "\n";
        return;
    }

    std::string linea;
    // Saltar la primera linea (encabezados)
    getline(archivo_csv, linea);

    while (getline(archivo_csv, linea)) {
        std::istringstream ss(linea);
        std::string jornada, fecha, local, golesL, golesV, visitante, competicion;

        getline(ss, jornada, ',');
        getline(ss, fecha, ',');
        getline(ss, local, ',');
        getline(ss, golesL, ',');
        getline(ss, golesV, ',');
        getline(ss, visitante, ',');
        getline(ss, competicion, ',');

        if (!esNumerico(golesL) || !esNumerico(golesV)) {
            std::cerr << "Error: Datos invalidos en la linea: " << linea << "\n";
            continue;
        }

        // Crear el partido y almacenar el puntero
        auto partido = std::make_unique<Partido>(
            jornada, fecha, local, std::stoi(golesL), std::stoi(golesV), visitante, competicion);
        Partido* ptr = partido.get();
        partidos.push_back(std::move(partido));

        int total_goles = ptr->goles_local + ptr->goles_visitante;

        // Actualizar estadisticas para equipos y competicion
        bool empate = (ptr->goles_local == ptr->goles_visitante);
        bool triunfo_local = (ptr->goles_local > ptr->goles_visitante);

        actualizarEstadisticas(local, competicion, ptr->goles_local, ptr->goles_visitante,
                               triunfo_local, empate);
        actualizarEstadisticas(visitante, competicion, ptr->goles_visitante, ptr->goles_local,
                               !triunfo_local && !empate, empate);

        // Insertar en el mapa de partidos por goles y por competicion
        goles_por_competicion[competicion].emplace(total_goles, ptr);

        // Actualizar el total de goles por competicion
        goles_totales_por_competicion[competicion] += total_goles;
    }
}