#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

// Contador global para condicionales
int conditionalCount = 0;

// Estructura para almacenar detalles de un partido
struct Partido
{
    std::string id;
    std::string jornada;
    std::string equipoLocal;
    std::string equipoVisitante;
    int golesLocal;
    int golesVisitante;
    std::chrono::system_clock::time_point fecha;
    std::string competicion;
};

// Estructura para estadísticas de un equipo
struct EstadisticasEquipo
{
    int partidosJugados = 0;
    int partidosGanados = 0;
    int partidosEmpatados = 0;
    int partidosPerdidos = 0;
    int golesMarcados = 0;
    int golesRecibidos = 0;
};

// Función para incrementar el contador de condicionales
void incrementConditional()
{
    conditionalCount++;
}

// Función para convertir fecha de string a time_point
std::chrono::system_clock::time_point stringToTimePoint(const std::string &fechaStr)
{
    std::tm tm = {};
    std::istringstream ss(fechaStr);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Función para convertir time_point a string
std::string timePointToString(const std::chrono::system_clock::time_point &tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y");
    return oss.str();
}

// Mapa principal: ID único -> Partido
std::unordered_map<std::string, Partido> baseDatosPartidos;

// Vector ordenado por fecha
std::vector<Partido *> partidosOrdenados;

// Estadísticas por equipo
std::unordered_map<std::string, EstadisticasEquipo> estadisticasPorEquipo;

// Partidos por competición
std::unordered_map<std::string, std::vector<Partido *>> partidosPorCompeticion;

// Índices secundarios
std::unordered_map<std::string, std::vector<Partido *>> partidosPorEquipo;
std::unordered_map<int, std::vector<Partido *>> partidosPorGoles;
// Funciones adicionales que debes implementar o ajustar
void mostrarGolesEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo);
void mostrarPromedioGolesEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo);
void mostrarResultadosPorCompeticion(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo);
void mostrarFechasGolesPorEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo);
void mostrarCompeticionConMasGoles(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion);
void mostrarEquiposConMasYMenosGoles(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion);

// Función para cargar datos desde CSV
bool cargarDatosDesdeCSV(const std::string &rutaArchivoCompleta)
{
    std::ifstream archivo(rutaArchivoCompleta);
    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << rutaArchivoCompleta << '\n';
        return false;
    }

    std::string linea;
    // Leer encabezado
    std::getline(archivo, linea);
    while (std::getline(archivo, linea))
    {
        std::stringstream ss(linea);
        std::string jornada, equipoLocal, equipoVisitante, golesLocalStr, golesVisitanteStr, fechaStr, competicion;

        std::getline(ss, jornada, ',');
        std::getline(ss, equipoLocal, ',');
        std::getline(ss, equipoVisitante, ',');
        std::getline(ss, golesLocalStr, ',');
        std::getline(ss, golesVisitanteStr, ',');
        std::getline(ss, fechaStr, ',');
        std::getline(ss, competicion, ',');

        Partido partido;
        partido.jornada = jornada;
        partido.equipoLocal = equipoLocal;
        partido.equipoVisitante = equipoVisitante;
        partido.golesLocal = std::stoi(golesLocalStr);
        partido.golesVisitante = std::stoi(golesVisitanteStr);
        partido.fecha = stringToTimePoint(fechaStr);
        partido.competicion = competicion;

        // Crear ID único (puede ser combinado con jornada y fecha)
        partido.id = jornada + "|" + fechaStr;

        // Insertar en la base de datos
        baseDatosPartidos[partido.id] = partido;

        // Insertar en el vector ordenado
        partidosOrdenados.push_back(&baseDatosPartidos[partido.id]);

        // Actualizar índices secundarios
        partidosPorCompeticion[competicion].push_back(&baseDatosPartidos[partido.id]);

        partidosPorEquipo[partido.equipoLocal].push_back(&baseDatosPartidos[partido.id]);
        partidosPorEquipo[partido.equipoVisitante].push_back(&baseDatosPartidos[partido.id]);

        int totalGoles = partido.golesLocal + partido.golesVisitante;
        partidosPorGoles[totalGoles].push_back(&baseDatosPartidos[partido.id]);

        // Actualizar estadísticas por equipo
        // Equipo Local
        EstadisticasEquipo &estLocal = estadisticasPorEquipo[partido.equipoLocal];
        estLocal.partidosJugados++;
        estLocal.golesMarcados += partido.golesLocal;
        estLocal.golesRecibidos += partido.golesVisitante;
        if (partido.golesLocal > partido.golesVisitante)
        {
            estLocal.partidosGanados++;
        }
        else if (partido.golesLocal == partido.golesVisitante)
        {
            estLocal.partidosEmpatados++;
        }
        else
        {
            estLocal.partidosPerdidos++;
        }

        // Equipo Visitante
        EstadisticasEquipo &estVisitante = estadisticasPorEquipo[partido.equipoVisitante];
        estVisitante.partidosJugados++;
        estVisitante.golesMarcados += partido.golesVisitante;
        estVisitante.golesRecibidos += partido.golesLocal;
        if (partido.golesVisitante > partido.golesLocal)
        {
            estVisitante.partidosGanados++;
        }
        else if (partido.golesVisitante == partido.golesLocal)
        {
            estVisitante.partidosEmpatados++;
        }
        else
        {
            estVisitante.partidosPerdidos++;
        }

        // Incrementar condicionales (3 ifs por partido)
        incrementConditional(); // if (golesLocal > golesVisitante)
        incrementConditional(); // else if (golesLocal == golesVisitante)
        incrementConditional(); // else
    }

    // Ordenar el vector por fecha
    std::sort(partidosOrdenados.begin(), partidosOrdenados.end(),
              [](const Partido *a, const Partido *b)
              {
                  return a->fecha < b->fecha;
              });

    archivo.close();
    return true;
}

