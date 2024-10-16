#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#define NOMBRE_ARCHIVO "Base_Datos_COMA.csv"

using namespace std;

// Estructura para almacenar datos de los partidos
struct Partido {
    string jornada;
    string equipoLocal;
    string equipoVisitante;
    int golesLocal;
    int golesVisitante;
    string fecha;
    string competicion;
};

void cargarDatos(vector<Partido> &partidos, const string &filename) {
    // Variable para medir el tiempo de ejecución
    clock_t begin, end;
    begin = clock();

    ifstream archivo(filename);
    string linea;
    char delimitador = ',';

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Leemos la primera línea para descartarla (encabezado)
    getline(archivo, linea);

    // Leemos todas las líneas
    while (getline(archivo, linea)) {
        stringstream stream(linea); // Convertir la cadena a un stream
        string jornada, fecha, equipoLocal, golesLocal, golesVisitante, equipoVisitante, competicion;
        
        // Extraer todos los valores de esa fila
        getline(stream, jornada, delimitador);
        getline(stream, fecha, delimitador);
        getline(stream, equipoLocal, delimitador);
        getline(stream, golesLocal, delimitador);
        getline(stream, golesVisitante, delimitador);
        getline(stream, equipoVisitante, delimitador);
        getline(stream, competicion, delimitador);

        // Convertir los goles a enteros
        int golesLocalInt = !golesLocal.empty() ? stoi(golesLocal) : 0;
        int golesVisitanteInt = !golesVisitante.empty() ? stoi(golesVisitante) : 0;

        // Crear una instancia de Partido y llenarla con los datos extraídos
        Partido partido = {jornada, equipoLocal, equipoVisitante, golesLocalInt, golesVisitanteInt, fecha, competicion};

        // Agregar el partido al vector
        partidos.push_back(partido);
    }

    cout << "Datos cargados correctamente." << endl;
    archivo.close();

    // Medir el tiempo de ejecución
    end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecución: " << elapsed_secs << " segundos" << endl;
}

// Funciones auxiliares
int golesTotales(const Partido &p) {
    return p.golesLocal + p.golesVisitante;
}

// Comparador para ordenar por goles
bool compararPorGoles(const Partido &a, const Partido &b) {
    if (golesTotales(a) == golesTotales(b)) {
        return a.fecha > b.fecha;  // Si tienen los mismos goles, ordena por fecha
    }
    return golesTotales(a) > golesTotales(b);  // Ordena por goles totales
}

// 1. Mostrar los 5 partidos con más goles
void mostrarTop5PartidosConMasGoles(const vector<Partido> &partidos) {
    vector<Partido> partidosOrdenados = partidos;
    sort(partidosOrdenados.begin(), partidosOrdenados.end(), compararPorGoles);

    cout << "Los 5 partidos con más goles son:\n";
    for (int i = 0; i < min(5, (int)partidosOrdenados.size()); ++i) {
        cout << partidosOrdenados[i].equipoLocal << " vs " 
             << partidosOrdenados[i].equipoVisitante << " - " 
             << golesTotales(partidosOrdenados[i]) << " goles - " 
             << partidosOrdenados[i].fecha << " - " 
             << partidosOrdenados[i].competicion << endl;
    }
}

// 2. Mostrar goles totales de un equipo
void mostrarGolesTotalesEquipo(const vector<Partido> &partidos, const string &equipo) {
    map<string, int> golesAFavor, golesEnContra;

    for (const Partido &p : partidos) {
        if (p.equipoLocal == equipo) {
            golesAFavor[p.competicion] += p.golesLocal;
            golesEnContra[p.competicion] += p.golesVisitante;
        }
        if (p.equipoVisitante == equipo) {
            golesAFavor[p.competicion] += p.golesVisitante;
            golesEnContra[p.competicion] += p.golesLocal;
        }
    }

    cout << "Goles totales de " << equipo << ":\n";
    for (const auto &entry : golesAFavor) {
        cout << entry.first << ": " << entry.second << " goles a favor y " 
             << golesEnContra[entry.first] << " en contra\n";
    }
}

// 3. Mostrar promedio de goles de un equipo
void mostrarPromedioGolesEquipo(const vector<Partido> &partidos, const string &equipo) {
    map<string, pair<int, int>> golesYPartidos;  // <competición, <goles, partidos>>

    for (const Partido &p : partidos) {
        if (p.equipoLocal == equipo) {
            golesYPartidos[p.competicion].first += p.golesLocal;
            golesYPartidos[p.competicion].second++;
        }
        if (p.equipoVisitante == equipo) {
            golesYPartidos[p.competicion].first += p.golesVisitante;
            golesYPartidos[p.competicion].second++;
        }
    }

    cout << "Promedio de goles de " << equipo << ":\n";
    for (const auto &entry : golesYPartidos) {
        double promedio = (double)entry.second.first / entry.second.second;
        cout << entry.first << ": " << promedio << " goles por partido\n";
    }
}

