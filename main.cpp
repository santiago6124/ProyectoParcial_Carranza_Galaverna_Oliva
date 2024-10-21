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


// Estructura de un partido
struct Partido {
    std::string jornada;
    std::string fecha;  // Formato DD/MM/YYYY
    std::string equipo_local;
    std::string equipo_visitante;
    int goles_local;
    int goles_visitante;
    std::string competicion;

    Partido(const std::string& jornada, const std::string& fecha,
            const std::string& local, int golesL, int golesV,
            const std::string& visitante, const std::string& competicion)
        : jornada(jornada), fecha(fecha), equipo_local(local),
          goles_local(golesL), goles_visitante(golesV),
          equipo_visitante(visitante), competicion(competicion) {}
};

// Estructura para estadísticas por equipo y competición
struct Estadisticas {
    int goles_a_favor = 0;
    int goles_en_contra = 0;
    int triunfos = 0;
    int derrotas = 0;
    int empates = 0;
};

// Mapas de índices secundarios y estadísticas
std::unordered_map<std::string, std::unordered_map<std::string, Estadisticas>> estadisticas;
std::unordered_map<std::string, std::multimap<int, Partido*>> goles_por_competicion;
std::unordered_map<std::string, int> goles_totales_por_competicion;
std::vector<std::unique_ptr<Partido>> partidos;

// Verificar si una cadena es numérica
bool esNumerico(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Extraer el año desde una fecha en formato DD/MM/YYYY
int extraerAño(const std::string& fecha) {
    return std::stoi(fecha.substr(fecha.length() - 4));
}

// Actualizar estadísticas de un equipo
void actualizarEstadisticas(const std::string& equipo, const std::string& competicion,
                            int goles_a_favor, int goles_en_contra, bool triunfo, bool empate) {
    Estadisticas& stats = estadisticas[equipo][competicion];
    stats.goles_a_favor += goles_a_favor;
    stats.goles_en_contra += goles_en_contra;
    if (triunfo) {
        stats.triunfos++;
    } else if (empate) {
        stats.empates++;
    } else {
        stats.derrotas++;
    }
}

// Cargar datos desde el archivo CSV con el formato proporcionado
void cargarDatosDesdeCSV(const std::string& archivo) {
    std::ifstream archivo_csv(archivo);

    if (!archivo_csv.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << "\n";
        return;
    }

    std::string linea;
    // Saltar la primera línea (encabezados)
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
            std::cerr << "Error: Datos inválidos en la línea: " << linea << "\n";
            continue;
        }

        // Crear el partido y almacenar el puntero
        auto partido = std::make_unique<Partido>(
            jornada, fecha, local, std::stoi(golesL), std::stoi(golesV), visitante, competicion);
        Partido* ptr = partido.get();
        partidos.push_back(std::move(partido));

        int total_goles = ptr->goles_local + ptr->goles_visitante;

        // Actualizar estadísticas para equipos y competición
        bool empate = (ptr->goles_local == ptr->goles_visitante);
        bool triunfo_local = (ptr->goles_local > ptr->goles_visitante);

        actualizarEstadisticas(local, competicion, ptr->goles_local, ptr->goles_visitante,
                               triunfo_local, empate);
        actualizarEstadisticas(visitante, competicion, ptr->goles_visitante, ptr->goles_local,
                               !triunfo_local && !empate, empate);

        // Insertar en el mapa de partidos por goles y por competición
        goles_por_competicion[competicion].emplace(total_goles, ptr);

        // Actualizar el total de goles por competición
        goles_totales_por_competicion[competicion] += total_goles;
    }
}
void mostrarTop5PartidosTodasLasCompeticiones() {
    std::cout << "Top 5 partidos con más goles por competición:\n";

    for (const auto& [competicion, partidos] : goles_por_competicion) {
        std::cout << "\nCompetición: " << competicion << "\n";
        int contador = 0;

        for (auto it = partidos.rbegin(); it != partidos.rend() && contador < 5; ++it, ++contador) {
            const auto* partido = it->second;
            int total_goles = partido->goles_local + partido->goles_visitante;

            std::cout << partido->equipo_local << " vs " << partido->equipo_visitante
                      << " - " << total_goles << " goles - " << partido->fecha
                      << " - " << competicion << " - ("
                      << partido->goles_local << "-" << partido->goles_visitante << ")\n";
        }
    }
}


void mostrarGolesPorEquipoYCompeticion() {
    std::string equipo;
    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end()) {
        std::cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    std::cout << "Goles para el equipo: " << equipo << "\n";
    for (const auto& [competicion, stats] : estadisticas[equipo]) {
        std::cout << "Competición: " << competicion << "\n";
        std::cout << "Goles a favor: " << stats.goles_a_favor << "\n";
        std::cout << "Goles en contra: " << stats.goles_en_contra << "\n";
    }
}

