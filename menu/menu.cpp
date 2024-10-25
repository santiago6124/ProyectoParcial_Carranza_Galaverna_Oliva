#include "../menu/menu.h"
#include "../Globals/globals.h"
#include "../carga_datos/carga_datos.h"
#include "../Estadisticas/estadisticas.h"
#include "../Utilidades/utilidades.h"
#include <iostream>
#include "../menu_modificaciones/menu_modificacion.h"
#include "../menu_procesamiento/menu_procesamiento.h"
#include "../menu_consultas/menu_consultas.h"

// Menu Principal
void menuPrincipal()
{
    int opcion = 0;
    do
    {
        cout << "\nMenu Principal:\n";
        cout << "1. Procesamiento de datos durante lectura archivo CSV\n";
        cout << "2. Modificacion de datos y reprocesamiento\n";
        cout << "3. Consultas dinamicas\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion)
        {
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
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Menu de Procesamiento de Datos
void menuProcesamientoDeDatos()
{
    int opcion;
    do
    {
        cout << "\nProcesamiento de Datos:\n";
        cout << "1. Top 5 partidos con mas goles por competicion\n";
        cout << "2. Goles a favor y en contra por equipo\n";
        cout << "3. Promedio de goles por equipo\n";
        cout << "4. Triunfos y derrotas por equipo\n";
        cout << "5. Fechas con mas y menos goles por equipo\n";
        cout << "6. Competicion con mas goles\n";
        cout << "7. Equipos con mas y menos goles\n";
        cout << "0. Volver al menu principal\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion)
        {
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
            cout << "Volviendo al menu principal...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Menu de Modificacion de Datos
void menuModificacionDeDatos()
{
    int opcion;
    do
    {
        cout << "\nModificacion de Datos:\n";
        cout << "1. Agregar un partido\n";
        cout << "2. Eliminar un partido\n";
        cout << "3. Modificar un partido\n";
        cout << "0. Volver al menu principal\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion)
        {
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
            cout << "Volviendo al menu principal...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}

// Menu de Consultas Dinamicas
void menuConsultasDinamicas()
{
    int opcion;
    do
    {
        cout << "\nConsultas Dinamicas:\n";
        cout << "1. Todos los resultados de un equipo en una competicion\n";
        cout << "2. Resultados de un equipo entre dos fechas\n";
        cout << "3. Comparacion de rendimiento entre dos equipos\n";
        cout << "4. Comparacion particular entre dos equipos\n";
        cout << "5. Filtrar equipos por umbral de goles promedio\n";
        cout << "0. Volver al menu principal\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion)
        {
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
            cout << "Volviendo al menu principal...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);
}