// 4. Mostrar victorias y derrotas de un equipo
void mostrarVictoriasYDerrotas(const vector<Partido> &partidos, const string &equipo) {
    map<string, pair<int, int>> victoriasYDerrotas;  // <competición, <victorias, derrotas>>

    for (const Partido &p : partidos) {
        if (p.equipoLocal == equipo) {
            if (p.golesLocal > p.golesVisitante) {
                victoriasYDerrotas[p.competicion].first++;
            } else if (p.golesLocal < p.golesVisitante) {
                victoriasYDerrotas[p.competicion].second++;
            }
        }
        if (p.equipoVisitante == equipo) {
            if (p.golesVisitante > p.golesLocal) {
                victoriasYDerrotas[p.competicion].first++;
            } else if (p.golesVisitante < p.golesLocal) {
                victoriasYDerrotas[p.competicion].second++;
            }
        }
    }

    cout << "Victorias y derrotas de " << equipo << ":\n";
    for (const auto &entry : victoriasYDerrotas) {
        cout << entry.first << ": " << entry.second.first << " victorias y " 
             << entry.second.second << " derrotas\n";
    }
}

// 5. Mostrar fechas con más y menos goles
void mostrarFechasConMasMenosGoles(const vector<Partido> &partidos) {
    if (partidos.empty()) return;

    auto maxGoles = max_element(partidos.begin(), partidos.end(), compararPorGoles);
    auto minGoles = min_element(partidos.begin(), partidos.end(), compararPorGoles);

    cout << "Fecha con más goles: " << maxGoles->fecha 
         << " (" << golesTotales(*maxGoles) << " goles)\n";
    cout << "Fecha con menos goles: " << minGoles->fecha 
         << " (" << golesTotales(*minGoles) << " goles)\n";
}

// 6. Competición con más goles
void mostrarCompeticionConMasGoles(const vector<Partido> &partidos) {
    map<string, int> golesPorCompeticion;

    for (const Partido &p : partidos) {
        golesPorCompeticion[p.competicion] += golesTotales(p);
    }

    auto maxCompeticion = max_element(golesPorCompeticion.begin(), golesPorCompeticion.end(),
                                      [](const pair<string, int> &a, const pair<string, int> &b) {
                                          return a.second < b.second;
                                      });
    cout << "La competición con más goles es: " << maxCompeticion->first 
         << " (" << maxCompeticion->second << " goles)\n";
}

// 7. Equipo con más y menos goles
void mostrarEquipoConMasYMenosGoles(const vector<Partido> &partidos) {
    map<string, int> golesPorEquipo;

    for (const Partido &p : partidos) {
        golesPorEquipo[p.equipoLocal] += p.golesLocal;
        golesPorEquipo[p.equipoVisitante] += p.golesVisitante;
    }

    auto maxEquipo = max_element(golesPorEquipo.begin(), golesPorEquipo.end(),
                                 [](const pair<string, int> &a, const pair<string, int> &b) {
                                     return a.second < b.second;
                                 });
    auto minEquipo = min_element(golesPorEquipo.begin(), golesPorEquipo.end(),
                                 [](const pair<string, int> &a, const pair<string, int> &b) {
                                     return a.second < b.second;
                                 });

    cout << "Equipo con más goles: " << maxEquipo->first 
         << " (" << maxEquipo->second << " goles)\n";
    cout << "Equipo con menos goles: " << minEquipo->first 
         << " (" << minEquipo->second << " goles)\n";
}

// Función del menú
void menu(vector<Partido> &partidos) {
    int opcion;
    string equipo;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Mostrar los 5 partidos con más goles\n";
        cout << "2. Mostrar goles totales de un equipo\n";
        cout << "3. Mostrar promedio de goles de un equipo\n";
        cout << "4. Mostrar victorias y derrotas de un equipo\n";
        cout << "5. Mostrar fechas con más y menos goles\n";
        cout << "6. Mostrar competición con más goles\n";
        cout << "7. Mostrar equipo con más y menos goles\n";
        cout << "0. Salir\n";
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                // Opción 1: Mostrar los 5 partidos con más goles
                mostrarTop5PartidosConMasGoles(partidos);
                break;
            }
            case 2: {
                // Opción 2: Mostrar goles totales de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();  // Limpiar el buffer
                getline(cin, equipo);
                mostrarGolesTotalesEquipo(partidos, equipo);
                break;
            }
            case 3: {
                // Opción 3: Mostrar promedio de goles de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();
                getline(cin, equipo);
                mostrarPromedioGolesEquipo(partidos, equipo);
                break;
            }
            case 4: {
                // Opción 4: Mostrar victorias y derrotas de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();
                getline(cin, equipo);
                mostrarVictoriasYDerrotas(partidos, equipo);
                break;
            }
            case 5: {
                // Opción 5: Mostrar fechas con más y menos goles
                mostrarFechasConMasMenosGoles(partidos);
                break;
            }
            case 6: {
                // Opción 6: Mostrar competición con más goles
                mostrarCompeticionConMasGoles(partidos);
                break;
            }
            case 7: {
                // Opción 7: Mostrar equipo con más y menos goles
                mostrarEquipoConMasYMenosGoles(partidos);
                break;
            }
            case 0:
                // Opción 0: Salir
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
                break;
        }
    } while (opcion != 0);
}

int main() {
    vector<Partido> partidos;

    // Llamamos a la función para cargar los datos antes de mostrar el menú
    cargarDatos(partidos, NOMBRE_ARCHIVO);

    // Llamamos al menú
    menu(partidos);

    return 0;
}
