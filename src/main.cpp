#include "../include/SparseMatrix.h"
#include <iostream>


int main() {
    SparseMatrix sm;

    sm.insertarCelda(0, 0, "42");
    std::cout << "consultarCelda(0,0): "
              << sm.consultarCelda(0, 0) << std::endl;  // "42"

    sm.modificarCelda(0, 0, "99");
    std::cout << "modificarCelda(0,0) -> consultarCelda(0,0): "
              << sm.consultarCelda(0, 0) << std::endl;  // "99"

    sm.eliminarCelda(0, 0);
    std::cout << "eliminarCelda(0,0) -> consultarCelda(0,0): '"
              << sm.consultarCelda(0, 0) << "'" << std::endl;  // ""

    return 0;
}
