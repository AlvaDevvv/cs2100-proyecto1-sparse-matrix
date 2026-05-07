#pragma once
#include <string>
#include <vector>
using namespace std;

class CellProvider {
public:
  virtual string getCellValue(int fila, int columna) = 0;
  virtual ~CellProvider() = default;
};

class SparseMatrix : public CellProvider {
private:
  struct Node {
    string valor;
    int pos_row;
    int pos_col;
    Node *next_row; // siguiente en la misma fila (derecha)
    Node *next_col; // siguiente en la misma columna (abajo)

    Node(int r, int c, const string &v);
  };

  vector<Node *> rows; // crecerá dinámicamente
  vector<Node *> cols;

  // Métodos auxiliares privados
  Node *findInRow(int fila, int columna) const;
  Node *findInCol(int fila, int columna) const;
  void insertIntoRow(Node *nodo);
  void insertIntoCol(Node *nodo);
  void removeFromRow(int fila, int columna);
  void removeFromCol(int fila, int columna);

public:
  SparseMatrix() = default; // constructor por defecto
  ~SparseMatrix();

  // Operaciones básicas de celda
  void insertarCelda(int f, int c, const string &valor);
  string consultarCelda(int f, int c) const;
  bool modificarCelda(int f, int c, const string &nuevoValor);
  bool eliminarCelda(int f, int c);

  // Operaciones sobre filas y columnas
  void eliminarFila(int f);
  void eliminarColumna(int c);
  void eliminarRango(int f1, int c1, int f2, int c2);

  // Agregaciones
  double sumaFila(int f) const;
  double sumaColumna(int c) const;
  double sumaRango(int f1, int c1, int f2, int c2) const;
  double promedioFila(int f) const;
  double promedioColumna(int c) const;
  double promedioRango(int f1, int c1, int f2, int c2) const;
  double maxRango(int f1, int c1, int f2, int c2) const;
  double minRango(int f1, int c1, int f2, int c2) const;

  // Implementación de CellProvider
  string getCellValue(int f, int c) override { return consultarCelda(f, c); }
};
