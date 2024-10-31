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
#include <chrono>     // Para medir el tiempo de ejecución
#include "../Globals/globals.h" // Asegúrate de incluir las estadísticas y partidos globales

using namespace std;
using namespace std::chrono;

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
}void listarResultadosEquipoCompeticion() {
    // Iniciar la medición del tiempo de ejecución


    string equipo, competicion;

    // Pedir el equipo y la competición al usuario
    cout << "Ingrese el equipo: ";
    getline(cin, equipo);
    cout << "Ingrese la competición: ";
    getline(cin, competicion);
        auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    equipo = toLower(equipo);           // Convertir a minúsculas para comparación
    competicion = toLower(competicion); // Convertir a minúsculas para comparación

    bool encontrados = false;

    // Listar los resultados del equipo en la competición específica
    for (const auto &partido_ptr : partidos) {
        Partido *partido = partido_ptr.get();
        string equipo_local = toLower(partido->equipo_local);
        string equipo_visitante = toLower(partido->equipo_visitante);
        string competicion_partido = toLower(partido->competicion);

        // Verificar si el equipo está en este partido y coincide la competición
        if (competicion_partido == competicion &&
            (equipo_local == equipo || equipo_visitante == equipo)) {
            encontrados = true;
            contador_ifs++;  // Conteo del 'if' para coincidencia de partido

            // Formatear la salida correctamente según el equipo que jugó
            cout << partido->fecha << " - ";

            if (equipo_local == equipo) {
                contador_ifs++;  // Conteo del 'if' para verificar equipo local

                cout << partido->equipo_local << " " << partido->goles_local << " a "
                     << partido->goles_visitante << " ";

                if (partido->goles_local > partido->goles_visitante) {
                    cout << "ganó a " << partido->equipo_visitante << ".\n";
                    contador_ifs++;  // Conteo del 'if' para victoria local
                } else if (partido->goles_local < partido->goles_visitante) {
                    cout << "perdió con " << partido->equipo_visitante << ".\n";
                    contador_ifs++;  // Conteo del 'if' para derrota local
                } else {
                    cout << "empató con " << partido->equipo_visitante << ".\n";
                    contador_ifs++;  // Conteo del 'if' para empate
                }
            } else {
                cout << partido->equipo_visitante << " " << partido->goles_visitante
                     << " a " << partido->goles_local << " ";

                if (partido->goles_visitante > partido->goles_local) {
                    cout << "ganó a " << partido->equipo_local << ".\n";
                    contador_ifs++;  // Conteo del 'if' para victoria visitante
                } else if (partido->goles_visitante < partido->goles_local) {
                    cout << "perdió con " << partido->equipo_local << ".\n";
                    contador_ifs++;  // Conteo del 'if' para derrota visitante
                } else {
                    cout << "empató con " << partido->equipo_local << ".\n";
                    contador_ifs++;  // Conteo del 'if' para empate visitante
                }
            }
        }
    }

    // Verificar si no se encontraron partidos
    if (!encontrados) {
        cout << "No se encontraron partidos para el equipo " << equipo
             << " en la competición " << competicion << ".\n";
        contador_ifs++;  // Conteo del 'if' para partidos no encontrados
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio);  // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void listarResultadosEquipoEntreFechas() {
    // Iniciar la medición del tiempo de ejecución
    int contador_ifs = 0; // Contador de 'if'


    string equipo, fecha_inicio, fecha_fin;

    // Pedir el equipo y las fechas al usuario
    cout << "Ingrese el equipo: ";
    getline(cin, equipo);


    // Validar la fecha de inicio
    do {
        cout << "Ingrese la fecha de inicio (DD/MM/YYYY): ";
        getline(cin, fecha_inicio);
        if (!validarFecha(fecha_inicio)) {
            cout << "Fecha inválida. Intente nuevamente.\n";
            contador_ifs++;  // Conteo del 'if' para fecha inválida
        }
    } while (!validarFecha(fecha_inicio));
    // Validar la fecha de fin
    do {
        cout << "Ingrese la fecha de fin (DD/MM/YYYY): ";
        getline(cin, fecha_fin);
        if (!validarFecha(fecha_fin)) {
            cout << "Fecha inválida. Intente nuevamente.\n";
            contador_ifs++;  // Conteo del 'if' para fecha inválida
        }
    } while (!validarFecha(fecha_fin));
        auto inicio = high_resolution_clock::now();

    equipo = toLower(equipo); // Convertir a minúsculas para comparación

    int fecha_inicio_int = convertirFecha(fecha_inicio);
    int fecha_fin_int = convertirFecha(fecha_fin);

    bool encontrados = false;

    // Buscar partidos del equipo entre las fechas especificadas
    for (const auto &partido_ptr : partidos) {
        Partido *partido = partido_ptr.get();
        string equipo_local = toLower(partido->equipo_local);
        string equipo_visitante = toLower(partido->equipo_visitante);
        string fecha_partido = partido->fecha;

        int fecha_partido_int = convertirFecha(fecha_partido);

        // Verificar si el equipo participó y la fecha está en el rango
        if ((equipo_local == equipo || equipo_visitante == equipo) &&
            fecha_partido_int >= fecha_inicio_int && fecha_partido_int <= fecha_fin_int) {
            encontrados = true;
            contador_ifs++;  // Conteo del 'if' para coincidencia de partido

            // Imprimir el resultado del partido
            cout << partido->fecha << " - ";
            if (equipo_local == equipo) {
                contador_ifs++;  // Conteo del 'if' para verificar equipo local

                cout << partido->equipo_local << " " << partido->goles_local << " a "
                     << partido->goles_visitante << " ";

                if (partido->goles_local > partido->goles_visitante) {
                    cout << "ganó a " << partido->equipo_visitante;
                    contador_ifs++;  // Conteo del 'if' para victoria local
                } else if (partido->goles_local < partido->goles_visitante) {
                    cout << "perdió con " << partido->equipo_visitante;
                    contador_ifs++;  // Conteo del 'if' para derrota local
                } else {
                    cout << "empató con " << partido->equipo_visitante;
                    contador_ifs++;  // Conteo del 'if' para empate
                }
            } else {
                cout << partido->equipo_visitante << " " << partido->goles_visitante
                     << " a " << partido->goles_local << " ";

                if (partido->goles_visitante > partido->goles_local) {
                    cout << "ganó a " << partido->equipo_local;
                    contador_ifs++;  // Conteo del 'if' para victoria visitante
                } else if (partido->goles_visitante < partido->goles_local) {
                    cout << "perdió con " << partido->equipo_local;
                    contador_ifs++;  // Conteo del 'if' para derrota visitante
                } else {
                    cout << "empató con " << partido->equipo_local;
                    contador_ifs++;  // Conteo del 'if' para empate visitante
                }
            }
            cout << " en " << partido->competicion << ".\n";
        }
    }

    // Verificar si no se encontraron partidos
    if (!encontrados) {
        cout << "No se encontraron partidos para " << equipo
             << " entre " << fecha_inicio << " y " << fecha_fin << ".\n";
        contador_ifs++;  // Conteo del 'if' para partidos no encontrados
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio);  // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}void compararRendimientoEntreEquipos() {
    std::string equipo1, equipo2;

    // Pedir los nombres de los equipos al usuario
    std::cout << "Ingrese el primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Convertir los nombres a minúsculas para comparación
    std::string equipo1_lower = toLower(equipo1);
    std::string equipo2_lower = toLower(equipo2);

    // Iniciar la medición del tiempo de ejecución después de la entrada del usuario
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    bool equipo1_encontrado = false, equipo2_encontrado = false;

    std::cout << "\nComparación de rendimiento:\n";

    // Recorrer los equipos en las estadísticas
    for (const auto &[equipo, competiciones] : estadisticas) {
        std::string equipo_lower = toLower(equipo); // Convertir para comparación

        // Verificar si el equipo es uno de los ingresados
        if (equipo_lower == equipo1_lower || equipo_lower == equipo2_lower) {
            contador_ifs++; // Conteo del 'if' para coincidencia de equipo

            for (const auto &[competicion, stats] : competiciones) {
                std::cout << equipo << " - " << competicion << ":\n";
                std::cout << "  Goles a favor: " << stats.goles_a_favor << "\n";
                std::cout << "  Goles en contra: " << stats.goles_en_contra << "\n";
            }

            if (equipo_lower == equipo1_lower) {
                equipo1_encontrado = true;
                contador_ifs++; // Conteo del 'if' para equipo1 encontrado
            }
            if (equipo_lower == equipo2_lower) {
                equipo2_encontrado = true;
                contador_ifs++; // Conteo del 'if' para equipo2 encontrado
            }
        }
    }

    // Mostrar mensajes de error si no se encontraron datos
    if (!equipo1_encontrado) {
        std::cout << "No se encontraron datos para el equipo: " << equipo1 << "\n";
        contador_ifs++; // Conteo del 'if' para equipo1 no encontrado
    }
    if (!equipo2_encontrado) {
        std::cout << "No se encontraron datos para el equipo: " << equipo2 << "\n";
        contador_ifs++; // Conteo del 'if' para equipo2 no encontrado
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    std::cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    std::cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
void compararRendimientoParticularEntreEquipos() {
    std::string equipo1, equipo2;

    // Pedir los nombres de los equipos al usuario
    std::cout << "Ingrese el primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Convertir los nombres a minúsculas para comparación
    std::string equipo1_lower = toLower(equipo1);
    std::string equipo2_lower = toLower(equipo2);

    // Iniciar la medición del tiempo de ejecución
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    int partidos_jugados = 0;
    int empates = 0;
    int victorias_equipo1 = 0;
    int victorias_equipo2 = 0;

    // Recorrer todos los partidos
    for (const auto &partido_ptr : partidos) {
        Partido *partido = partido_ptr.get();
        std::string local = toLower(partido->equipo_local);
        std::string visitante = toLower(partido->equipo_visitante);

        // Verificar si ambos equipos participaron en el mismo partido
        if ((local == equipo1_lower && visitante == equipo2_lower) ||
            (local == equipo2_lower && visitante == equipo1_lower)) {
            partidos_jugados++;
            contador_ifs++; // Conteo del 'if' para verificar participación de equipos

            // Verificar el resultado del partido
            if (partido->goles_local == partido->goles_visitante) {
                empates++;
                contador_ifs++; // Conteo del 'if' para empate
            } else if ((local == equipo1_lower && partido->goles_local > partido->goles_visitante) ||
                       (visitante == equipo1_lower && partido->goles_visitante > partido->goles_local)) {
                victorias_equipo1++;
                contador_ifs++; // Conteo del 'if' para victoria del equipo1
            } else {
                victorias_equipo2++;
                contador_ifs++; // Conteo del 'if' para victoria del equipo2
            }
        }
    }

    // Mostrar los resultados
    std::cout << "\nComparación de rendimiento particular entre " << equipo1 << " y " << equipo2 << ":\n";
    std::cout << "Partidos jugados entre ellos: " << partidos_jugados << "\n";
    std::cout << "Empates: " << empates << "\n";

    // Comparar el número de victorias
    if (victorias_equipo1 > victorias_equipo2) {
        std::cout << equipo1 << " ha ganado más partidos: " << victorias_equipo1 << "\n";
        contador_ifs++; // Conteo del 'if' para más victorias del equipo1
    } else if (victorias_equipo2 > victorias_equipo1) {
        std::cout << equipo2 << " ha ganado más partidos: " << victorias_equipo2 << "\n";
        contador_ifs++; // Conteo del 'if' para más victorias del equipo2
    } else {
        std::cout << "Ambos equipos tienen la misma cantidad de victorias.\n";
        contador_ifs++; // Conteo del 'if' para empate en victorias
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    std::cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    std::cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
#include <iostream>
#include <string>
#include <algorithm> // Para std::toupper
#include <chrono>     // Para medir el tiempo de ejecución
#include "../Globals/globals.h" // Asegúrate de incluir las estadísticas

using namespace std;
using namespace std::chrono;

void filtrarEquiposPorUmbralDeGoles() {
    double umbral;
    char opcion;

    // Pedir el umbral y la opción al usuario
    cout << "Ingrese el umbral de goles promedio por partido: ";
    cin >> umbral;

    // Validar la opción del usuario
    do {
        cout << "¿Desea filtrar por encima ('A') o por debajo ('B') del umbral? (A/B): ";
        cin >> opcion;
        opcion = toupper(opcion); // Convertir a mayúsculas para comparación
    } while (opcion != 'A' && opcion != 'B');

    cin.ignore(); // Limpiar el buffer de entrada

    // Iniciar la medición del tiempo de ejecución
    auto inicio = high_resolution_clock::now();
    int contador_ifs = 0; // Contador de 'if'

    cout << "\nEquipos con promedio de goles ";
    if (opcion == 'A') {
        cout << "por encima de " << umbral << ":\n";
        contador_ifs++; // Conteo del 'if' para opción A
    } else {
        cout << "por debajo de " << umbral << ":\n";
        contador_ifs++; // Conteo del 'if' para opción B
    }

    bool encontrado = false;

    // Recorrer las estadísticas de todos los equipos y competiciones
    for (const auto &[equipo, competiciones] : estadisticas) {
        for (const auto &[competicion, stats] : competiciones) {
            int partidos_jugados = stats.triunfos + stats.derrotas + stats.empates;

            // Evitar divisiones por cero
            if (partidos_jugados == 0) {
                contador_ifs++; // Conteo del 'if' para evitar división por cero
                continue;
            }

            // Calcular el promedio de goles por partido
            double promedio_goles = static_cast<double>(stats.goles_a_favor) / partidos_jugados;

            // Verificar si el equipo cumple con el criterio del umbral
            if ((opcion == 'A' && promedio_goles >= umbral) ||
                (opcion == 'B' && promedio_goles <= umbral)) {
                cout << equipo << " " << promedio_goles
                     << " goles promedio por partido en " << competicion << "\n";
                encontrado = true;
                contador_ifs++; // Conteo del 'if' para verificar el criterio del umbral
            }
        }
    }

    // Verificar si no se encontraron equipos
    if (!encontrado) {
        cout << "No se encontraron equipos que cumplan con el criterio.\n";
        contador_ifs++; // Conteo del 'if' para equipos no encontrados
    }

    // Finalizar la medición del tiempo de ejecución
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<duration<double>>(fin - inicio); // Duración en segundos

    // Mostrar resultados de ejecución
    cout << "\nTiempo de ejecución: " << duracion.count() << " segundos\n";
    cout << "Cantidad total de 'if': " << contador_ifs << "\n";
}
