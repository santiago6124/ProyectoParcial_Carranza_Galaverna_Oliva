#include "../menu/menu.h"
#include "../Globals/globals.h"
#include "../carga_datos/carga_datos.h"
#include "../Estadisticas/estadisticas.h"
#include "../Utilidades/utilidades.h"
#include <iostream>
#include "../menu_modificaciones/menu_modificacion.h"
#include "../menu_procesamiento/menu_procesamiento.h"
#include "../menu_consultas/menu_consultas.h"

// Menú Principal
void menuPrincipal() {
    int opcion = 0;
    do {
        std::cout << "\nMenu Principal:\n";
        std::cout << "1. Procesamiento de datos durante lectura archivo CSV\n";
        std::cout << "2. Modificación de datos y reprocesamiento\n";
        std::cout << "3. Consultas dinámicas\n";
        std::cout << "0. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1:
                menuProcesamientoDeDatos();
                break;
            case 2:
                menuModificacionDeDatos();
                break;
            case 3:
                menuConsultasDinamicas();
                break;
            case 0:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Menú de Procesamiento de Datos
void menuProcesamientoDeDatos() {
    int opcion;
    do {
        std::cout << "\nProcesamiento de Datos:\n";
        std::cout << "1. Top 5 partidos con más goles por competición\n";
        std::cout << "2. Goles a favor y en contra por equipo\n";
        std::cout << "3. Promedio de goles por equipo\n";
        std::cout << "4. Triunfos y derrotas por equipo\n";
        std::cout << "5. Fechas con más y menos goles por equipo\n";
        std::cout << "6. Competición con más goles\n";
        std::cout << "7. Equipos con más y menos goles\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

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
}

// Menú de Modificación de Datos
void menuModificacionDeDatos() {
    int opcion;
    do {
        std::cout << "\nModificación de Datos:\n";
        std::cout << "1. Agregar un partido\n";
        std::cout << "2. Eliminar un partido\n";
        std::cout << "3. Modificar un partido\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

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

// Menú de Consultas Dinámicas
void menuConsultasDinamicas() {
    int opcion;
    do {
        std::cout << "\nConsultas Dinámicas:\n";
        std::cout << "1. Todos los resultados de un equipo en una competición\n";
        std::cout << "2. Resultados de un equipo entre dos fechas\n";
        std::cout << "3. Comparación de rendimiento entre dos equipos\n";
        std::cout << "4. Comparación particular entre dos equipos\n";
        std::cout << "5. Filtrar equipos por umbral de goles promedio\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1:
                listarResultadosEquipoCompeticion();
                break;
            case 2:
                listarResultadosEquipoEntreFechas();
                break;
            case 3:
                compararRendimientoEntreEquipos();
                break;
            case 4:
                compararRendimientoParticularEntreEquipos();
                break;
            case 5:
                filtrarEquiposPorUmbralDeGoles();
                break;
            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}
