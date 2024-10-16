#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#define NOMBRE_ARCHIVO "Base_Datos_COMA.csv"
using namespace std;

int main()
{
    clock_t begin, end;
    begin = clock(); //empieza a contar el tiempo

    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ',';

    getline(archivo, linea);//Lee encabezado

    
    while (getline(archivo, linea))//lee las otras filas
    {
        stringstream stream(linea); 
        string jornada, fecha, equipoLocal, golesLocal, golesVisitante, equipoVisitante, competicion;
        
        getline(stream, jornada, delimitador);
        getline(stream, fecha, delimitador);
        getline(stream, equipoLocal, delimitador);
        getline(stream, golesLocal, delimitador);
        getline(stream, golesVisitante, delimitador);
        getline(stream, equipoVisitante, delimitador);
        getline(stream, competicion, delimitador);
        

    }

    archivo.close();

    
    end = clock();//corta el tiempo y mide el tiempo de ejecucion
    double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
    cout << "\nTiempo de ejecucion: " << elapsed_secs << " segundos." << endl;

    return 0;
}
