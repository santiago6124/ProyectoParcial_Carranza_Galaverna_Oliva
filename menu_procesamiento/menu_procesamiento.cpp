#include "menu_procesamiento.h"
#include "../Partido/partido.h"
#include "../Estadisticas/estadisticas.h"
#include "../Utilidades/utilidades.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <sstream> // Para istringstream
#include <iomanip> // Add this line to include <memory> for unique_ptr
#include "../Globals/globals.h"

void mostrarTop5PartidosTodasLasCompeticiones()
{
    cout << "Top 5 partidos con mas goles por competicion:\n";

    for (const auto &[competicion, partidos] : goles_por_competicion)
    {
        cout << "\nCompeticion: " << competicion << "\n";
        int contador = 0;

        for (auto it = partidos.rbegin(); it != partidos.rend() && contador < 5; ++it, ++contador)
        {
            const auto *partido = it->second;
            int total_goles = partido->goles_local + partido->goles_visitante;

            cout << partido->equipo_local << " vs " << partido->equipo_visitante
                 << " - " << total_goles << " goles - " << partido->fecha
                 << " - " << competicion << " - ("
                 << partido->goles_local << "-" << partido->goles_visitante << ")\n";
        }
    }
}

void mostrarGolesPorEquipoYCompeticion()
{
    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end())
    {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    cout << "Goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo])
    {
        cout << "Competicion: " << competicion << "\n";
        cout << "Goles a favor: " << stats.goles_a_favor << "\n";
        cout << "Goles en contra: " << stats.goles_en_contra << "\n";
    }
}

void mostrarPromedioGolesPorEquipoYCompeticion()
{
    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end())
    {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    cout << "Promedio de goles para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo])
    {
        int partidos = stats.triunfos + stats.derrotas + stats.empates;
        if (partidos == 0)
        {
            cout << "Competicion: " << competicion << " - Sin partidos registrados.\n";
            continue;
        }

        double promedio_favor = static_cast<double>(stats.goles_a_favor) / partidos;
        double promedio_contra = static_cast<double>(stats.goles_en_contra) / partidos;

        cout << "Competicion: " << competicion << "\n";
        cout << "Promedio goles a favor: " << promedio_favor << "\n";
        cout << "Promedio goles en contra: " << promedio_contra << "\n";
    }
}
void mostrarTriunfosYDerrotasPorEquipoYCompeticion()
{
    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end())
    {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    cout << "Triunfos y derrotas para el equipo: " << equipo << "\n";
    for (const auto &[competicion, stats] : estadisticas[equipo])
    {
        cout << "Competicion: " << competicion << "\n";
        cout << "Triunfos: " << stats.triunfos << "\n";
        cout << "Derrotas: " << stats.derrotas << "\n";
        cout << "Empates: " << stats.empates << "\n";
    }
}
void mostrarFechasConMasYMenosGolesPorEquipoYCompeticion()
{
    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    getline(cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end())
    {
        cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    cout << "Fechas con mas y menos goles para el equipo: " << equipo << "\n";

    for (const auto &[competicion, stats] : estadisticas[equipo])
    {
        const Partido *partido_mas_goles = nullptr;
        const Partido *partido_menos_goles = nullptr;
        int max_goles = -1;
        int min_goles = INT_MAX;

        for (const auto &partido_ptr : partidos)
        {
            const Partido *partido = partido_ptr.get();
            if ((partido->equipo_local == equipo || partido->equipo_visitante == equipo) &&
                partido->competicion == competicion)
            {
                int total_goles = partido->goles_local + partido->goles_visitante;

                if (total_goles > max_goles)
                {
                    max_goles = total_goles;
                    partido_mas_goles = partido;
                }
                if (total_goles < min_goles)
                {
                    min_goles = total_goles;
                    partido_menos_goles = partido;
                }
            }
        }

        cout << "Competicion: " << competicion << "\n";
        if (partido_mas_goles)
        {
            cout << "Fecha con mas goles: " << partido_mas_goles->fecha << " - "
                 << max_goles << " goles\n";
        }
        if (partido_menos_goles)
        {
            cout << "Fecha con menos goles: " << partido_menos_goles->fecha << " - "
                 << min_goles << " goles\n";
        }
    }
}
void mostrarCompeticionConMasGoles()
{
    if (goles_totales_por_competicion.empty())
    {
        cout << "No hay competiciones registradas.\n";
        return;
    }

    auto max_goles_it = max_element(
        goles_totales_por_competicion.begin(), goles_totales_por_competicion.end(),
        [](const auto &a, const auto &b)
        {
            return a.second < b.second;
        });

    cout << "La competicion con mas goles es: " << max_goles_it->first
         << " con " << max_goles_it->second << " goles.\n";
}
void mostrarEquipoConMasYMenosGoles()
{
    // Variables para almacenar los equipos con mas y menos goles a nivel global.
    string equipo_max_global, equipo_min_global;
    int max_goles_global = -1, min_goles_global = INT_MAX;

    // Mapas para almacenar los equipos con mas y menos goles por competicion.
    unordered_map<string, pair<string, string>> equipos_por_competicion;
    unordered_map<string, pair<int, int>> goles_por_competicion;

    // Recorrer cada equipo y sus estadisticas.
    for (const auto &[equipo, competiciones] : estadisticas)
    {
        int goles_total_equipo = 0;

        for (const auto &[competicion, stats] : competiciones)
        {
            // Sumar los goles del equipo en esta competicion.
            goles_total_equipo += stats.goles_a_favor;

            // Inicializar los valores de comparacion si es la primera vez.
            if (goles_por_competicion[competicion].first == 0)
            {
                goles_por_competicion[competicion] = {stats.goles_a_favor, stats.goles_a_favor};
                equipos_por_competicion[competicion] = {equipo, equipo};
            }

            // Comparar para mas y menos goles por competicion.
            if (stats.goles_a_favor > goles_por_competicion[competicion].first)
            {
                goles_por_competicion[competicion].first = stats.goles_a_favor;
                equipos_por_competicion[competicion].first = equipo;
            }
            if (stats.goles_a_favor < goles_por_competicion[competicion].second)
            {
                goles_por_competicion[competicion].second = stats.goles_a_favor;
                equipos_por_competicion[competicion].second = equipo;
            }
        }

        // Comparar para mas y menos goles a nivel global.
        if (goles_total_equipo > max_goles_global)
        {
            max_goles_global = goles_total_equipo;
            equipo_max_global = equipo;
        }
        if (goles_total_equipo < min_goles_global)
        {
            min_goles_global = goles_total_equipo;
            equipo_min_global = equipo;
        }
    }

    // Mostrar los equipos con mas y menos goles por competicion.
    cout << "Equipos con mas y menos goles por competicion:\n";
    for (const auto &[competicion, equipos] : equipos_por_competicion)
    {
        cout << "Competicion: " << competicion << "\n";
        cout << "Equipo con mas goles: " << equipos.first
             << " con " << goles_por_competicion[competicion].first << " goles\n";
        cout << "Equipo con menos goles: " << equipos.second
             << " con " << goles_por_competicion[competicion].second << " goles\n";
    }

    // Mostrar los equipos con mas y menos goles en total.
    cout << "\nEquipos con mas y menos goles en total:\n";
    cout << "Equipo con mas goles: " << equipo_max_global
         << " con " << max_goles_global << " goles\n";
    cout << "Equipo con menos goles: " << equipo_min_global
         << " con " << min_goles_global << " goles\n";
}