// utilidades.h
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>

std::string toLower(const std::string& str);
bool esNumerico(const std::string& str);
bool validarFecha(const std::string& fecha);
int ingresarEnteroPositivo(const std::string& mensaje);
int extraerAnio(const std::string& fecha);

#endif  // UTILIDADES_H
