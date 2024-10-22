#include "menu_modificacion.h"
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
#include <sstream>  // Para std::istringstream
#include <iomanip> // Add this line to include <memory> for std::unique_ptr
#include "../Globals/globals.h"



void agregarPartido() {
    std::string fecha, equipo_local, equipo_visitante, competicion;
    int goles_local, goles_visitante;

    // Pedir datos al usuario con validacion
    do {
        std::cout << "Ingrese la fecha (DD/MM/YYYY): ";
        std::getline(std::cin, fecha);
        if (!validarFecha(fecha)) {
            std::cout << "Fecha invalida. Intente nuevamente.\n";
        }
    } while (!validarFecha(fecha));

    std::cout << "Ingrese el equipo local: ";
    std::getline(std::cin, equipo_local);

    goles_local = ingresarEnteroPositivo("Ingrese los goles del equipo local: ");

    std::cout << "Ingrese el equipo visitante: ";
    std::getline(std::cin, equipo_visitante);

    goles_visitante = ingresarEnteroPositivo("Ingrese los goles del equipo visitante: ");

    std::cout << "Ingrese la competicion: ";
    std::getline(std::cin, competicion);

    // Crear y almacenar el nuevo partido
    auto nuevo_partido = std::make_unique<Partido>(
        "Nueva Jornada", fecha, equipo_local, goles_local, goles_visitante, equipo_visitante, competicion);
    Partido* ptr = nuevo_partido.get();
    partidos.push_back(std::move(nuevo_partido));

    // Actualizar estadisticas y mapas
    actualizarEstadisticas(equipo_local, competicion, goles_local, goles_visitante,
                           goles_local > goles_visitante, goles_local == goles_visitante);
    actualizarEstadisticas(equipo_visitante, competicion, goles_visitante, goles_local,
                           goles_visitante > goles_local, goles_local == goles_visitante);

    goles_por_competicion[competicion].emplace(goles_local + goles_visitante, ptr);
    goles_totales_por_competicion[competicion] += (goles_local + goles_visitante);

    std::cout << "Partido agregado exitosamente.\n";
}
#include <algorithm> // para std::transform



// Funcion de filtrado por año
std::vector<Partido*> filtrarPartidosPorAnio() {
    std::string competicion, equipo;
    int anio;

    // Pedir la competicion
    std::cout << "Ingrese la competicion: ";
    std::getline(std::cin, competicion);
    competicion = toLower(competicion);  // Convertir a minusculas

    // Pedir el equipo
    std::cout << "Ingrese el equipo (local o visitante): ";
    std::getline(std::cin, equipo);
    equipo = toLower(equipo);  // Convertir a minusculas

    // Pedir el año del partido
    std::cout << "Ingrese el año del partido (YYYY): ";
    std::cin >> anio;
    std::cin.ignore();  // Limpiar el buffer de entrada

    // Filtrar los partidos segun los criterios proporcionados
    std::vector<Partido*> resultados;
    for (const auto& partido_ptr : partidos) {
        Partido* partido = partido_ptr.get();

        // Convertir datos del partido a minusculas para comparacion
        std::string competicion_partido = toLower(partido->competicion);
        std::string local = toLower(partido->equipo_local);
        std::string visitante = toLower(partido->equipo_visitante);

        // Extraer el año del partido
        int anio_partido = extraerAnio(partido->fecha);

        // Verificar coincidencias
        if (competicion_partido == competicion &&
            (local == equipo || visitante == equipo) &&
            anio_partido == anio) {
            resultados.push_back(partido);
        }
    }

    return resultados;
}


