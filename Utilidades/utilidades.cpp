#include "utilidades.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <limits>

// Convertir una cadena a minusculas
string toLower(const string &str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return tolower(c); });
    return result;
}

// Verificar si una cadena es numerica
bool esNumerico(const string &str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

// Validar que la fecha este en formato DD/MM/YYYY y que sea una fecha valida
bool validarFecha(const string &fecha) {
    if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/')
        return false;

    string dia_str = fecha.substr(0, 2);
    string mes_str = fecha.substr(3, 2);
    string anio_str = fecha.substr(6, 4);

    if (!esNumerico(dia_str) || !esNumerico(mes_str) || !esNumerico(anio_str))
        return false;

    int dia = stoi(dia_str);
    int mes = stoi(mes_str);
    int anio = stoi(anio_str);

    // Verificar rangos validos
    if (anio < 1900 || anio > 2100)
        return false;
    if (mes < 1 || mes > 12)
        return false;

    int dias_en_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajustar para años bisiestos
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)))
        dias_en_mes[1] = 29;

    return dia >= 1 && dia <= dias_en_mes[mes - 1];
}

// Solicitar un entero positivo con validación
int ingresarEnteroPositivo(const string &mensaje) {
    int valor;
    string input;

    while (true) {
        cout << mensaje;
        getline(cin, input);

        try {
            valor = stoi(input);
            if (valor >= 0) {
                break;
            } else {
                cout << "Por favor, ingrese un numero positivo.\n";
            }
        } catch (invalid_argument &) {
            cout << "Entrada invalida. Por favor, ingrese un numero valido.\n";
        } catch (out_of_range &) {
            cout << "Numero fuera de rango. Intente nuevamente.\n";
        }
    }

    return valor;
}

// Extraer el año de una fecha en formato DD/MM/YYYY
int extraerAnio(const string &fecha) {
    return stoi(fecha.substr(6, 4)); // Extraer los ultimos 4 caracteres (el año)
}
