#include "../include/SparseMatrix.h"
#include <algorithm> // swap
#include <string>

using namespace std;

// Función auxiliar
static bool toDouble(const string &s, double &out) {
  if (s.empty())
    return false;
  try {
    size_t pos;
    out = stod(s, &pos);
    return pos == s.size();
  } catch (...) {
    return false;
  }
}

// Constructor del Nodo
SparseMatrix::Node::Node(int r, int c, const string &v)
    : valor(v), pos_row(r), pos_col(c), next_row(nullptr), next_col(nullptr) {}

// Destructor
SparseMatrix::~SparseMatrix() {
  for (Node *curr : rows) {
    while (curr != nullptr) {
      Node *temp = curr;
      curr = curr->next_col;
      delete temp;
    }
  }
}

// Insertar celda
void SparseMatrix::insertarCelda(int f, int c, const string &valor) {
  if (f < 0 || c < 0)
    return;
  if (valor.empty())
    return;

  if (static_cast<size_t>(f) >= rows.size())
    rows.resize(f + 1, nullptr);
  if (static_cast<size_t>(c) >= cols.size())
    cols.resize(c + 1, nullptr);

  Node *curr = rows[f];
  while (curr && curr->pos_col <= c) {
    if (curr->pos_col == c) {
      curr->valor = valor;
      return;
    }
    curr = curr->next_col;
  }

  Node *nuevo = new Node(f, c, valor);

  Node *prevCol = nullptr;
  curr = rows[f];
  while (curr && curr->pos_col < c) {
    prevCol = curr;
    curr = curr->next_col;
  }
  nuevo->next_col = curr;
  if (!prevCol)
    rows[f] = nuevo;
  else
    prevCol->next_col = nuevo;

  Node *prevRow = nullptr;
  curr = cols[c];
  while (curr && curr->pos_row < f) {
    prevRow = curr;
    curr = curr->next_row;
  }
  nuevo->next_row = curr;
  if (!prevRow)
    cols[c] = nuevo;
  else
    prevRow->next_row = nuevo;
}

// Consultar celda
string SparseMatrix::consultarCelda(int f, int c) const {
  if (f < 0 || c < 0)
    return "";
  if (static_cast<size_t>(f) >= rows.size())
    return "";
  Node *curr = rows[f];
  while (curr && curr->pos_col <= c) {
    if (curr->pos_col == c)
      return curr->valor;
    curr = curr->next_col;
  }
  return "";
}

// Modificar celda
bool SparseMatrix::modificarCelda(int f, int c, const std::string &nuevoValor) {
  if (f < 0 || c < 0)
    return false;
  if (static_cast<size_t>(f) >= rows.size())
    return false;
  Node *curr = rows[f];
  while (curr && curr->pos_col <= c) {
    if (curr->pos_col == c) {
      if (nuevoValor.empty()) {
        eliminarCelda(f, c);
      } else {
        curr->valor = nuevoValor;
      }
      return true;
    }
    curr = curr->next_col;
  }
  return false;
}

// Eliminar celda
bool SparseMatrix::eliminarCelda(int f, int c) {
  if (f < 0 || c < 0)
    return false;
  if (static_cast<size_t>(f) >= rows.size() ||
      static_cast<size_t>(c) >= cols.size())
    return false;

  Node *prev = nullptr;
  Node *curr = rows[f];
  while (curr && curr->pos_col < c) {
    prev = curr;
    curr = curr->next_col;
  }
  if (!curr || curr->pos_col != c)
    return false;

  if (prev)
    prev->next_col = curr->next_col;
  else
    rows[f] = curr->next_col;

  Node *prevRow = nullptr;
  Node *currCol = cols[c];
  while (currCol && currCol->pos_row < f) {
    prevRow = currCol;
    currCol = currCol->next_row;
  }
  if (currCol && currCol->pos_row == f) {
    if (prevRow)
      prevRow->next_row = currCol->next_row;
    else
      cols[c] = currCol->next_row;
  }

  delete curr;
  return true;
}

// Eliminar fila
void SparseMatrix::eliminarFila(int f) {
  if (f < 0 || static_cast<size_t>(f) >= rows.size() || !rows[f])
    return;
  Node *curr = rows[f];
  while (curr) {
    int c = curr->pos_col;
    Node *next = curr->next_col;
    if (static_cast<size_t>(c) < cols.size() && cols[c]) {
      Node *prev = nullptr;
      Node *colCurr = cols[c];
      while (colCurr && colCurr->pos_row < f) {
        prev = colCurr;
        colCurr = colCurr->next_row;
      }
      if (colCurr && colCurr->pos_row == f) {
        if (prev)
          prev->next_row = colCurr->next_row;
        else
          cols[c] = colCurr->next_row;
      }
    }
    delete curr;
    curr = next;
  }
  rows[f] = nullptr;
}

// Eliminar columna
void SparseMatrix::eliminarColumna(int c) {
  if (c < 0 || static_cast<size_t>(c) >= cols.size() || !cols[c])
    return;
  Node *curr = cols[c];
  while (curr) {
    int f = curr->pos_row;
    Node *next = curr->next_row;
    if (static_cast<size_t>(f) < rows.size() && rows[f]) {
      Node *prev = nullptr;
      Node *rowCurr = rows[f];
      while (rowCurr && rowCurr->pos_col < c) {
        prev = rowCurr;
        rowCurr = rowCurr->next_col;
      }
      if (rowCurr && rowCurr->pos_col == c) {
        if (prev)
          prev->next_col = rowCurr->next_col;
        else
          rows[f] = rowCurr->next_col;
      }
    }
    delete curr;
    curr = next;
  }
  cols[c] = nullptr;
}