void eliminarPartido() {
    std::vector<Partido*> resultados = filtrarPartidosPorAnio();

    if (resultados.empty()) {
        std::cout << "No se encontraron partidos que coincidan con los criterios.\n";
        return;
    }

    // Mostrar los partidos filtrados
    std::cout << "Partidos encontrados:\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        Partido* partido = resultados[i];
        std::cout << i + 1 << ". " << partido->equipo_local << " vs "
                  << partido->equipo_visitante << " - " << partido->fecha
                  << " (" << partido->competicion << ")\n";
    }

    // Seleccionar un partido para eliminar
    int opcion;
    do {
        std::cout << "Seleccione el numero del partido que desea eliminar: ";
        std::cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            std::cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion < 1 || opcion > static_cast<int>(resultados.size()));

    // Eliminar el partido seleccionado
    Partido* partido_eliminado = resultados[opcion - 1];
    std::string competicion = partido_eliminado->competicion;
    int total_goles = partido_eliminado->goles_local + partido_eliminado->goles_visitante;

    auto& partidos_competicion = goles_por_competicion[competicion];
    for (auto it = partidos_competicion.begin(); it != partidos_competicion.end(); ++it) {
        if (it->second == partido_eliminado) {
            partidos_competicion.erase(it);
            break;
        }
    }

    goles_totales_por_competicion[competicion] -= total_goles;

    // Eliminar de la lista principal
    partidos.erase(std::remove_if(partidos.begin(), partidos.end(),
                                  [&](const std::unique_ptr<Partido>& p) {
                                      return p.get() == partido_eliminado;
                                  }),
                   partidos.end());

    std::cout << "Partido eliminado correctamente.\n";
}


void modificarPartido() {
    std::vector<Partido*> resultados = filtrarPartidosPorAnio();

    if (resultados.empty()) {
        std::cout << "No se encontraron partidos que coincidan con los criterios.\n";
        return;
    }

    // Mostrar los partidos filtrados
    std::cout << "Partidos encontrados:\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        Partido* partido = resultados[i];
        std::cout << i + 1 << ". " << partido->equipo_local << " vs "
                  << partido->equipo_visitante << " - " << partido->fecha
                  << " (" << partido->competicion << ")\n";
    }

    // Seleccionar un partido para modificar
    int opcion;
    do {
        std::cout << "Seleccione el numero del partido que desea modificar: ";
        std::cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            std::cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion < 1 || opcion > static_cast<int>(resultados.size()));

    Partido* partido = resultados[opcion - 1];

    // Menu para modificar campos
    int campo;
    do {
        std::cout << "\nSeleccione el campo a modificar:\n";
        std::cout << "1. Fecha (" << partido->fecha << ")\n";
        std::cout << "2. Equipo local (" << partido->equipo_local << ")\n";
        std::cout << "3. Goles equipo local (" << partido->goles_local << ")\n";
        std::cout << "4. Equipo visitante (" << partido->equipo_visitante << ")\n";
        std::cout << "5. Goles equipo visitante (" << partido->goles_visitante << ")\n";
        std::cout << "6. Competicion (" << partido->competicion << ")\n";
        std::cout << "0. Guardar y salir\n";
        std::cout << "Opcion: ";
        std::cin >> campo;
        std::cin.ignore();

        switch (campo) {
            case 1: {
                std::string nueva_fecha;
                do {
                    std::cout << "Ingrese la nueva fecha (DD/MM/YYYY): ";
                    std::getline(std::cin, nueva_fecha);
                    if (!validarFecha(nueva_fecha)) {
                        std::cout << "Fecha invalida. Intente nuevamente.\n";
                    }
                } while (!validarFecha(nueva_fecha));
                partido->fecha = nueva_fecha;
                break;
            }
            case 2:
                std::cout << "Ingrese el nuevo equipo local: ";
                std::getline(std::cin, partido->equipo_local);
                break;
            case 3:
                partido->goles_local = ingresarEnteroPositivo("Ingrese los nuevos goles del equipo local: ");
                break;
            case 4:
                std::cout << "Ingrese el nuevo equipo visitante: ";
                std::getline(std::cin, partido->equipo_visitante);
                break;
            case 5:
                partido->goles_visitante = ingresarEnteroPositivo("Ingrese los nuevos goles del equipo visitante: ");
                break;
            case 6:
                std::cout << "Ingrese la nueva competicion: ";
                std::getline(std::cin, partido->competicion);
                break;
            case 0:
                std::cout << "Cambios guardados.\n";
                break;
            default:
                std::cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (campo != 0);
}