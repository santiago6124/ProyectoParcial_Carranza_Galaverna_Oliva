#include "menu_consultas.h"
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
#include <sstream> // Para std::istringstream
#include <iomanip> // Add this line to include <memory> for std::unique_ptr
#include "../Globals/globals.h"

// Consultas Dinamicas
//  Funcion auxiliar: Convertir fecha DD/MM/YYYY a un entero YYYYMMDD para comparacion
int convertirFecha(const std::string &fecha)
{
    std::istringstream ss(fecha);
    std::string dia, mes, anio;
    getline(ss, dia, '/');
    getline(ss, mes, '/');
    getline(ss, anio);

    // Convertir al formato YYYYMMDD
    return std::stoi(anio + mes + dia);
}
void listarResultadosEquipoCompeticion()
{
    std::string equipo, competicion;

    // Pedir el equipo y la competicion al usuario
    std::cout << "Ingrese el equipo: ";
    std::getline(std::cin, equipo);
    std::cout << "Ingrese la competicion: ";
    std::getline(std::cin, competicion);

    equipo = toLower(equipo);           // Convertir a minusculas para comparacion
    competicion = toLower(competicion); // Convertir a minusculas para comparacion

    bool encontrados = false;

    // Listar los resultados del equipo en la competicion especifica
    for (const auto &partido_ptr : partidos)
    {
        Partido *partido = partido_ptr.get();
        std::string equipo_local = toLower(partido->equipo_local);
        std::string equipo_visitante = toLower(partido->equipo_visitante);
        std::string competicion_partido = toLower(partido->competicion);

        // Verificar si el equipo esta en este partido y coincide la competicion
        if (competicion_partido == competicion &&
            (equipo_local == equipo || equipo_visitante == equipo))
        {
            encontrados = true;

            // Formatear la salida correctamente segun el equipo que jugo
            std::cout << partido->fecha << " - ";

            if (equipo_local == equipo)
            {
                std::cout << partido->equipo_local << " " << partido->goles_local << " a "
                          << partido->goles_visitante << " ";

                if (partido->goles_local > partido->goles_visitante)
                {
                    std::cout << "gano a " << partido->equipo_visitante << ".\n";
                }
                else if (partido->goles_local < partido->goles_visitante)
                {
                    std::cout << "perdio con " << partido->equipo_visitante << ".\n";
                }
                else
                {
                    std::cout << "empato con " << partido->equipo_visitante << ".\n";
                }
            }
            else
            {
                std::cout << partido->equipo_visitante << " " << partido->goles_visitante
                          << " a " << partido->goles_local << " ";

                if (partido->goles_visitante > partido->goles_local)
                {
                    std::cout << "gano a " << partido->equipo_local << ".\n";
                }
                else if (partido->goles_visitante < partido->goles_local)
                {
                    std::cout << "perdio con " << partido->equipo_local << ".\n";
                }
                else
                {
                    std::cout << "empato con " << partido->equipo_local << ".\n";
                }
            }
        }
    }

    if (!encontrados)
    {
        std::cout << "No se encontraron partidos para el equipo " << equipo
                  << " en la competicion " << competicion << ".\n";
    }
}