// Función para mostrar la eficiencia
void mostrarEficiencia(const std::string &nombreProceso,
                       const std::chrono::duration<double, std::milli> &tiempoEjecucion,
                       int cantidadIfs,
                       const std::string &algoritmoUsado,
                       const std::string &estructurasUsadas)
{
    std::cout << "Procesamiento de datos: " << nombreProceso << "\n"
              << "- Tiempo: " << tiempoEjecucion.count() << " ms\n"
              << "- Cantidad de if: " << cantidadIfs << "\n"
              << "- Algoritmo usado: " << algoritmoUsado << "\n"
              << "- Estructuras usadas: " << estructurasUsadas << "\n\n";
}

void mostrarResultadosPrincipales(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion)
{
    std::cout << "Resultados Principales:\n";
    for (const auto &entry : partidosPorCompeticion)
    {
        const std::string &competicion = entry.first;
        const std::vector<Partido*>& partidos = entry.second;

        // Ordenar los partidos por total de goles y luego por fecha descendente
        std::vector<Partido*> partidosOrdenadosLocal = partidos;
        std::sort(partidosOrdenadosLocal.begin(), partidosOrdenadosLocal.end(),
                  [&](const Partido* a, const Partido* b) -> bool
                  {
                      int totalA = a->golesLocal + a->golesVisitante;
                      int totalB = b->golesLocal + b->golesVisitante;

                      // Ordenar primero por total de goles
                      if (totalA != totalB)
                      {
                          return totalA > totalB; // Descendente
                      }
                      else
                      {
                          // Si hay un empate en goles, ordenar por fecha descendente
                          if (a->fecha != b->fecha)
                          {
                              return a->fecha > b->fecha; // Más reciente primero
                          }
                          else
                          {
                              // Criterio programador en caso de empate de fechas
                              return a->equipoLocal < b->equipoLocal;
                          }
                      }
                  });

        std::cout << "Competición: " << competicion << "\n";
        int count = 0;
        for (const auto *partido : partidosOrdenadosLocal)
        {
            if (count >= 5)
                break; // Solo mostrar los primeros 5
            std::cout << "  Jornada: " << partido->jornada
                      << " | " << timePointToString(partido->fecha) // Función para convertir fecha
                      << " | " << partido->equipoLocal << " " << partido->golesLocal
                      << " - " << partido->golesVisitante << " " << partido->equipoVisitante
                      << "\n";
            count++;
        }
    }
}


