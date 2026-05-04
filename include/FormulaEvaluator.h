#pragma once
#include "SparseMatrix.h"
#include <string>
#include <utility>

using namespace std;

class FormulaEvaluator {
private:
    CellProvider* matrix;
    string formula;
    int index;

    void skipBlank();

    double evalExpression();
    double evalTerm();
    double evalFactor();

    double readNumber();
    double readCell();

public:
    FormulaEvaluator(CellProvider* matrix);

    double evaluate(const string& inputFormula);

    static pair<int, int> cellRefToIndex(const string& cellName);
};