#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

// Funcion para verificar si una cadena es numerica
bool esNumerico(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Funcion para convertir la fecha al formato dd/mm/YYYY
std::string formatearFecha(const std::string& fecha) {
    std::istringstream ss(fecha);
    std::string dia, mes, anio;

    getline(ss, dia, '/');
    getline(ss, mes, '/');
    getline(ss, anio);

    // Asegurar que dia y mes tengan dos digitos
    std::ostringstream fecha_formateada;
    fecha_formateada << std::setw(2) << std::setfill('0') << dia << "/"
                     << std::setw(2) << std::setfill('0') << mes << "/"
                     << anio;

    return fecha_formateada.str();
}

// Funcion para normalizar las fechas en el CSV
void normalizarCSV(const std::string& archivo_entrada, const std::string& archivo_salida) {
    std::ifstream archivo_csv(archivo_entrada);
    std::ofstream archivo_csv_salida(archivo_salida);

    if (!archivo_csv.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada " << archivo_entrada << "\n";
        return;
    }

    if (!archivo_csv_salida.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de salida " << archivo_salida << "\n";
        return;
    }

    std::string linea;
    bool primera_linea = true;

    // Leer y escribir linea por linea
    while (getline(archivo_csv, linea)) {
        std::istringstream ss(linea);
        std::string jornada, fecha, local, golesL, golesV, visitante, competicion;

        // Leer los campos separados por comas
        getline(ss, jornada, ',');
        getline(ss, fecha, ',');
        getline(ss, local, ',');
        getline(ss, golesL, ',');
        getline(ss, golesV, ',');
        getline(ss, visitante, ',');
        getline(ss, competicion, ',');

        // Formatear la fecha si no es la primera linea (encabezado)
        if (!primera_linea) {
            fecha = formatearFecha(fecha);
        }

        // Escribir la linea modificada en el archivo de salida
        archivo_csv_salida << jornada << "," << fecha << "," << local << ","
                           << golesL << "," << golesV << "," << visitante << ","
                           << competicion << "\n";

        primera_linea = false;
    }

    std::cout << "Fechas normalizadas y archivo guardado en " << archivo_salida << ".\n";
}

int main() {
    std::string archivo_entrada = "Base_Datos_COMA.csv";
    std::string archivo_salida = "Base_Datos_COMA_Normalizado.csv";

    normalizarCSV(archivo_entrada, archivo_salida);

    return 0;
}
