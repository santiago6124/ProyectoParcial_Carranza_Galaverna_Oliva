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
#include <sstream> // Para istringstream
#include <iomanip> // Add this line to include <memory> for unique_ptr
#include "../Globals/globals.h"

void agregarPartido()
{
    string fecha, equipo_local, equipo_visitante, competicion;
    int goles_local, goles_visitante;

    // Pedir datos al usuario con validacion
    do
    {
        cout << "Ingrese la fecha (DD/MM/YYYY): ";
        getline(cin, fecha);
        if (!validarFecha(fecha))
        {
            cout << "Fecha invalida. Intente nuevamente.\n";
        }
    } while (!validarFecha(fecha));

    cout << "Ingrese el equipo local: ";
    getline(cin, equipo_local);

    goles_local = ingresarEnteroPositivo("Ingrese los goles del equipo local: ");

    cout << "Ingrese el equipo visitante: ";
    getline(cin, equipo_visitante);

    goles_visitante = ingresarEnteroPositivo("Ingrese los goles del equipo visitante: ");

    cout << "Ingrese la competicion: ";
    getline(cin, competicion);

    // Crear y almacenar el nuevo partido
    auto nuevo_partido = make_unique<Partido>(
        "Nueva Jornada", fecha, equipo_local, goles_local, goles_visitante, equipo_visitante, competicion);
    Partido *ptr = nuevo_partido.get();
    partidos.push_back(move(nuevo_partido));

    // Actualizar estadisticas y mapas
    actualizarEstadisticas(equipo_local, competicion, goles_local, goles_visitante,
                           goles_local > goles_visitante, goles_local == goles_visitante);
    actualizarEstadisticas(equipo_visitante, competicion, goles_visitante, goles_local,
                           goles_visitante > goles_local, goles_local == goles_visitante);

    goles_por_competicion[competicion].emplace(goles_local + goles_visitante, ptr);
    goles_totales_por_competicion[competicion] += (goles_local + goles_visitante);

    cout << "Partido agregado exitosamente.\n";
}
#include <algorithm> // para transform

