#include "../include/SparseMatrix.h"
#include <iostream>

int main() {
  SparseMatrix sm;
  sm.insertarCelda(0, 0, "10");
  sm.insertarCelda(0, 1, "20");
  sm.insertarCelda(1, 0, "texto");
  sm.insertarCelda(1, 1, "30");

  std::cout << "Suma fila 0: " << sm.sumaFila(0) << std::endl;       // 30
  std::cout << "Suma columna 1: " << sm.sumaColumna(1) << std::endl; // 50
  std::cout << "Suma rango (0,0)-(1,1): " << sm.sumaRango(0, 0, 1, 1)
            << std::endl; // 60
  std::cout << "Promedio fila 1: " << sm.promedioFila(1)
            << std::endl; // 30 (solo un valor numérico)
  std::cout << "Max rango: " << sm.maxRango(0, 0, 1, 1) << std::endl; // 30
  std::cout << "Min rango: " << sm.minRango(0, 0, 1, 1) << std::endl; // 10

  sm.eliminarFila(0);
  std::cout << "Celda (0,0): " << sm.consultarCelda(0, 0) << std::endl; // vacío
  std::cout << "Celda (1,0): " << sm.consultarCelda(1, 0) << std::endl; // texto

  sm.eliminarRango(0, 0, 2, 2); // limpia todo
  std::cout << "Suma total: " << sm.sumaRango(0, 0, 2, 2) << std::endl; // 0

  return 0;
}
