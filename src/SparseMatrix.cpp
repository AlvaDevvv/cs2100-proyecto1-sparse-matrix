#include "SparseMatrix.h"


struct SparseMatrix::Impl {
    //por implementar
};

SparseMatrix::SparseMatrix()  : d(new Impl()) {}
SparseMatrix::~SparseMatrix() { delete d; }

void        SparseMatrix::insertarCelda(int, int, const std::string&) {}
std::string SparseMatrix::consultarCelda(int, int) const { return ""; }
void        SparseMatrix::modificarCelda(int, int, const std::string&) {}
void        SparseMatrix::eliminarCelda(int, int) {}

void SparseMatrix::eliminarFila(int) {}
void SparseMatrix::eliminarColumna(int) {}
void SparseMatrix::eliminarRango(int,int,int,int) {}

double SparseMatrix::sumaFila(int)                        const { return 0; }
double SparseMatrix::sumaColumna(int)                     const { return 0; }
double SparseMatrix::sumaRango(int,int,int,int)           const { return 0; }
double SparseMatrix::promedioFila(int)                    const { return 0; }
double SparseMatrix::promedioColumna(int)                 const { return 0; }
double SparseMatrix::promedioRango(int,int,int,int)       const { return 0; }
double SparseMatrix::maxRango(int,int,int,int)            const { return 0; }
double SparseMatrix::minRango(int,int,int,int)            const { return 0; }
