// main.cp
#include "menu/menu.h"
#include "carga_datos/carga_datos.h"
#include <iostream>

int main() {
    std::string archivo = "Base_Datos_COMA_Normalizado.csv";
    cargarDatosDesdeCSV(archivo);

    std::cout << "Bienvenido al sistema de gestión de partidos.\n";
    menuPrincipal();

    return 0;
}
