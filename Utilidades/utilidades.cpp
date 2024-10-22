// utilidades.cpp
#include "utilidades.h"
#include <algorithm>
#include <cctype>
#include <iostream>

// Convertir una cadena a minúsculas
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Verificar si una cadena es numérica
bool esNumerico(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
bool validarFecha(const std::string& fecha) {
    if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/') return false;
    std::string dia = fecha.substr(0, 2);
    std::string mes = fecha.substr(3, 2);
    std::string anio = fecha.substr(6, 4);
    return esNumerico(dia) && esNumerico(mes) && esNumerico(anio);
}

int ingresarEnteroPositivo(const std::string& mensaje) {
    int valor;
    do {
        std::cout << mensaje;
        std::cin >> valor;
        std::cin.ignore();  // Limpiar el buffer de entrada
        if (valor < 0) std::cout << "Por favor, ingrese un numero positivo.\n";
    } while (valor < 0);
    return valor;
}
// Funcion auxiliar: Extraer el año de una fecha en formato DD/MM/YYYY
int extraerAnio(const std::string& fecha) {
    return std::stoi(fecha.substr(6, 4));  // Extraer los ultimos 4 caracteres (el año)
}