#pragma once
#include <string>
class CellProvider {
public:
  virtual std::string getCellValue(int fila, int columna) = 0;
  virtual ~CellProvider() = default;
};
