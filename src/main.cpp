#include "../include/SparseMatrix.h"
#include <iostream>
int main() {
  SparseMatrix sm;
  sm.insertarCelda(0, 0, "42");
  std::cout << sm.consultarCelda(0, 0)
            << std::endl; // debería imprimir "42" cuando implementes
  return 0;
}