void listarResultadosEquipoEntreFechas()
{
    std::string equipo, fecha_inicio, fecha_fin;

    // Pedir el equipo y las fechas al usuario
    std::cout << "Ingrese el equipo: ";
    std::getline(std::cin, equipo);

    do
    {
        std::cout << "Ingrese la fecha de inicio (DD/MM/YYYY): ";
        std::getline(std::cin, fecha_inicio);
        if (!validarFecha(fecha_inicio))
        {
            std::cout << "Fecha invalida. Intente nuevamente.\n";
        }
    } while (!validarFecha(fecha_inicio));

    do
    {
        std::cout << "Ingrese la fecha de fin (DD/MM/YYYY): ";
        std::getline(std::cin, fecha_fin);
        if (!validarFecha(fecha_fin))
        {
            std::cout << "Fecha invalida. Intente nuevamente.\n";
        }
    } while (!validarFecha(fecha_fin));

    equipo = toLower(equipo); // Convertir a minusculas para comparacion

    int fecha_inicio_int = convertirFecha(fecha_inicio);
    int fecha_fin_int = convertirFecha(fecha_fin);

    bool encontrados = false;

    // Buscar partidos del equipo entre las fechas especificadas
    for (const auto &partido_ptr : partidos)
    {
        Partido *partido = partido_ptr.get();
        std::string equipo_local = toLower(partido->equipo_local);
        std::string equipo_visitante = toLower(partido->equipo_visitante);
        std::string fecha_partido = partido->fecha;

        int fecha_partido_int = convertirFecha(fecha_partido);

        // Verificar si el equipo participo y la fecha esta en el rango
        if ((equipo_local == equipo || equipo_visitante == equipo) &&
            fecha_partido_int >= fecha_inicio_int && fecha_partido_int <= fecha_fin_int)
        {
            encontrados = true;

            // Imprimir el resultado del partido
            std::cout << partido->fecha << " - ";
            if (equipo_local == equipo)
            {
                std::cout << partido->equipo_local << " " << partido->goles_local << " a "
                          << partido->goles_visitante << " ";

                if (partido->goles_local > partido->goles_visitante)
                {
                    std::cout << "gano a " << partido->equipo_visitante;
                }
                else if (partido->goles_local < partido->goles_visitante)
                {
                    std::cout << "perdio con " << partido->equipo_visitante;
                }
                else
                {
                    std::cout << "empato con " << partido->equipo_visitante;
                }
            }
            else
            {
                std::cout << partido->equipo_visitante << " " << partido->goles_visitante
                          << " a " << partido->goles_local << " ";

                if (partido->goles_visitante > partido->goles_local)
                {
                    std::cout << "gano a " << partido->equipo_local;
                }
                else if (partido->goles_visitante < partido->goles_local)
                {
                    std::cout << "perdio con " << partido->equipo_local;
                }
                else
                {
                    std::cout << "empato con " << partido->equipo_local;
                }
            }
            std::cout << " en " << partido->competicion << ".\n";
        }
    }

    if (!encontrados)
    {
        std::cout << "No se encontraron partidos para " << equipo
                  << " entre " << fecha_inicio << " y " << fecha_fin << ".\n";
    }
}
void compararRendimientoEntreEquipos()
{
    std::string equipo1, equipo2;

    // Pedir los nombres de los equipos al usuario
    std::cout << "Ingrese el primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Convertir los nombres a minusculas para comparacion
    std::string equipo1_lower = toLower(equipo1);
    std::string equipo2_lower = toLower(equipo2);

    bool equipo1_encontrado = false, equipo2_encontrado = false;

    std::cout << "\nComparacion de rendimiento:\n";

    // Recorrer los equipos en las estadisticas
    for (const auto &[equipo, competiciones] : estadisticas)
    {
        std::string equipo_lower = toLower(equipo); // Convertir para comparacion

        // Verificar si el equipo es uno de los ingresados
        if (equipo_lower == equipo1_lower || equipo_lower == equipo2_lower)
        {
            for (const auto &[competicion, stats] : competiciones)
            {
                std::cout << equipo << " - " << competicion << ":\n";
                std::cout << "  Goles a favor: " << stats.goles_a_favor << "\n";
                std::cout << "  Goles en contra: " << stats.goles_en_contra << "\n";
            }

            if (equipo_lower == equipo1_lower)
                equipo1_encontrado = true;
            if (equipo_lower == equipo2_lower)
                equipo2_encontrado = true;
        }
    }

    // Mostrar mensajes de error si no se encontraron datos
    if (!equipo1_encontrado)
    {
        std::cout << "No se encontraron datos para el equipo: " << equipo1 << "\n";
    }
    if (!equipo2_encontrado)
    {
        std::cout << "No se encontraron datos para el equipo: " << equipo2 << "\n";
    }
}