// Función para agregar un partido
void agregarPartido()
{
    Partido nuevoPartido;
    std::string fechaStr;

    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese la jornada: ";
    std::getline(std::cin, nuevoPartido.jornada);

    std::cout << "Ingrese el nombre del equipo local: ";
    std::getline(std::cin, nuevoPartido.equipoLocal);

    std::cout << "Ingrese el nombre del equipo visitante: ";
    std::getline(std::cin, nuevoPartido.equipoVisitante);

    std::cout << "Ingrese los goles del equipo local: ";
    std::cin >> nuevoPartido.golesLocal;

    std::cout << "Ingrese los goles del equipo visitante: ";
    std::cin >> nuevoPartido.golesVisitante;

    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese la fecha del partido (dd/mm/yyyy): ";
    std::getline(std::cin, fechaStr);
    nuevoPartido.fecha = stringToTimePoint(fechaStr);

    std::cout << "Ingrese la competición: ";
    std::getline(std::cin, nuevoPartido.competicion);

    // Crear ID único
    nuevoPartido.id = nuevoPartido.jornada + "|" + fechaStr;

    // Verificar si el ID ya existe
    incrementConditional(); // if (baseDatosPartidos.find(nuevoPartido.id) != baseDatosPartidos.end())
    if (baseDatosPartidos.find(nuevoPartido.id) != baseDatosPartidos.end())
    {
        std::cerr << "Error: Ya existe un partido con este ID.\n";
        return;
    }

    // Insertar en la base de datos
    baseDatosPartidos[nuevoPartido.id] = nuevoPartido;

    // Insertar en el vector ordenado
    partidosOrdenados.push_back(&baseDatosPartidos[nuevoPartido.id]);
    std::sort(partidosOrdenados.begin(), partidosOrdenados.end(),
              [](const Partido *a, const Partido *b)
              {
                  return a->fecha < b->fecha;
              });

    // Actualizar índices secundarios
    partidosPorCompeticion[nuevoPartido.competicion].push_back(&baseDatosPartidos[nuevoPartido.id]);

    partidosPorEquipo[nuevoPartido.equipoLocal].push_back(&baseDatosPartidos[nuevoPartido.id]);
    partidosPorEquipo[nuevoPartido.equipoVisitante].push_back(&baseDatosPartidos[nuevoPartido.id]);

    int totalGoles = nuevoPartido.golesLocal + nuevoPartido.golesVisitante;
    partidosPorGoles[totalGoles].push_back(&baseDatosPartidos[nuevoPartido.id]);

    // Actualizar estadísticas por equipo
    // Equipo Local
    EstadisticasEquipo &estLocal = estadisticasPorEquipo[nuevoPartido.equipoLocal];
    estLocal.partidosJugados++;
    estLocal.golesMarcados += nuevoPartido.golesLocal;
    estLocal.golesRecibidos += nuevoPartido.golesVisitante;
    if (nuevoPartido.golesLocal > nuevoPartido.golesVisitante)
    {
        estLocal.partidosGanados++;
    }
    else if (nuevoPartido.golesLocal == nuevoPartido.golesVisitante)
    {
        estLocal.partidosEmpatados++;
    }
    else
    {
        estLocal.partidosPerdidos++;
    }

    // Equipo Visitante
    EstadisticasEquipo &estVisitante = estadisticasPorEquipo[nuevoPartido.equipoVisitante];
    estVisitante.partidosJugados++;
    estVisitante.golesMarcados += nuevoPartido.golesVisitante;
    estVisitante.golesRecibidos += nuevoPartido.golesLocal;
    if (nuevoPartido.golesVisitante > nuevoPartido.golesLocal)
    {
        estVisitante.partidosGanados++;
    }
    else if (nuevoPartido.golesVisitante == nuevoPartido.golesLocal)
    {
        estVisitante.partidosEmpatados++;
    }
    else
    {
        estVisitante.partidosPerdidos++;
    }

    std::cout << "Partido agregado exitosamente.\n";
}

// Función para eliminar un partido
void eliminarPartido()
{
    std::string id;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el ID del partido a eliminar: ";
    std::getline(std::cin, id);

    // Buscar el partido
    incrementConditional(); // if (baseDatosPartidos.find(id) != baseDatosPartidos.end())
    if (baseDatosPartidos.find(id) == baseDatosPartidos.end())
    {
        std::cerr << "Error: Partido no encontrado.\n";
        return;
    }

    Partido *partido = &baseDatosPartidos[id];

    // Actualizar estadísticas por equipo
    // Equipo Local
    EstadisticasEquipo &estLocal = estadisticasPorEquipo[partido->equipoLocal];
    estLocal.partidosJugados--;
    estLocal.golesMarcados -= partido->golesLocal;
    estLocal.golesRecibidos -= partido->golesVisitante;
    if (partido->golesLocal > partido->golesVisitante)
    {
        estLocal.partidosGanados--;
    }
    else if (partido->golesLocal == partido->golesVisitante)
    {
        estLocal.partidosEmpatados--;
    }
    else
    {
        estLocal.partidosPerdidos--;
    }

    // Equipo Visitante
    EstadisticasEquipo &estVisitante = estadisticasPorEquipo[partido->equipoVisitante];
    estVisitante.partidosJugados--;
    estVisitante.golesMarcados -= partido->golesVisitante;
    estVisitante.golesRecibidos -= partido->golesLocal;
    if (partido->golesVisitante > partido->golesLocal)
    {
        estVisitante.partidosGanados--;
    }
    else if (partido->golesVisitante == partido->golesLocal)
    {
        estVisitante.partidosEmpatados--;
    }
    else
    {
        estVisitante.partidosPerdidos--;
    }

    // Eliminar de índices secundarios
    // Competición
    auto &vecComp = partidosPorCompeticion[partido->competicion];
    vecComp.erase(std::remove(vecComp.begin(), vecComp.end(), partido), vecComp.end());

    // Equipo Local
    auto &vecEqLocal = partidosPorEquipo[partido->equipoLocal];
    vecEqLocal.erase(std::remove(vecEqLocal.begin(), vecEqLocal.end(), partido), vecEqLocal.end());

    // Equipo Visitante
    auto &vecEqVisit = partidosPorEquipo[partido->equipoVisitante];
    vecEqVisit.erase(std::remove(vecEqVisit.begin(), vecEqVisit.end(), partido), vecEqVisit.end());

    // Goles
    int totalGoles = partido->golesLocal + partido->golesVisitante;
    auto &vecGoles = partidosPorGoles[totalGoles];
    vecGoles.erase(std::remove(vecGoles.begin(), vecGoles.end(), partido), vecGoles.end());

    // Eliminar del vector ordenado
    partidosOrdenados.erase(std::remove(partidosOrdenados.begin(), partidosOrdenados.end(), partido), partidosOrdenados.end());

    // Eliminar de la base de datos
    baseDatosPartidos.erase(id);

    std::cout << "Partido eliminado exitosamente.\n";
}

