#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
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
    // Variable para medir el tiempo de ejecucion
    clock_t begin, end;
    begin = clock();

    ifstream archivo(filename);
    string linea;
    char delimitador = ',';

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Leemos la primera linea para descartarla (encabezado)
    getline(archivo, linea);

    // Leemos todas las lineas
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

        // Crear una instancia de Partido y llenarla con los datos extraidos
        Partido partido = {jornada, equipoLocal, equipoVisitante, golesLocalInt, golesVisitanteInt, fecha, competicion};

        // Agregar el partido al vector
        partidos.push_back(partido);
    }

    cout << "Datos cargados correctamente." << endl;
    archivo.close();

    // Medir el tiempo de ejecucion
    end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Tiempo de ejecucion: " << elapsed_secs << " segundos" << endl;
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

// 1. Mostrar los 5 partidos con mas goles
void mostrarTop5PartidosConMasGoles(const vector<Partido> &partidos) {
    vector<Partido> partidosOrdenados = partidos;
    sort(partidosOrdenados.begin(), partidosOrdenados.end(), compararPorGoles);

    cout << "Los 5 partidos con mas goles son:\n";
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
// Mostrar promedio de goles a favor y en contra de un equipo por competicion
void mostrarPromedioGolesAFavorYEnContra(const vector<Partido> &partidos, const string &equipo) {
    map<string, pair<int, int>> golesAFavorYEnContra;  // <competicion, <goles a favor, goles en contra>>
    map<string, int> partidosJugados;  // <competicion, cantidad de partidos>

    for (const Partido &p : partidos) {
        if (p.equipoLocal == equipo) {
            golesAFavorYEnContra[p.competicion].first += p.golesLocal;
            golesAFavorYEnContra[p.competicion].second += p.golesVisitante;
            partidosJugados[p.competicion]++;
        } else if (p.equipoVisitante == equipo) {
            golesAFavorYEnContra[p.competicion].first += p.golesVisitante;
            golesAFavorYEnContra[p.competicion].second += p.golesLocal;
            partidosJugados[p.competicion]++;
        }
    }

    cout << "Promedio de goles a favor y en contra de " << equipo << ":\n";
    for (const auto &entry : golesAFavorYEnContra) {
        double promedioAFavor = (double)entry.second.first / partidosJugados[entry.first];
        double promedioEnContra = (double)entry.second.second / partidosJugados[entry.first];
        cout << entry.first << ": " << promedioAFavor << " goles a favor por partido, "
             << promedioEnContra << " goles en contra por partido\n";
    }
}


// 4. Mostrar victorias y derrotas de un equipo
void mostrarVictoriasYDerrotas(const vector<Partido> &partidos, const string &equipo) {
    map<string, pair<int, int>> victoriasYDerrotas;  // <competicion, <victorias, derrotas>>

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

// 5. Mostrar fechas con mas y menos goles
// Mostrar la fecha con mas y menos goles de un equipo por competicion
void mostrarFechasConMasYMenosGolesPorCompeticion(const vector<Partido> &partidos, const string &equipo) {
    map<string, pair<Partido, Partido>> maxMinGoles;  // <competicion, <partido con mas goles, partido con menos goles>>

    for (const Partido &p : partidos) {
        if (p.equipoLocal == equipo || p.equipoVisitante == equipo) {
            int goles = p.golesLocal + p.golesVisitante;

            if (maxMinGoles.find(p.competicion) == maxMinGoles.end()) {
                maxMinGoles[p.competicion] = make_pair(p, p);
            } else {
                if (goles > maxMinGoles[p.competicion].first.golesLocal + maxMinGoles[p.competicion].first.golesVisitante) {
                    maxMinGoles[p.competicion].first = p;
                }
                if (goles < maxMinGoles[p.competicion].second.golesLocal + maxMinGoles[p.competicion].second.golesVisitante) {
                    maxMinGoles[p.competicion].second = p;
                }
            }
        }
    }

    cout << "Fechas con mas y menos goles de " << equipo << " por competicion:\n";
    for (const auto &entry : maxMinGoles) {
        cout << entry.first << ":\n";
        cout << "  Fecha con mas goles: " << entry.second.first.fecha << " (" << entry.second.first.golesLocal + entry.second.first.golesVisitante << " goles)\n";
        cout << "  Fecha con menos goles: " << entry.second.second.fecha << " (" << entry.second.second.golesLocal + entry.second.second.golesVisitante << " goles)\n";
    }
}


// 6. Competicion con mas goles
void mostrarCompeticionConMasGoles(const vector<Partido> &partidos) {
    map<string, int> golesPorCompeticion;

    for (const Partido &p : partidos) {
        golesPorCompeticion[p.competicion] += golesTotales(p);
    }

    auto maxCompeticion = max_element(golesPorCompeticion.begin(), golesPorCompeticion.end(),
                                      [](const pair<string, int> &a, const pair<string, int> &b) {
                                          return a.second < b.second;
                                      });
    cout << "La competicion con mas goles es: " << maxCompeticion->first 
         << " (" << maxCompeticion->second << " goles)\n";
}

// 7. Equipo con mas y menos goles
// Mostrar equipo con mas y menos goles convertidos (por competicion y global)
void mostrarEquipoConMasYMenosGolesPorCompeticion(const vector<Partido> &partidos) {
    map<string, map<string, int>> golesPorEquipoYCompeticion;  // <competicion, <equipo, goles>>
    map<string, int> golesTotalesPorEquipo;  // Goles totales por equipo (en todas las competiciones)

    for (const Partido &p : partidos) {
        golesPorEquipoYCompeticion[p.competicion][p.equipoLocal] += p.golesLocal;
        golesPorEquipoYCompeticion[p.competicion][p.equipoVisitante] += p.golesVisitante;

        golesTotalesPorEquipo[p.equipoLocal] += p.golesLocal;
        golesTotalesPorEquipo[p.equipoVisitante] += p.golesVisitante;
    }

    // Mostrar por competicion
    for (const auto &entry : golesPorEquipoYCompeticion) {
        auto maxEquipo = max_element(entry.second.begin(), entry.second.end(),
                                     [](const pair<string, int> &a, const pair<string, int> &b) {
                                         return a.second < b.second;
                                     });
        auto minEquipo = min_element(entry.second.begin(), entry.second.end(),
                                     [](const pair<string, int> &a, const pair<string, int> &b) {
                                         return a.second < b.second;
                                     });
        cout << "Competicion: " << entry.first << "\n";
        cout << "  Equipo con mas goles: " << maxEquipo->first << " (" << maxEquipo->second << " goles)\n";
        cout << "  Equipo con menos goles: " << minEquipo->first << " (" << minEquipo->second << " goles)\n";
    }

    // Mostrar globalmente
    auto maxEquipoGlobal = max_element(golesTotalesPorEquipo.begin(), golesTotalesPorEquipo.end(),
                                       [](const pair<string, int> &a, const pair<string, int> &b) {
                                           return a.second < b.second;
                                       });
    auto minEquipoGlobal = min_element(golesTotalesPorEquipo.begin(), golesTotalesPorEquipo.end(),
                                       [](const pair<string, int> &a, const pair<string, int> &b) {
                                           return a.second < b.second;
                                       });

    cout << "\nGlobalmente:\n";
    cout << "  Equipo con mas goles: " << maxEquipoGlobal->first << " (" << maxEquipoGlobal->second << " goles)\n";
    cout << "  Equipo con menos goles: " << minEquipoGlobal->first << " (" << minEquipoGlobal->second << " goles)\n";
}

// Funcion del menu
void menu(vector<Partido> &partidos) {
    int opcion;
    string equipo;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Mostrar los 5 partidos con mas goles\n";
        cout << "2. Mostrar goles totales de un equipo\n";
        cout << "3. Mostrar promedio de goles de un equipo\n";
        cout << "4. Mostrar victorias y derrotas de un equipo\n";
        cout << "5. Mostrar fechas con mas y menos goles\n";
        cout << "6. Mostrar competicion con mas goles\n";
        cout << "7. Mostrar equipo con mas y menos goles\n";
        cout << "0. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                // Opcion 1: Mostrar los 5 partidos con mas goles
                mostrarTop5PartidosConMasGoles(partidos);
                break;
            }
            case 2: {
                // Opcion 2: Mostrar goles totales de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();  // Limpiar el buffer
                getline(cin, equipo);
                mostrarGolesTotalesEquipo(partidos, equipo);
                break;
            }
            case 3: {
                // Opcion 3: Mostrar promedio de goles de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();
                getline(cin, equipo);
                mostrarPromedioGolesAFavorYEnContra(partidos, equipo);
                break;
            }
            case 4: {
                // Opcion 4: Mostrar victorias y derrotas de un equipo
                string equipo;
                cout << "Ingrese el nombre del equipo: ";
                cin.ignore();
                getline(cin, equipo);
                mostrarVictoriasYDerrotas(partidos, equipo);
                break;
            }
            case 5: {
                // Opcion 5: Mostrar fechas con mas y menos goles
                    string equipo;
    cout << "Ingrese el nombre del equipo: ";
    cin.ignore();
    getline(cin, equipo);
    mostrarFechasConMasYMenosGolesPorCompeticion(partidos, equipo);
                break;
            }
            case 6: {
                // Opcion 6: Mostrar competicion con mas goles
                mostrarCompeticionConMasGoles(partidos);
                break;
            }
            case 7: {
                // Opcion 7: Mostrar equipo con mas y menos goles
                mostrarEquipoConMasYMenosGolesPorCompeticion(partidos);
                break;
            }
            case 0:
                // Opcion 0: Salir
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
                break;
        }
    } while (opcion != 0);
}

int main() {
    vector<Partido> partidos;

    // Llamamos a la funcion para cargar los datos antes de mostrar el menu
    cargarDatos(partidos, NOMBRE_ARCHIVO);

    // Llamamos al menu
    menu(partidos);

    return 0;
}
