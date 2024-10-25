// utilidades.h
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
using namespace std;
string toLower(const string &str);
bool esNumerico(const string &str);
bool validarFecha(const string &fecha);
int ingresarEnteroPositivo(const string &mensaje);
int extraerAnio(const string &fecha);

#endif // UTILIDADES_H