// Función para modificar un partido
void modificarPartido()
{
    std::string id;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el ID del partido a modificar: ";
    std::getline(std::cin, id);

    // Buscar el partido
    incrementConditional(); // if (baseDatosPartidos.find(id) != baseDatosPartidos.end())
    if (baseDatosPartidos.find(id) == baseDatosPartidos.end())
    {
        std::cerr << "Error: Partido no encontrado.\n";
        return;
    }

    Partido *partido = &baseDatosPartidos[id];

    // Mostrar datos actuales
    std::cout << "Datos actuales del partido:\n";
    std::cout << "Jornada: " << partido->jornada << "\n"
              << "Equipo Local: " << partido->equipoLocal << "\n"
              << "Equipo Visitante: " << partido->equipoVisitante << "\n"
              << "Goles Local: " << partido->golesLocal << "\n"
              << "Goles Visitante: " << partido->golesVisitante << "\n"
              << "Fecha: " << timePointToString(partido->fecha) << "\n"
              << "Competición: " << partido->competicion << "\n";

    // Opciones para modificar
    int opcion;
    std::cout << "Seleccione el campo a modificar:\n"
              << "1. Jornada\n"
              << "2. Equipo Local\n"
              << "3. Equipo Visitante\n"
              << "4. Goles Local\n"
              << "5. Goles Visitante\n"
              << "6. Fecha\n"
              << "7. Competición\n"
              << "Ingrese su opción: ";
    std::cin >> opcion;

    std::cin.ignore(); // Limpiar buffer

    switch (opcion)
    {
    case 1:
    {
        std::string nuevaJornada;
        std::cout << "Ingrese la nueva jornada: ";
        std::getline(std::cin, nuevaJornada);
        partido->jornada = nuevaJornada;
        break;
    }
    case 2:
    {
        std::string nuevoEquipoLocal;
        std::cout << "Ingrese el nuevo equipo local: ";
        std::getline(std::cin, nuevoEquipoLocal);
        partido->equipoLocal = nuevoEquipoLocal;
        break;
    }
    case 3:
    {
        std::string nuevoEquipoVisitante;
        std::cout << "Ingrese el nuevo equipo visitante: ";
        std::getline(std::cin, nuevoEquipoVisitante);
        partido->equipoVisitante = nuevoEquipoVisitante;
        break;
    }
    case 4:
    {
        int nuevosGolesLocal;
        std::cout << "Ingrese los nuevos goles del equipo local: ";
        std::cin >> nuevosGolesLocal;
        partido->golesLocal = nuevosGolesLocal;
        break;
    }
    case 5:
    {
        int nuevosGolesVisitante;
        std::cout << "Ingrese los nuevos goles del equipo visitante: ";
        std::cin >> nuevosGolesVisitante;
        partido->golesVisitante = nuevosGolesVisitante;
        break;
    }
    case 6:
    {
        std::string nuevaFechaStr;
        std::cout << "Ingrese la nueva fecha (dd/mm/yyyy): ";
        std::getline(std::cin, nuevaFechaStr);
        partido->fecha = stringToTimePoint(nuevaFechaStr);
        break;
    }
    case 7:
    {
        std::string nuevaCompeticion;
        std::cout << "Ingrese la nueva competición: ";
        std::getline(std::cin, nuevaCompeticion);
        partido->competicion = nuevaCompeticion;
        break;
    }
    default:
        std::cerr << "Opción inválida.\n";
        return;
    }

    // Recalcular estadísticas y actualizar índices si es necesario
    // Para simplificar, se puede optar por recargar todos los datos
    // Alternativamente, implementar lógica para actualizar solo los cambios

    std::cout << "Partido modificado exitosamente.\n";
}

// Función para resultados de un equipo en una competición específica
void resultadosEquipoCompeticion()
{
    std::string equipo, competicion;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    std::cout << "Ingrese la competición: ";
    std::getline(std::cin, competicion);

    incrementConditional(); // if (partidosPorCompeticion.find(competicion) != partidosPorCompeticion.end())
    if (partidosPorCompeticion.find(competicion) == partidosPorCompeticion.end())
    {
        std::cerr << "Error: Competición no encontrada.\n";
        return;
    }

    const std::vector<Partido *> &partidos = partidosPorCompeticion[competicion];
    std::cout << "Resultados de " << equipo << " en " << competicion << ":\n";
    for (const auto &partido : partidos)
    {
        incrementConditional(); // if (partido->equipoLocal == equipo || partido->equipoVisitante == equipo)
        if (partido->equipoLocal == equipo || partido->equipoVisitante == equipo)
        {
            std::cout << "  " << partido->equipoLocal << " " << partido->golesLocal
                      << " - " << partido->golesVisitante << " " << partido->equipoVisitante
                      << " | Fecha: " << timePointToString(partido->fecha) << "\n";
        }
    }
}

