// utilidades.cpp
#include "utilidades.h"
#include <algorithm>
#include <cctype>
#include <iostream>

// Convertir una cadena a minusculas
string toLower(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c)
              { return tolower(c); });
    return result;
}

// Verificar si una cadena es numerica
bool esNumerico(const string &str)
{
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}
bool validarFecha(const string &fecha)
{
    if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/')
        return false;
    string dia = fecha.substr(0, 2);
    string mes = fecha.substr(3, 2);
    string anio = fecha.substr(6, 4);
    return esNumerico(dia) && esNumerico(mes) && esNumerico(anio);
}

int ingresarEnteroPositivo(const string &mensaje)
{
    int valor;
    do
    {
        cout << mensaje;
        cin >> valor;
        cin.ignore(); // Limpiar el buffer de entrada
        if (valor < 0)
            cout << "Por favor, ingrese un numero positivo.\n";
    } while (valor < 0);
    return valor;
}
// Funcion auxiliar: Extraer el año de una fecha en formato DD/MM/YYYY
int extraerAnio(const string &fecha)
{
    return stoi(fecha.substr(6, 4)); // Extraer los ultimos 4 caracteres (el año)
}