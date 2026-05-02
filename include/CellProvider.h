#pragma once
#include <string>



class CellProvider {
public:
    virtual ~CellProvider() = default;
    virtual std::string getCellValue(int fila, int col) = 0;
};