// Función para resultados de un equipo entre dos fechas
void resultadosEquipoFechas()
{
    std::string equipo, fechaInicioStr, fechaFinStr;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el nombre del equipo: ";
    std::getline(std::cin, equipo);

    std::cout << "Ingrese la fecha de inicio (dd/mm/yyyy): ";
    std::getline(std::cin, fechaInicioStr);
    std::chrono::system_clock::time_point fechaInicio = stringToTimePoint(fechaInicioStr);

    std::cout << "Ingrese la fecha de fin (dd/mm/yyyy): ";
    std::getline(std::cin, fechaFinStr);
    std::chrono::system_clock::time_point fechaFin = stringToTimePoint(fechaFinStr);

    // Buscar partidos del equipo entre las fechas
    const std::vector<Partido *> &partidos = partidosPorEquipo[equipo];
    std::cout << "Resultados de " << equipo << " entre " << fechaInicioStr << " y " << fechaFinStr << ":\n";
    for (const auto &partido : partidos)
    {
        incrementConditional(); // if (partido->fecha >= fechaInicio && partido->fecha <= fechaFin)
        if (partido->fecha >= fechaInicio && partido->fecha <= fechaFin)
        {
            std::cout << "  " << timePointToString(partido->fecha) << ": "
                      << partido->equipoLocal << " " << partido->golesLocal
                      << " - " << partido->golesVisitante << " " << partido->equipoVisitante
                      << " | Competición: " << partido->competicion << "\n";
        }
    }
}

// Función para comparación de rendimiento general entre dos equipos
void comparacionRendimientoGeneral()
{
    std::string equipo1, equipo2;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el nombre del primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el nombre del segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Verificar existencia de equipos
    incrementConditional(); // if (estadisticasPorEquipo.find(equipo1) != estadisticasPorEquipo.end())
    if (estadisticasPorEquipo.find(equipo1) == estadisticasPorEquipo.end())
    {
        std::cerr << "Error: " << equipo1 << " no encontrado.\n";
        return;
    }

    incrementConditional(); // if (estadisticasPorEquipo.find(equipo2) != estadisticasPorEquipo.end())
    if (estadisticasPorEquipo.find(equipo2) == estadisticasPorEquipo.end())
    {
        std::cerr << "Error: " << equipo2 << " no encontrado.\n";
        return;
    }

    std::cout << "Comparación de rendimiento general entre " << equipo1 << " y " << equipo2 << ":\n";

    // Obtener todas las competiciones
    std::unordered_map<std::string, bool> competiciones;
    for (const auto &entry : partidosPorCompeticion)
    {
        competiciones[entry.first] = true;
    }

    for (const auto &entry : competiciones)
    {
        const std::string &comp = entry.first;

        // Calcular para equipo1
        int golesAFavor1 = 0, golesEnContra1 = 0;
        const std::vector<Partido *> &partidos1 = partidosPorEquipo[equipo1];
        for (const auto &partido : partidos1)
        {
            if (partido->competicion == comp)
            {
                if (partido->equipoLocal == equipo1)
                {
                    golesAFavor1 += partido->golesLocal;
                    golesEnContra1 += partido->golesVisitante;
                }
                else if (partido->equipoVisitante == equipo1)
                {
                    golesAFavor1 += partido->golesVisitante;
                    golesEnContra1 += partido->golesLocal;
                }
            }
        }

        // Calcular para equipo2
        int golesAFavor2 = 0, golesEnContra2 = 0;
        const std::vector<Partido *> &partidos2 = partidosPorEquipo[equipo2];
        for (const auto &partido : partidos2)
        {
            if (partido->competicion == comp)
            {
                if (partido->equipoLocal == equipo2)
                {
                    golesAFavor2 += partido->golesLocal;
                    golesEnContra2 += partido->golesVisitante;
                }
                else if (partido->equipoVisitante == equipo2)
                {
                    golesAFavor2 += partido->golesVisitante;
                    golesEnContra2 += partido->golesLocal;
                }
            }
        }

        std::cout << "Competición: " << comp << "\n";
        std::cout << "  " << equipo1 << " - Goles a favor: " << golesAFavor1
                  << ", Goles en contra: " << golesEnContra1 << "\n";
        std::cout << "  " << equipo2 << " - Goles a favor: " << golesAFavor2
                  << ", Goles en contra: " << golesEnContra2 << "\n";
    }
}

