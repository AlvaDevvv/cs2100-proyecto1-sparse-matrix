#include "../include/SparseMatrix.h"
#include <iostream>
using namespace std;

int main() {
    SparseMatrix sm;

    sm.insertarCelda(0, 0, "10");
    sm.insertarCelda(0, 1, "20");
    sm.insertarCelda(1, 0, "texto");
    sm.insertarCelda(1, 1, "30");

    cout << "Suma fila 0: "        << sm.sumaFila(0)           << endl; // 30
    cout << "Suma columna 1: "     << sm.sumaColumna(1)        << endl; // 50
    cout << "Suma rango A1:B2: "   << sm.sumaRango(0,0,1,1)   << endl; // 60
    cout << "Promedio fila 1: "    << sm.promedioFila(1)       << endl; // 30
    cout << "Max rango A1:B2: "    << sm.maxRango(0,0,1,1)    << endl; // 30
    cout << "Min rango A1:B2: "    << sm.minRango(0,0,1,1)    << endl; // 10

    sm.eliminarFila(0);
    cout << "Celda A1 tras eliminarFila(0): '" << sm.consultarCelda(0,0) << "'" << endl; // ""
    cout << "Celda A2: "           << sm.consultarCelda(1,0)  << endl; // texto

    sm.eliminarRango(0,0,2,2);
    cout << "Suma total tras eliminarRango: " << sm.sumaRango(0,0,2,2) << endl; // 0

    return 0;
}
