#include "../include/FormulaEvaluator.h"
#include "../include/SparseMatrix.h"
#include <iostream>

using namespace std;

int main() {
  SparseMatrix sm;

  sm.insertarCelda(0, 0, "10");
  sm.insertarCelda(0, 1, "20");
  sm.insertarCelda(1, 0, "texto");
  sm.insertarCelda(1, 1, "30");

  cout << "Suma fila 0: " << sm.sumaFila(0) << endl; // 30

  cout << "Suma columna 1: " << sm.sumaColumna(1) << endl; // 50

  cout << "Suma rango (0,0)-(1,1): " << sm.sumaRango(0, 0, 1, 1) << endl; // 60

  cout << "Promedio fila 1: " << sm.promedioFila(1) << endl; // 30

  cout << "Max rango: " << sm.maxRango(0, 0, 1, 1) << endl; // 30

  cout << "Min rango: " << sm.minRango(0, 0, 1, 1) << endl; // 10

  sm.eliminarFila(0);

  cout << "Celda (0,0): " << sm.consultarCelda(0, 0) << endl;

  cout << "Celda (1,0): " << sm.consultarCelda(1, 0) << endl;

  sm.eliminarRango(0, 0, 2, 2);

  cout << "Suma total: " << sm.sumaRango(0, 0, 2, 2) << endl; // 0

  // =========================================
  // PRUEBAS DE FORMULAEVALUATOR
  // =========================================

  sm.insertarCelda(0, 0, "10"); // A1
  sm.insertarCelda(0, 1, "20"); // B1
  sm.insertarCelda(1, 1, "2");  // B2

  FormulaEvaluator eval(&sm);

  cout << "=A1+B1 -> " << eval.evaluate("=A1+B1") << endl; // 30

  cout << "=A1*B2 -> " << eval.evaluate("=A1*B2") << endl; // 20

  cout << "=(A1+B1)/B2 -> " << eval.evaluate("=(A1+B1)/B2") << endl; // 15

  pair<int, int> coords = FormulaEvaluator::cellRefToIndex("AA12");

  cout << "AA12 -> fila=" << coords.first << ", columna=" << coords.second
       << endl;

  return 0;
}