// Función para comparación de rendimiento particular entre dos equipos
void comparacionRendimientoParticular()
{
    std::string equipo1, equipo2;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el nombre del primer equipo: ";
    std::getline(std::cin, equipo1);

    std::cout << "Ingrese el nombre del segundo equipo: ";
    std::getline(std::cin, equipo2);

    // Buscar partidos entre los dos equipos
    std::vector<Partido *> enfrentamientos;
    const std::vector<Partido *> &partidos1 = partidosPorEquipo[equipo1];
    for (const auto &partido : partidos1)
    {
        incrementConditional(); // if (partido->equipoLocal == equipo2 || partido->equipoVisitante == equipo2)
        if (partido->equipoLocal == equipo2 || partido->equipoVisitante == equipo2)
        {
            enfrentamientos.push_back(partido);
        }
    }

    if (enfrentamientos.empty())
    {
        std::cout << "No hay enfrentamientos entre " << equipo1 << " y " << equipo2 << ".\n";
        return;
    }

    int totalPartidos = enfrentamientos.size();
    int empates = 0, ganados1 = 0, ganados2 = 0;

    for (const auto &partido : enfrentamientos)
    {
        if (partido->golesLocal == partido->golesVisitante)
        {
            empates++;
        }
        else
        {
            std::string ganador = (partido->golesLocal > partido->golesVisitante) ? partido->equipoLocal : partido->equipoVisitante;
            if (ganador == equipo1)
                ganados1++;
            else
                ganados2++;
        }
    }

    std::cout << "Comparación de rendimiento particular entre " << equipo1 << " y " << equipo2 << ":\n";
    std::cout << "  Total de partidos jugados: " << totalPartidos << "\n";
    std::cout << "  Empates: " << empates << "\n";
    std::cout << "  Ganados por " << equipo1 << ": " << ganados1 << "\n";
    std::cout << "  Ganados por " << equipo2 << ": " << ganados2 << "\n";
}

// Función para filtrar equipos por umbral de goles promedio
void filtrarEquiposPorUmbral()
{
    double umbral;
    char tipoFiltro;
    std::cin.ignore(); // Limpiar buffer

    std::cout << "Ingrese el umbral de goles promedio por partido: ";
    std::cin >> umbral;

    std::cout << "Ingrese 'A' para filtrar por arriba o 'B' para por debajo: ";
    std::cin >> tipoFiltro;

    tipoFiltro = toupper(tipoFiltro);

    std::cout << "Equipos que cumplen con el criterio:\n";

    for (const auto &entry : estadisticasPorEquipo)
    {
        const std::string &equipo = entry.first;
        const EstadisticasEquipo &stats = entry.second;

        // Calcular promedio de goles a favor
        double promedio = static_cast<double>(stats.golesMarcados) / stats.partidosJugados;

        // Incrementar contador de condicionales
        incrementConditional(); // if (tipoFiltro == 'A' && promedio >= umbral)
        if ((tipoFiltro == 'A' && promedio >= umbral) ||
            (tipoFiltro == 'B' && promedio <= umbral))
        {

            // Mostrar por competiciones
            std::unordered_map<std::string, double> promedioPorCompeticion;
            std::unordered_map<std::string, int> partidosPorCompeticionLocal;

            for (const auto &partido : partidosPorEquipo[equipo])
            {
                // Sumar los goles marcados por el equipo (local o visitante) en cada competición
                if (partido->equipoLocal == equipo)
                {
                    promedioPorCompeticion[partido->competicion] += partido->golesLocal;
                }
                else if (partido->equipoVisitante == equipo)
                {
                    promedioPorCompeticion[partido->competicion] += partido->golesVisitante;
                }
                partidosPorCompeticionLocal[partido->competicion]++;
            }

            // Calcular y mostrar el promedio por competición
            for (const auto &comp_entry : promedioPorCompeticion)
            {
                const std::string &competicion = comp_entry.first;
                double goles = comp_entry.second;
                int partidosComp = partidosPorCompeticionLocal[competicion];
                double promedioComp = goles / partidosComp;

                // Incrementar contador de condicionales
                incrementConditional(); // if ((tipoFiltro == 'A' && promedioComp >= umbral) ||
                                        //     (tipoFiltro == 'B' && promedioComp <= umbral))
                if ((tipoFiltro == 'A' && promedioComp >= umbral) ||
                    (tipoFiltro == 'B' && promedioComp <= umbral))
                {
                    std::cout << "  " << equipo << " - " << promedioComp << " goles promedio en " << competicion << "\n";
                }
            }
        }
    }
}

// Función para mostrar el submenú de consultas dinámicas
void mostrarSubMenuConsultas()
{
    std::cout << "===== Submenú de Consultas Dinámicas =====\n";
    std::cout << "a. Resultados de un equipo en una competición específica\n";
    std::cout << "b. Resultados de un equipo entre dos fechas\n";
    std::cout << "c. Comparación de rendimiento general entre dos equipos\n";
    std::cout << "d. Comparación de rendimiento particular entre dos equipos\n";
    std::cout << "e. Filtrar equipos por umbral de goles promedio\n";
    std::cout << "f. Volver al Menú Principal\n";
    std::cout << "Seleccione una opción: ";
}

// Función para manejar consultas dinámicas
void manejarConsultasDinamicas()
{
    char opcion;
    do
    {
        mostrarSubMenuConsultas();
        std::cin >> opcion;
        opcion = tolower(opcion);
        switch (opcion)
        {
        case 'a':
            resultadosEquipoCompeticion();
            break;
        case 'b':
            resultadosEquipoFechas();
            break;
        case 'c':
            comparacionRendimientoGeneral();
            break;
        case 'd':
            comparacionRendimientoParticular();
            break;
        case 'e':
            filtrarEquiposPorUmbral();
            break;
        case 'f':
            std::cout << "Volviendo al Menú Principal.\n";
            break;
        default:
            std::cerr << "Opción inválida.\n";
        }
    } while (opcion != 'f');
}