// Funcion de filtrado por año
vector<Partido *> filtrarPartidosPorAnio()
{
    string competicion, equipo;
    int anio;

    // Pedir la competicion
    cout << "Ingrese la competicion: ";
    getline(cin, competicion);
    competicion = toLower(competicion); // Convertir a minusculas

    // Pedir el equipo
    cout << "Ingrese el equipo (local o visitante): ";
    getline(cin, equipo);
    equipo = toLower(equipo); // Convertir a minusculas

    // Pedir el año del partido
    cout << "Ingrese el año del partido (YYYY): ";
    cin >> anio;
    cin.ignore(); // Limpiar el buffer de entrada

    // Filtrar los partidos segun los criterios proporcionados
    vector<Partido *> resultados;
    for (const auto &partido_ptr : partidos)
    {
        Partido *partido = partido_ptr.get();

        // Convertir datos del partido a minusculas para comparacion
        string competicion_partido = toLower(partido->competicion);
        string local = toLower(partido->equipo_local);
        string visitante = toLower(partido->equipo_visitante);

        // Extraer el año del partido
        int anio_partido = extraerAnio(partido->fecha);

        // Verificar coincidencias
        if (competicion_partido == competicion &&
            (local == equipo || visitante == equipo) &&
            anio_partido == anio)
        {
            resultados.push_back(partido);
        }
    }

    return resultados;
}
void eliminarPartido() {
    vector<Partido *> resultados = filtrarPartidosPorAnio();

    if (resultados.empty()) {
        cout << "No se encontraron partidos que coincidan con los criterios.\n";
        return;
    }

    // Mostrar los partidos filtrados
    cout << "Partidos encontrados:\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        Partido *partido = resultados[i];
        cout << i + 1 << ". " << partido->equipo_local << " vs "
             << partido->equipo_visitante << " - " << partido->fecha
             << " (" << partido->competicion << ")\n";
    }

    // Seleccionar un partido para eliminar
    int opcion;
    do {
        cout << "Seleccione el numero del partido que desea eliminar: ";
        cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion < 1 || opcion > static_cast<int>(resultados.size()));

    // Obtener el partido a eliminar
    Partido *partido_eliminado = resultados[opcion - 1];
    string competicion = partido_eliminado->competicion;
    int total_goles = partido_eliminado->goles_local + partido_eliminado->goles_visitante;

    // Eliminar del mapa de goles por competición usando un bucle con iteradores
    auto &partidos_competicion = goles_por_competicion[competicion];
    for (auto it = partidos_competicion.begin(); it != partidos_competicion.end(); ++it) {
        if (it->second == partido_eliminado) {
            partidos_competicion.erase(it);
            break;
        }
    }

    // Actualizar los goles totales
    goles_totales_por_competicion[competicion] -= total_goles;

    // Eliminar del vector global
    partidos.erase(remove_if(partidos.begin(), partidos.end(),
                             [&](const unique_ptr<Partido> &p) {
                                 return p.get() == partido_eliminado;
                             }),
                   partidos.end());

    cout << "Partido eliminado correctamente.\n";
}
void modificarPartido() {
    vector<Partido *> resultados = filtrarPartidosPorAnio();

    if (resultados.empty()) {
        cout << "No se encontraron partidos que coincidan con los criterios.\n";
        return;
    }

    // Mostrar los partidos filtrados
    cout << "Partidos encontrados:\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        Partido *partido = resultados[i];
        cout << i + 1 << ". " << partido->equipo_local << " vs "
             << partido->equipo_visitante << " - " << partido->fecha
             << " (" << partido->competicion << ")\n";
    }

    // Seleccionar un partido para modificar
    int opcion;
    do {
        cout << "Seleccione el numero del partido que desea modificar: ";
        cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion < 1 || opcion > static_cast<int>(resultados.size()));

    Partido *partido = resultados[opcion - 1];

    // Eliminar los goles antiguos del mapa
    auto &partidos_competicion = goles_por_competicion[partido->competicion];
    for (auto it = partidos_competicion.begin(); it != partidos_competicion.end(); ++it) {
        if (it->second == partido) {
            partidos_competicion.erase(it);
            break;
        }
    }

    goles_totales_por_competicion[partido->competicion] -= 
        (partido->goles_local + partido->goles_visitante);

    // Menú para modificar campos del partido
    int campo;
    do {
        cout << "\nSeleccione el campo a modificar:\n";
        cout << "1. Fecha (" << partido->fecha << ")\n";
        cout << "2. Equipo local (" << partido->equipo_local << ")\n";
        cout << "3. Goles equipo local (" << partido->goles_local << ")\n";
        cout << "4. Equipo visitante (" << partido->equipo_visitante << ")\n";
        cout << "5. Goles equipo visitante (" << partido->goles_visitante << ")\n";
        cout << "6. Competición (" << partido->competicion << ")\n";
        cout << "0. Guardar y salir\n";
        cout << "Opción: ";
        cin >> campo;
        cin.ignore();  // Limpiar el buffer de entrada

        switch (campo) {
            case 1: {
                string nueva_fecha;
                do {
                    cout << "Ingrese la nueva fecha (DD/MM/YYYY): ";
                    getline(cin, nueva_fecha);
                    if (!validarFecha(nueva_fecha)) {
                        cout << "Fecha inválida. Intente nuevamente.\n";
                    }
                } while (!validarFecha(nueva_fecha));
                partido->fecha = nueva_fecha;
                break;
            }
            case 2:
                cout << "Ingrese el nuevo equipo local: ";
                getline(cin, partido->equipo_local);
                break;
            case 3:
                partido->goles_local = ingresarEnteroPositivo(
                    "Ingrese los nuevos goles del equipo local: ");
                break;
            case 4:
                cout << "Ingrese el nuevo equipo visitante: ";
                getline(cin, partido->equipo_visitante);
                break;
            case 5:
                partido->goles_visitante = ingresarEnteroPositivo(
                    "Ingrese los nuevos goles del equipo visitante: ");
                break;
            case 6:
                cout << "Ingrese la nueva competición: ";
                getline(cin, partido->competicion);
                break;
            case 0:
                cout << "Cambios guardados.\n";
                break;
            default:
                cout << "Opción inválida. Intente nuevamente.\n";
        }
    } while (campo != 0);

    // Agregar los nuevos valores al mapa y actualizar estadísticas
    goles_por_competicion[partido->competicion].emplace(
        partido->goles_local + partido->goles_visitante, partido);

    goles_totales_por_competicion[partido->competicion] += 
        (partido->goles_local + partido->goles_visitante);

    actualizarEstadisticas(partido->equipo_local, partido->competicion, 
                           partido->goles_local, partido->goles_visitante,
                           partido->goles_local > partido->goles_visitante,
                           partido->goles_local == partido->goles_visitante);

    actualizarEstadisticas(partido->equipo_visitante, partido->competicion, 
                           partido->goles_visitante, partido->goles_local,
                           partido->goles_visitante > partido->goles_local,
                           partido->goles_local == partido->goles_visitante);

    cout << "Partido modificado exitosamente.\n";
}
