// main.cp
#include "menu/menu.h"
#include "carga_datos/carga_datos.h"
#include <iostream>

int main() {
    std::string archivo = "Base_Datos_COMA.csv";
    cargarDatosDesdeCSV(archivo);

    std::cout << "Bienvenido al sistema de gestion de partidos.\n";
    menuPrincipal();

    return 0;
}
