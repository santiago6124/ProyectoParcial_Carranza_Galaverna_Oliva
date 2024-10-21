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


int main() {
    std::string archivo = "Base_Datos_COMA.csv";
    cargarDatosDesdeCSV(archivo);

    std::cout << "Total de partidos cargados: " << partidos.size() << "\n";

    int opcion = 0;
    do {
        std::cout << "\nSeleccione una opción:\n";
        std::cout << "1. Top 5 partidos con más goles por competición\n";
        std::cout << "2. Goles a favor y en contra por equipo\n";
        std::cout << "3. Promedio de goles por equipo\n";
        std::cout << "4. Triunfos y derrotas por equipo\n";
        std::cout << "5. Fecha con más y menos goles por equipo\n";
        std::cout << "6. Competición con más goles\n";
         std::cout << "7. Equipos con más y menos goles\n";
        std::cout << "0. Salir\n";
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
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    return 0;
}


