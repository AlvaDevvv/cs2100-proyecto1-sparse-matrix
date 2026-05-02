#pragma once
#include <string>

// GUI esta usa MockSparseMatrix

class SparseMatrix {
public:
    SparseMatrix();
    ~SparseMatrix();

    // No permitir copia (manejo manual de memoria)
    SparseMatrix(const SparseMatrix&)            = delete;
    SparseMatrix& operator=(const SparseMatrix&) = delete;

    // Operaciones básicas de celda

    // Inserta o actualiza el valor en (fila, col).
    // Si valor está vacío, equivale a eliminar la celda.
    void insertarCelda(int fila, int col, const std::string& valor);

    // Retorna el valor almacenado, o "" si la celda está vacía.
    std::string consultarCelda(int fila, int col) const;

    // Actualiza el valor de una celda existente sin crear nodo nuevo.
    // Si la celda no existe, la crea.
    void modificarCelda(int fila, int col, const std::string& valor);

    // Elimina el nodo ajustando punteros de fila y columna.
    // No hace nada si la celda no existe.
    void eliminarCelda(int fila, int col);

    // ----------------------------------------------------------
    // Operaciones sobre filas y columnas
    // ----------------------------------------------------------

    // Elimina todos los nodos de la fila dada.
    void eliminarFila(int fila);

    // Elimina todos los nodos de la columna dada.
    void eliminarColumna(int col);

    // Elimina todos los nodos en el rango rectangular [f1,f2] x [c1,c2].
    void eliminarRango(int f1, int c1, int f2, int c2);

    // ----------------------------------------------------------
    // Operaciones de agregación
    // ----------------------------------------------------------

    double sumaFila(int fila)                              const;
    double sumaColumna(int col)                            const;
    double sumaRango(int f1, int c1, int f2, int c2)       const;

    double promedioFila(int fila)                          const;
    double promedioColumna(int col)                        const;
    double promedioRango(int f1, int c1, int f2, int c2)   const;

    double maxRango(int f1, int c1, int f2, int c2)        const;
    double minRango(int f1, int c1, int f2, int c2)        const;

private:

    struct Impl;
    Impl* d;
};
