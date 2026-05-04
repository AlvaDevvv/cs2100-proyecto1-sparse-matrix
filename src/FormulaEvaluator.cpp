#include "../include/FormulaEvaluator.h"
#include <cctype>
#include <stdexcept>

using namespace std;

FormulaEvaluator::FormulaEvaluator(CellProvider* matrix) {
    this->matrix = matrix;
    index = 0;
}

void FormulaEvaluator::skipBlank() {
    while (index < (int)formula.size() && isspace(formula[index])) {
        index++;
    }
}

double FormulaEvaluator::evaluate(const string& inputFormula) {
    if (inputFormula.empty() || inputFormula[0] != '=') {
        throw runtime_error("La formula debe empezar con =");
    }

    formula = inputFormula.substr(1);
    index = 0;

    double answer = evalExpression();

    skipBlank();

    if (index != (int)formula.size()) {
        throw runtime_error("Formula invalida");
    }

    return answer;
}

double FormulaEvaluator::evalExpression() {
    double leftValue = evalTerm();

    while (true) {
        skipBlank();

        if (index >= (int)formula.size()) break;

        char symbol = formula[index];

        if (symbol != '+' && symbol != '-') break;

        index++;

        double nextValue = evalTerm();

        if (symbol == '+') {
            leftValue += nextValue;
        } else {
            leftValue -= nextValue;
        }
    }

    return leftValue;
}

double FormulaEvaluator::evalTerm() {
    double leftValue = evalFactor();

    while (true) {
        skipBlank();

        if (index >= (int)formula.size()) break;

        char symbol = formula[index];

        if (symbol != '*' && symbol != '/') break;

        index++;

        double nextValue = evalFactor();

        if (symbol == '*') {
            leftValue *= nextValue;
        } else {
            if (nextValue == 0) {
                throw runtime_error("Division por cero");
            }
            leftValue /= nextValue;
        }
    }

    return leftValue;
}

double FormulaEvaluator::evalFactor() {
    skipBlank();

    if (index >= (int)formula.size()) {
        throw runtime_error("Formula incompleta");
    }

    if (formula[index] == '(') {
        index++;

        double inside = evalExpression();

        skipBlank();

        if (index >= (int)formula.size() || formula[index] != ')') {
            throw runtime_error("Falta cerrar parentesis");
        }

        index++;
        return inside;
    }

    if (isdigit(formula[index]) || formula[index] == '.' || formula[index] == '-') {
        return readNumber();
    }

    if (isalpha(formula[index])) {
        return readCell();
    }

    throw runtime_error("Caracter invalido en formula");
}

double FormulaEvaluator::readNumber() {
    skipBlank();

    int beginPos = index;

    if (formula[index] == '-') {
        index++;
    }

    bool foundDigit = false;
    bool foundPoint = false;

    while (index < (int)formula.size()) {
        if (isdigit(formula[index])) {
            foundDigit = true;
            index++;
        } else if (formula[index] == '.') {
            if (foundPoint) break;
            foundPoint = true;
            index++;
        } else {
            break;
        }
    }

    if (!foundDigit) {
        throw runtime_error("Numero invalido");
    }

    return stod(formula.substr(beginPos, index - beginPos));
}

double FormulaEvaluator::readCell() {
    skipBlank();

    int beginPos = index;

    while (index < (int)formula.size() && isalpha(formula[index])) {
        index++;
    }

    while (index < (int)formula.size() && isdigit(formula[index])) {
        index++;
    }

    string cellName = formula.substr(beginPos, index - beginPos);

    pair<int, int> coords = cellRefToIndex(cellName);
    int row = coords.first;
    int col = coords.second;

    string cellValue = matrix->getCellValue(row, col);

    if (cellValue.empty()) {
        throw runtime_error("Referencia vacia: " + cellName);
    }

    try {
        size_t charsUsed = 0;
        double numericValue = stod(cellValue, &charsUsed);

        if (charsUsed != cellValue.size()) {
            throw runtime_error("Celda no numerica: " + cellName);
        }

        return numericValue;
    } catch (...) {
        throw runtime_error("Celda no numerica: " + cellName);
    }
}

pair<int, int> FormulaEvaluator::cellRefToIndex(const string& cellName) {
    int ptr = 0;
    int columnIndex = 0;

    while (ptr < (int)cellName.size() && isalpha(cellName[ptr])) {
        columnIndex = columnIndex * 26 + (toupper(cellName[ptr]) - 'A' + 1);
        ptr++;
    }

    if (columnIndex == 0) {
        throw runtime_error("Referencia invalida: " + cellName);
    }

    int rowIndex = 0;

    while (ptr < (int)cellName.size() && isdigit(cellName[ptr])) {
        rowIndex = rowIndex * 10 + (cellName[ptr] - '0');
        ptr++;
    }

    if (rowIndex == 0 || ptr != (int)cellName.size()) {
        throw runtime_error("Referencia invalida: " + cellName);
    }

    return {rowIndex - 1, columnIndex - 1};
}