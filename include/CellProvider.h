#pragma once
#include <string>

// Interfaz para que el evaluador de fórmulas pueda
// consultar celdas sin depender de SparseMatrix directamente.
class CellProvider {
public:
    virtual std::string getCellValue(int fila, int columna) = 0;
    virtual ~CellProvider() = default;
};
