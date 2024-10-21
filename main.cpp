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
            case 0:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    return 0;
}