void compararRendimientoParticularEntreEquipos()
{
    std::string equipo1, equipo2;

    // Pedir los nombres de los equipos al usuario
    std::cout << "Ingrese el primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Convertir los nombres a minusculas para comparacion
    std::string equipo1_lower = toLower(equipo1);
    std::string equipo2_lower = toLower(equipo2);

    int partidos_jugados = 0;
    int empates = 0;
    int victorias_equipo1 = 0;
    int victorias_equipo2 = 0;

    // Recorrer todos los partidos
    for (const auto &partido_ptr : partidos)
    {
        Partido *partido = partido_ptr.get();
        std::string local = toLower(partido->equipo_local);
        std::string visitante = toLower(partido->equipo_visitante);

        // Verificar si ambos equipos participaron en el mismo partido
        if ((local == equipo1_lower && visitante == equipo2_lower) ||
            (local == equipo2_lower && visitante == equipo1_lower))
        {
            partidos_jugados++;

            // Verificar el resultado del partido
            if (partido->goles_local == partido->goles_visitante)
            {
                empates++;
            }
            else if ((local == equipo1_lower && partido->goles_local > partido->goles_visitante) ||
                     (visitante == equipo1_lower && partido->goles_visitante > partido->goles_local))
            {
                victorias_equipo1++;
            }
            else
            {
                victorias_equipo2++;
            }
        }
    }

    // Mostrar los resultados
    std::cout << "\nComparacion de rendimiento particular entre " << equipo1 << " y " << equipo2 << ":\n";
    std::cout << "Partidos jugados entre ellos: " << partidos_jugados << "\n";
    std::cout << "Empates: " << empates << "\n";

    if (victorias_equipo1 > victorias_equipo2)
    {
        std::cout << equipo1 << " ha ganado mas partidos: " << victorias_equipo1 << "\n";
    }
    else if (victorias_equipo2 > victorias_equipo1)
    {
        std::cout << equipo2 << " ha ganado mas partidos: " << victorias_equipo2 << "\n";
    }
    else
    {
        std::cout << "Ambos equipos tienen la misma cantidad de victorias.\n";
    }
}

void filtrarEquiposPorUmbralDeGoles()
{
    double umbral;
    char opcion;

    // Pedir el umbral y la opcion al usuario
    std::cout << "Ingrese el umbral de goles promedio por partido: ";
    std::cin >> umbral;

    do
    {
        std::cout << "¿Desea filtrar por encima ('A') o por debajo ('B') del umbral? (A/B): ";
        std::cin >> opcion;
        opcion = std::toupper(opcion); // Convertir a mayusculas para comparacion
    } while (opcion != 'A' && opcion != 'B');

    std::cin.ignore(); // Limpiar el buffer de entrada

    std::cout << "\nEquipos con promedio de goles ";
    if (opcion == 'A')
    {
        std::cout << "por encima de " << umbral << ":\n";
    }
    else
    {
        std::cout << "por debajo de " << umbral << ":\n";
    }

    bool encontrado = false;

    // Recorrer las estadisticas de todos los equipos y competiciones
    for (const auto &[equipo, competiciones] : estadisticas)
    {
        for (const auto &[competicion, stats] : competiciones)
        {
            int partidos_jugados = stats.triunfos + stats.derrotas + stats.empates;

            // Evitar divisiones por cero
            if (partidos_jugados == 0)
                continue;

            // Calcular el promedio de goles por partido
            double promedio_goles = static_cast<double>(stats.goles_a_favor) / partidos_jugados;

            // Verificar si el equipo cumple con el criterio del umbral
            if ((opcion == 'A' && promedio_goles >= umbral) ||
                (opcion == 'B' && promedio_goles <= umbral))
            {
                std::cout << equipo << " " << promedio_goles
                          << " goles promedio por partido en " << competicion << "\n";
                encontrado = true;
            }
        }
    }

    if (!encontrado)
    {
        std::cout << "No se encontraron equipos que cumplan con el criterio.\n";
    }
}