// Función que muestra el submenú para procesar datos del CSV
void menuProcesarDatos(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion) {
    int opcion;
    do {
        std::cout << "\n--- Menú de Procesamiento de Datos ---\n";
        std::cout << "1. Mostrar primeros 5 partidos con mayor cantidad de goles por competición\n";
        std::cout << "2. Mostrar goles a favor y en contra de un equipo\n";
        std::cout << "3. Mostrar promedio de goles a favor y en contra de un equipo\n";
        std::cout << "4. Mostrar cantidad total de derrotas y triunfos de un equipo\n";
        std::cout << "5. Mostrar fecha con más y menos goles de un equipo\n";
        std::cout << "6. Mostrar competición con más goles convertidos\n";
        std::cout << "7. Mostrar equipo con más y menos goles convertidos\n";
        std::cout << "0. Regresar al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarResultadosPrincipales(partidosPorCompeticion); // Asegúrate de ajustar esta función también
                break;
            case 2: {
                std::string equipo;
                std::cout << "Ingrese el nombre del equipo: ";
                std::cin >> equipo;
                mostrarGolesEquipo(partidosPorCompeticion, equipo); // Asegúrate de ajustar esta función también
                break;
            }
            case 3: {
                std::string equipo;
                std::cout << "Ingrese el nombre del equipo: ";
                std::cin >> equipo;
                mostrarPromedioGolesEquipo(partidosPorCompeticion, equipo); // Asegúrate de ajustar esta función también
                break;
            }
            case 4: {
                std::string equipo;
                std::cout << "Ingrese el nombre del equipo: ";
                std::cin >> equipo;
                mostrarResultadosPorCompeticion(partidosPorCompeticion, equipo); // Asegúrate de ajustar esta función también
                break;
            }
            case 5: {
                std::string equipo;
                std::cout << "Ingrese el nombre del equipo: ";
                std::cin >> equipo;
                mostrarFechasGolesPorEquipo(partidosPorCompeticion, equipo); // Asegúrate de ajustar esta función también
                break;
            }
            case 6:
                mostrarCompeticionConMasGoles(partidosPorCompeticion); // Asegúrate de ajustar esta función también
                break;
            case 7:
                mostrarEquiposConMasYMenosGoles(partidosPorCompeticion); // Asegúrate de ajustar esta función también
                break;
            case 0:
                std::cout << "Regresando al menú principal...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

void mostrarGolesEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo)
{
    int golesAFavor = 0, golesEnContra = 0;
    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            if (partido->equipoLocal == equipo)
            {
                golesAFavor += partido->golesLocal;
                golesEnContra += partido->golesVisitante;
            }
            else if (partido->equipoVisitante == equipo)
            {
                golesAFavor += partido->golesVisitante;
                golesEnContra += partido->golesLocal;
            }
        }
    }
    std::cout << "Goles a favor de " << equipo << ": " << golesAFavor << "\n";
    std::cout << "Goles en contra de " << equipo << ": " << golesEnContra << "\n";
}

void mostrarPromedioGolesEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo)
{
    int golesAFavor = 0, golesEnContra = 0, partidosJugados = 0;
    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            if (partido->equipoLocal == equipo || partido->equipoVisitante == equipo)
            {
                partidosJugados++;
                if (partido->equipoLocal == equipo)
                {
                    golesAFavor += partido->golesLocal;
                    golesEnContra += partido->golesVisitante;
                }
                else
                {
                    golesAFavor += partido->golesVisitante;
                    golesEnContra += partido->golesLocal;
                }
            }
        }
    }
    if (partidosJugados > 0)
    {
        std::cout << "Promedio de goles a favor de " << equipo << ": " << (static_cast<double>(golesAFavor) / partidosJugados) << "\n";
        std::cout << "Promedio de goles en contra de " << equipo << ": " << (static_cast<double>(golesEnContra) / partidosJugados) << "\n";
    }
    else
    {
        std::cout << "No se han jugado partidos para " << equipo << ".\n";
    }
}


void mostrarResultadosPorCompeticion(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo)
{
    int triunfos = 0, derrotas = 0;
    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            if (partido->equipoLocal == equipo)
            {
                if (partido->golesLocal > partido->golesVisitante)
                {
                    triunfos++;
                }
                else if (partido->golesLocal < partido->golesVisitante)
                {
                    derrotas++;
                }
            }
            else if (partido->equipoVisitante == equipo)
            {
                if (partido->golesVisitante > partido->golesLocal)
                {
                    triunfos++;
                }
                else if (partido->golesVisitante < partido->golesLocal)
                {
                    derrotas++;
                }
            }
        }
    }
    std::cout << "Triunfos de " << equipo << ": " << triunfos << "\n";
    std::cout << "Derrotas de " << equipo << ": " << derrotas << "\n";
}