// Eliminar rango
void SparseMatrix::eliminarRango(int f1, int c1, int f2, int c2) {
  if (f1 > f2)
    swap(f1, f2);
  if (c1 > c2)
    swap(c1, c2);
  for (int f = f1; f <= f2; ++f) {
    for (int c = c1; c <= c2; ++c) {
      eliminarCelda(f, c);
    }
  }
}

// Suma fila
double SparseMatrix::sumaFila(int f) const {
  if (f < 0 || static_cast<size_t>(f) >= rows.size())
    return 0.0;
  double suma = 0.0;
  Node *curr = rows[f];
  while (curr) {
    double val;
    if (toDouble(curr->valor, val))
      suma += val;
    curr = curr->next_col;
  }
  return suma;
}

// Suma columna
double SparseMatrix::sumaColumna(int c) const {
  if (c < 0 || static_cast<size_t>(c) >= cols.size())
    return 0.0;
  double suma = 0.0;
  Node *curr = cols[c];
  while (curr) {
    double val;
    if (toDouble(curr->valor, val))
      suma += val;
    curr = curr->next_row;
  }
  return suma;
}

// Suma rango
double SparseMatrix::sumaRango(int f1, int c1, int f2, int c2) const {
  if (f1 > f2)
    swap(f1, f2);
  if (c1 > c2)
    swap(c1, c2);
  double suma = 0.0;
  for (int f = f1; f <= f2; ++f) {
    if (static_cast<size_t>(f) >= rows.size())
      continue;
    Node *curr = rows[f];
    while (curr && curr->pos_col <= c2) {
      if (curr->pos_col >= c1) {
        double val;
        if (toDouble(curr->valor, val))
          suma += val;
      }
      curr = curr->next_col;
    }
  }
  return suma;
}

// Promedio fila
double SparseMatrix::promedioFila(int f) const {
  if (f < 0 || static_cast<size_t>(f) >= rows.size())
    return 0.0;
  double suma = 0.0;
  int count = 0;
  Node *curr = rows[f];
  while (curr) {
    double val;
    if (toDouble(curr->valor, val)) {
      suma += val;
      ++count;
    }
    curr = curr->next_col;
  }
  return count ? suma / count : 0.0;
}

// Promedio columna
double SparseMatrix::promedioColumna(int c) const {
  if (c < 0 || static_cast<size_t>(c) >= cols.size())
    return 0.0;
  double suma = 0.0;
  int count = 0;
  Node *curr = cols[c];
  while (curr) {
    double val;
    if (toDouble(curr->valor, val)) {
      suma += val;
      ++count;
    }
    curr = curr->next_row;
  }
  return count ? suma / count : 0.0;
}

// Promedio rango
double SparseMatrix::promedioRango(int f1, int c1, int f2, int c2) const {
  if (f1 > f2)
    swap(f1, f2);
  if (c1 > c2)
    swap(c1, c2);
  double suma = 0.0;
  int count = 0;
  for (int f = f1; f <= f2; ++f) {
    if (static_cast<size_t>(f) >= rows.size())
      continue;
    Node *curr = rows[f];
    while (curr && curr->pos_col <= c2) {
      if (curr->pos_col >= c1) {
        double val;
        if (toDouble(curr->valor, val)) {
          suma += val;
          ++count;
        }
      }
      curr = curr->next_col;
    }
  }
  return count ? suma / count : 0.0;
}

// Máximo rango
double SparseMatrix::maxRango(int f1, int c1, int f2, int c2) const {
  if (f1 > f2)
    swap(f1, f2);
  if (c1 > c2)
    swap(c1, c2);
  bool encontrado = false;
  double maxVal = 0.0;
  for (int f = f1; f <= f2; ++f) {
    if (static_cast<size_t>(f) >= rows.size())
      continue;
    Node *curr = rows[f];
    while (curr && curr->pos_col <= c2) {
      if (curr->pos_col >= c1) {
        double val;
        if (toDouble(curr->valor, val)) {
          if (!encontrado || val > maxVal) {
            maxVal = val;
            encontrado = true;
          }
        }
      }
      curr = curr->next_col;
    }
  }
  return encontrado ? maxVal : 0.0;
}

// Mínimo rango
double SparseMatrix::minRango(int f1, int c1, int f2, int c2) const {
  if (f1 > f2)
    swap(f1, f2);
  if (c1 > c2)
    swap(c1, c2);
  bool encontrado = false;
  double minVal = 0.0;
  for (int f = f1; f <= f2; ++f) {
    if (static_cast<size_t>(f) >= rows.size())
      continue;
    Node *curr = rows[f];
    while (curr && curr->pos_col <= c2) {
      if (curr->pos_col >= c1) {
        double val;
        if (toDouble(curr->valor, val)) {
          if (!encontrado || val < minVal) {
            minVal = val;
            encontrado = true;
          }
        }
      }
      curr = curr->next_col;
    }
  }
  return encontrado ? minVal : 0.0;
}