void mostrarPromedioGolesPorEquipoYCompeticion() {
    std::string equipo;
    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end()) {
        std::cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    std::cout << "Promedio de goles para el equipo: " << equipo << "\n";
    for (const auto& [competicion, stats] : estadisticas[equipo]) {
        int partidos = stats.triunfos + stats.derrotas + stats.empates;
        if (partidos == 0) {
            std::cout << "Competición: " << competicion << " - Sin partidos registrados.\n";
            continue;
        }

        double promedio_favor = static_cast<double>(stats.goles_a_favor) / partidos;
        double promedio_contra = static_cast<double>(stats.goles_en_contra) / partidos;

        std::cout << "Competición: " << competicion << "\n";
        std::cout << "Promedio goles a favor: " << promedio_favor << "\n";
        std::cout << "Promedio goles en contra: " << promedio_contra << "\n";
    }
}
void mostrarTriunfosYDerrotasPorEquipoYCompeticion() {
    std::string equipo;
    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end()) {
        std::cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    std::cout << "Triunfos y derrotas para el equipo: " << equipo << "\n";
    for (const auto& [competicion, stats] : estadisticas[equipo]) {
        std::cout << "Competición: " << competicion << "\n";
        std::cout << "Triunfos: " << stats.triunfos << "\n";
        std::cout << "Derrotas: " << stats.derrotas << "\n";
        std::cout << "Empates: " << stats.empates << "\n";
    }
}
void mostrarFechasConMasYMenosGolesPorEquipoYCompeticion() {
    std::string equipo;
    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    if (estadisticas.find(equipo) == estadisticas.end()) {
        std::cout << "No hay datos para el equipo: " << equipo << "\n";
        return;
    }

    std::cout << "Fechas con más y menos goles para el equipo: " << equipo << "\n";

    for (const auto& [competicion, stats] : estadisticas[equipo]) {
        const Partido* partido_mas_goles = nullptr;
        const Partido* partido_menos_goles = nullptr;
        int max_goles = -1;
        int min_goles = INT_MAX;

        for (const auto& partido_ptr : partidos) {
            const Partido* partido = partido_ptr.get();
            if ((partido->equipo_local == equipo || partido->equipo_visitante == equipo) &&
                partido->competicion == competicion) {
                int total_goles = partido->goles_local + partido->goles_visitante;

                if (total_goles > max_goles) {
                    max_goles = total_goles;
                    partido_mas_goles = partido;
                }
                if (total_goles < min_goles) {
                    min_goles = total_goles;
                    partido_menos_goles = partido;
                }
            }
        }

        std::cout << "Competición: " << competicion << "\n";
        if (partido_mas_goles) {
            std::cout << "Fecha con más goles: " << partido_mas_goles->fecha << " - "
                      << max_goles << " goles\n";
        }
        if (partido_menos_goles) {
            std::cout << "Fecha con menos goles: " << partido_menos_goles->fecha << " - "
                      << min_goles << " goles\n";
        }
    }
}
void mostrarCompeticionConMasGoles() {
    if (goles_totales_por_competicion.empty()) {
        std::cout << "No hay competiciones registradas.\n";
        return;
    }

    auto max_goles_it = std::max_element(
        goles_totales_por_competicion.begin(), goles_totales_por_competicion.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    std::cout << "La competición con más goles es: " << max_goles_it->first
              << " con " << max_goles_it->second << " goles.\n";
}
void mostrarEquipoConMasYMenosGoles() {
    // Variables para almacenar los equipos con más y menos goles a nivel global.
    std::string equipo_max_global, equipo_min_global;
    int max_goles_global = -1, min_goles_global = INT_MAX;

    // Mapas para almacenar los equipos con más y menos goles por competición.
    std::unordered_map<std::string, std::pair<std::string, std::string>> equipos_por_competicion;
    std::unordered_map<std::string, std::pair<int, int>> goles_por_competicion;

    // Recorrer cada equipo y sus estadísticas.
    for (const auto& [equipo, competiciones] : estadisticas) {
        int goles_total_equipo = 0;

        for (const auto& [competicion, stats] : competiciones) {
            // Sumar los goles del equipo en esta competición.
            goles_total_equipo += stats.goles_a_favor;

            // Inicializar los valores de comparación si es la primera vez.
            if (goles_por_competicion[competicion].first == 0) {
                goles_por_competicion[competicion] = {stats.goles_a_favor, stats.goles_a_favor};
                equipos_por_competicion[competicion] = {equipo, equipo};
            }

            // Comparar para más y menos goles por competición.
            if (stats.goles_a_favor > goles_por_competicion[competicion].first) {
                goles_por_competicion[competicion].first = stats.goles_a_favor;
                equipos_por_competicion[competicion].first = equipo;
            }
            if (stats.goles_a_favor < goles_por_competicion[competicion].second) {
                goles_por_competicion[competicion].second = stats.goles_a_favor;
                equipos_por_competicion[competicion].second = equipo;
            }
        }

        // Comparar para más y menos goles a nivel global.
        if (goles_total_equipo > max_goles_global) {
            max_goles_global = goles_total_equipo;
            equipo_max_global = equipo;
        }
        if (goles_total_equipo < min_goles_global) {
            min_goles_global = goles_total_equipo;
            equipo_min_global = equipo;
        }
    }

    // Mostrar los equipos con más y menos goles por competición.
    std::cout << "Equipos con más y menos goles por competición:\n";
    for (const auto& [competicion, equipos] : equipos_por_competicion) {
        std::cout << "Competición: " << competicion << "\n";
        std::cout << "Equipo con más goles: " << equipos.first
                  << " con " << goles_por_competicion[competicion].first << " goles\n";
        std::cout << "Equipo con menos goles: " << equipos.second
                  << " con " << goles_por_competicion[competicion].second << " goles\n";
    }

    // Mostrar los equipos con más y menos goles en total.
    std::cout << "\nEquipos con más y menos goles en total:\n";
    std::cout << "Equipo con más goles: " << equipo_max_global
              << " con " << max_goles_global << " goles\n";
    std::cout << "Equipo con menos goles: " << equipo_min_global
              << " con " << min_goles_global << " goles\n";
}
// Funciones para agregar un partido
bool validarFecha(const std::string& fecha) {
    if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/') return false;
    std::string dia = fecha.substr(0, 2);
    std::string mes = fecha.substr(3, 2);
    std::string anio = fecha.substr(6, 4);
    return esNumerico(dia) && esNumerico(mes) && esNumerico(anio);
}

int ingresarEnteroPositivo(const std::string& mensaje) {
    int valor;
    do {
        std::cout << mensaje;
        std::cin >> valor;
        std::cin.ignore();  // Limpiar el buffer de entrada
        if (valor < 0) std::cout << "Por favor, ingrese un número positivo.\n";
    } while (valor < 0);
    return valor;
}

void agregarPartido() {
    std::string fecha, equipo_local, equipo_visitante, competicion;
    int goles_local, goles_visitante;

    // Pedir datos al usuario con validación
    do {
        std::cout << "Ingrese la fecha (DD/MM/YYYY): ";
        std::getline(std::cin, fecha);
        if (!validarFecha(fecha)) {
            std::cout << "Fecha inválida. Intente nuevamente.\n";
        }
    } while (!validarFecha(fecha));

    std::cout << "Ingrese el equipo local: ";
    std::getline(std::cin, equipo_local);

    goles_local = ingresarEnteroPositivo("Ingrese los goles del equipo local: ");

    std::cout << "Ingrese el equipo visitante: ";
    std::getline(std::cin, equipo_visitante);

    goles_visitante = ingresarEnteroPositivo("Ingrese los goles del equipo visitante: ");

    std::cout << "Ingrese la competición: ";
    std::getline(std::cin, competicion);

    // Crear y almacenar el nuevo partido
    auto nuevo_partido = std::make_unique<Partido>(
        "Nueva Jornada", fecha, equipo_local, goles_local, goles_visitante, equipo_visitante, competicion);
    Partido* ptr = nuevo_partido.get();
    partidos.push_back(std::move(nuevo_partido));

    // Actualizar estadísticas y mapas
    actualizarEstadisticas(equipo_local, competicion, goles_local, goles_visitante,
                           goles_local > goles_visitante, goles_local == goles_visitante);
    actualizarEstadisticas(equipo_visitante, competicion, goles_visitante, goles_local,
                           goles_visitante > goles_local, goles_local == goles_visitante);

    goles_por_competicion[competicion].emplace(goles_local + goles_visitante, ptr);
    goles_totales_por_competicion[competicion] += (goles_local + goles_visitante);

    std::cout << "Partido agregado exitosamente.\n";
}
#include <algorithm> // para std::transform

// Función auxiliar: Convertir una cadena a minúsculas
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Función auxiliar: Extraer el año de una fecha en formato DD/MM/YYYY
int extraerAnio(const std::string& fecha) {
    return std::stoi(fecha.substr(6, 4));  // Extraer los últimos 4 caracteres (el año)
}

// Función de filtrado por año
std::vector<Partido*> filtrarPartidosPorAnio() {
    std::string competicion, equipo;
    int anio;

    // Pedir la competición
    std::cout << "Ingrese la competición: ";
    std::getline(std::cin, competicion);
    competicion = toLower(competicion);  // Convertir a minúsculas

    // Pedir el equipo
    std::cout << "Ingrese el equipo (local o visitante): ";
    std::getline(std::cin, equipo);
    equipo = toLower(equipo);  // Convertir a minúsculas

    // Pedir el año del partido
    std::cout << "Ingrese el año del partido (YYYY): ";
    std::cin >> anio;
    std::cin.ignore();  // Limpiar el buffer de entrada

    // Filtrar los partidos según los criterios proporcionados
    std::vector<Partido*> resultados;
    for (const auto& partido_ptr : partidos) {
        Partido* partido = partido_ptr.get();

        // Convertir datos del partido a minúsculas para comparación
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
        std::cout << "Seleccione el número del partido que desea eliminar: ";
        std::cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            std::cout << "Opción inválida. Intente nuevamente.\n";
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
        std::cout << "Seleccione el número del partido que desea modificar: ";
        std::cin >> opcion;
        if (opcion < 1 || opcion > static_cast<int>(resultados.size())) {
            std::cout << "Opción inválida. Intente nuevamente.\n";
        }
    } while (opcion < 1 || opcion > static_cast<int>(resultados.size()));

    Partido* partido = resultados[opcion - 1];

    // Menú para modificar campos
    int campo;
    do {
        std::cout << "\nSeleccione el campo a modificar:\n";
        std::cout << "1. Fecha (" << partido->fecha << ")\n";
        std::cout << "2. Equipo local (" << partido->equipo_local << ")\n";
        std::cout << "3. Goles equipo local (" << partido->goles_local << ")\n";
        std::cout << "4. Equipo visitante (" << partido->equipo_visitante << ")\n";
        std::cout << "5. Goles equipo visitante (" << partido->goles_visitante << ")\n";
        std::cout << "6. Competición (" << partido->competicion << ")\n";
        std::cout << "0. Guardar y salir\n";
        std::cout << "Opción: ";
        std::cin >> campo;
        std::cin.ignore();

        switch (campo) {
            case 1: {
                std::string nueva_fecha;
                do {
                    std::cout << "Ingrese la nueva fecha (DD/MM/YYYY): ";
                    std::getline(std::cin, nueva_fecha);
                    if (!validarFecha(nueva_fecha)) {
                        std::cout << "Fecha inválida. Intente nuevamente.\n";
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
                std::cout << "Ingrese la nueva competición: ";
                std::getline(std::cin, partido->competicion);
                break;
            case 0:
                std::cout << "Cambios guardados.\n";
                break;
            default:
                std::cout << "Opción inválida. Intente nuevamente.\n";
        }
    } while (campo != 0);
}



int menuProcesamientoDeDatos() {
    int opcion = 0;
    do {
        std::cout << "\nProcesamiento de datos durante lectura archivo CSV:\n";
        std::cout << "1. Top 5 partidos con más goles por competición\n";
        std::cout << "2. Goles a favor y en contra por equipo\n";
        std::cout << "3. Promedio de goles por equipo\n";
        std::cout << "4. Triunfos y derrotas por equipo\n";
        std::cout << "5. Fecha con más y menos goles por equipo\n";
        std::cout << "6. Competición con más goles\n";
        std::cout << "7. Equipos con más y menos goles\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                mostrarTop5PartidosTodasLasCompeticiones();
                break;
            case 2:
                mostrarGolesPorEquipoYCompeticion();
                break;
            case 3:
                mostrarPromedioGolesPorEquipoYCompeticion();
                break;
            case 4:
                mostrarTriunfosYDerrotasPorEquipoYCompeticion();
                break;
            case 5:
                mostrarFechasConMasYMenosGolesPorEquipoYCompeticion();
                break;
            case 6:
                mostrarCompeticionConMasGoles();
                break;
            case 7:
                mostrarEquipoConMasYMenosGoles();
                break;
            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    return 0;
}

void menuModificacionDeDatos() {
    int opcion = 0;
    do {
        std::cout << "\nModificación de datos y reprocesamiento:\n";
        std::cout << "1. Agregar un partido\n";
        std::cout << "2. Eliminar un partido\n";
        std::cout << "3. Modificar un partido\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                agregarPartido();
                break;
            case 2:
                eliminarPartido();
                break;
            case 3:
                modificarPartido();
                break;
            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Menú principal
int main() {
    std::string archivo = "Base_Datos_COMA.csv";
    cargarDatosDesdeCSV(archivo);

    std::cout << "Total de partidos cargados: " << partidos.size() << "\n";

    int opcion = 0;
    do {
        std::cout << "\nMenú Principal:\n";
        std::cout << "1. Procesamiento de datos durante lectura archivo CSV\n";
        std::cout << "2. Modificación de datos y reprocesamiento\n";
        std::cout << "0. Salir\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                menuProcesamientoDeDatos();
                break;
            case 2:
                menuModificacionDeDatos();
                break;
            case 0:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    return 0;
}