void mostrarFechasGolesPorEquipo(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion, const std::string& equipo)
{
    std::map<std::string, int> golesPorFecha;
    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            if (partido->equipoLocal == equipo)
            {
                golesPorFecha[timePointToString(partido->fecha)] += partido->golesLocal;
            }
            else if (partido->equipoVisitante == equipo)
            {
                golesPorFecha[timePointToString(partido->fecha)] += partido->golesVisitante;
            }
        }
    }

    // Encontrar la fecha con más y menos goles
    auto maxFecha = std::max_element(golesPorFecha.begin(), golesPorFecha.end(), [](const auto& a, const auto& b)
                                     { return a.second < b.second; });

    auto minFecha = std::min_element(golesPorFecha.begin(), golesPorFecha.end(), [](const auto& a, const auto& b)
                                     { return a.second < b.second; });

    if (maxFecha != golesPorFecha.end())
    {
        std::cout << "Fecha con más goles de " << equipo << ": " << maxFecha->first << " (Goles: " << maxFecha->second << ")\n";
    }
    else
    {
        std::cout << "No se han registrado goles para " << equipo << ".\n";
    }

    if (minFecha != golesPorFecha.end())
    {
        std::cout << "Fecha con menos goles de " << equipo << ": " << minFecha->first << " (Goles: " << minFecha->second << ")\n";
    }
}


void mostrarCompeticionConMasGoles(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion)
{
    std::map<std::string, int> golesPorCompeticion;
    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            golesPorCompeticion[entry.first] += (partido->golesLocal + partido->golesVisitante);
        }
    }

    auto maxCompeticion = std::max_element(golesPorCompeticion.begin(), golesPorCompeticion.end(), [](const auto& a, const auto& b)
                                           { return a.second < b.second; });

    if (maxCompeticion != golesPorCompeticion.end())
    {
        std::cout << "Competición con más goles convertidos: " << maxCompeticion->first << " (Goles: " << maxCompeticion->second << ")\n";
    }
}

void mostrarEquiposConMasYMenosGoles(const std::unordered_map<std::string, std::vector<Partido*>>& partidosPorCompeticion)
{
    std::map<std::string, int> golesPorEquipo;

    for (const auto &entry : partidosPorCompeticion)
    {
        for (const Partido* partido : entry.second)
        {
            golesPorEquipo[partido->equipoLocal] += partido->golesLocal;
            golesPorEquipo[partido->equipoVisitante] += partido->golesVisitante;
        }
    }

    auto maxEquipo = std::max_element(golesPorEquipo.begin(), golesPorEquipo.end(), [](const auto& a, const auto& b)
                                      { return a.second < b.second; });

    auto minEquipo = std::min_element(golesPorEquipo.begin(), golesPorEquipo.end(), [](const auto& a, const auto& b)
                                      { return a.second < b.second; });

    if (maxEquipo != golesPorEquipo.end())
    {
        std::cout << "Equipo con más goles: " << maxEquipo->first << " (Goles: " << maxEquipo->second << ")\n";
    }
    if (minEquipo != golesPorEquipo.end())
    {
        std::cout << "Equipo con menos goles: " << minEquipo->first << " (Goles: " << minEquipo->second << ")\n";
    }
}


// Función principal
int main()
{
    std::string rutaFija = "./Base_Datos_COMA.csv";

    auto inicioCarga = std::chrono::high_resolution_clock::now();
    bool cargado = cargarDatosDesdeCSV(rutaFija);

    auto finCarga = std::chrono::high_resolution_clock::now();

    if (!cargado)
    {
        return 1;
    }

    auto tiempoCarga = std::chrono::duration_cast<std::chrono::milliseconds>(finCarga - inicioCarga);
    mostrarEficiencia("Carga de Datos desde CSV", tiempoCarga, conditionalCount,
                      "Lectura Secuencial y Uso de HashMaps",
                      "unordered_map, vector");

    // Reiniciar contador de condicionales
    conditionalCount = 0;

    int opcion;
    do
    {
        std::cout << "===== Menú de Gestión de Partidos de Fútbol =====\n";
        std::cout << "1. Mostrar Resultados Principales\n";
        std::cout << "2. Agregar Partido\n";
        std::cout << "3. Eliminar Partido\n";
        std::cout << "4. Modificar Partido\n";
        std::cout << "5. Consultas Dinámicas\n";
        std::cout << "6. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        auto inicioProceso = std::chrono::high_resolution_clock::now();
        switch (opcion)
        {
        case 1:
            menuProcesarDatos(partidosPorCompeticion);
            break;
        case 2:
            agregarPartido();
            break;
        case 3:
            eliminarPartido();
            break;
        case 4:
            modificarPartido();
            break;
        case 5:
            manejarConsultasDinamicas();
            break;
        case 6:
            std::cout << "Saliendo del programa.\n";
            break;
        default:
            std::cerr << "Opción inválida.\n";
        }
        auto finProceso = std::chrono::high_resolution_clock::now();
        auto tiempoProceso = std::chrono::duration_cast<std::chrono::milliseconds>(finProceso - inicioProceso);
        mostrarEficiencia("Proceso Seleccionado", tiempoProceso, conditionalCount,
                          "Algoritmos y Estructuras Utilizadas",
                          "unordered_map, vector, sort");
        conditionalCount = 0; // Reiniciar contador después de cada proceso
    } while (opcion != 6);

    return 0;
}