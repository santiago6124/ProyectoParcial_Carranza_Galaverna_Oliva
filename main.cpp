#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>  // Para punteros inteligentes
#include <algorithm>  // Para std::remove_if

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

// Comparador para mantener el orden en std::set (AVL Tree)
struct ComparadorAVL {
    bool operator()(const Partido* a, const Partido* b) const {
        return a->fecha < b->fecha;
    }
};

// Definición de tipos: Árbol AVL simulado con std::set
using AVLTree = std::set<Partido*, ComparadorAVL>;

// Mapas de índices secundarios
std::unordered_map<std::string, AVLTree> competicion_map;
std::unordered_map<std::string, AVLTree> equipo_map;
std::unordered_map<int, AVLTree> año_map;

// Lista principal que almacena los partidos
std::vector<std::unique_ptr<Partido>> partidos;

// Verificar si una cadena es numérica
bool esNumerico(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Extraer el año desde una fecha en formato DD/MM/YYYY
int extraerAño(const std::string& fecha) {
    return std::stoi(fecha.substr(fecha.length() - 4));
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

        auto partido = std::make_unique<Partido>(
            jornada, fecha, local, std::stoi(golesL), std::stoi(golesV), visitante, competicion);
        Partido* ptr = partido.get();
        partidos.push_back(std::move(partido));

        // Insertar en los árboles AVL de los índices secundarios
        competicion_map[competicion].insert(ptr);
        equipo_map[local].insert(ptr);
        equipo_map[visitante].insert(ptr);
        año_map[extraerAño(fecha)].insert(ptr);
    }
}

// Mostrar partidos por competición
void mostrarPartidosPorCompeticion(const std::string& competicion) {
    if (competicion_map.find(competicion) != competicion_map.end()) {
        for (const auto* partido : competicion_map[competicion]) {
            std::cout << partido->fecha << ": " << partido->equipo_local << " vs "
                      << partido->equipo_visitante << " (" << partido->goles_local << "-"
                      << partido->goles_visitante << ") [" << partido->jornada << "]\n";
        }
    } else {
        std::cout << "No hay partidos en la competición: " << competicion << "\n";
    }
}

int main() {
    std::string archivo = "Base_Datos_COMA.csv";
    cargarDatosDesdeCSV(archivo);

    std::cout << "Total de partidos cargados: " << partidos.size() << "\n";

    // Mostrar partidos por competición para verificar que los datos se cargaron correctamente
    mostrarPartidosPorCompeticion("Champions League");

    return 0;
}
