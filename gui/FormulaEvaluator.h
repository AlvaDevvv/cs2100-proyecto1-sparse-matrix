#pragma once
#include "../include/SparseMatrix.h"
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

// ============================================================
// FormulaEvaluator
// Evalúa expresiones que empiezan con '='.
// Soporta: +, -, *, /, paréntesis, números literales y
// referencias de celda estilo hoja (A1, B3, AA12).
//
// Uso:
//   FormulaEvaluator eval(punteroAMatriz);
//   string resultado = eval.evaluar("=A1+B2*3");
// ============================================================

class FormulaEvaluator {
public:
  explicit FormulaEvaluator(SparseMatrix *m) : matriz(m) {}

  // Retorna el resultado como string, o "#ERR:..." en caso de error.
  std::string evaluar(const std::string &formula) {
    if (formula.empty() || formula[0] != '=')
      return formula;

    src = formula.substr(1);
    pos = 0;
    try {
      double resultado = parseExpr();
      skipEsp();
      if (pos != (int)src.size())
        return "#ERROR: caracter inesperado";

      // Mostrar sin decimales si es entero
      if (resultado == (long long)resultado)
        return std::to_string((long long)resultado);

      std::ostringstream oss;
      oss << resultado;
      return oss.str();

    } catch (const std::exception &e) {
      return std::string("#ERR: ") + e.what();
    }
  }

  // Convierte "A1" -> fila=0, col=0  /  "B3" -> fila=2, col=1
  static bool parsearRef(const std::string &ref, int &fila, int &col) {
    int i = 0;
    std::string letras, nums;
    while (i < (int)ref.size() && std::isalpha((unsigned char)ref[i]))
      letras += (char)std::toupper((unsigned char)ref[i++]);
    while (i < (int)ref.size() && std::isdigit((unsigned char)ref[i]))
      nums += ref[i++];
    if (letras.empty() || nums.empty() || i != (int)ref.size())
      return false;
    col = 0;
    for (char ch : letras)
      col = col * 26 + (ch - 'A' + 1);
    col -= 1;
    fila = std::stoi(nums) - 1;
    return fila >= 0 && col >= 0;
  }

private:
  SparseMatrix *matriz;
  std::string src;
  int pos = 0;

  void skipEsp() {
    while (pos < (int)src.size() && src[pos] == ' ')
      ++pos;
  }

  // Expr → Term ((+|-) Term)*
  double parseExpr() {
    double val = parseTerm();
    skipEsp();
    while (pos < (int)src.size() && (src[pos] == '+' || src[pos] == '-')) {
      char op = src[pos++];
      double rhs = parseTerm();
      val = (op == '+') ? val + rhs : val - rhs;
      skipEsp();
    }
    return val;
  }

  // Term → Factor ((*|/) Factor)*
  double parseTerm() {
    double val = parseFactor();
    skipEsp();
    while (pos < (int)src.size() && (src[pos] == '*' || src[pos] == '/')) {
      char op = src[pos++];
      double rhs = parseFactor();
      if (op == '/' && rhs == 0.0)
        throw std::runtime_error("division por cero");
      val = (op == '*') ? val * rhs : val / rhs;
      skipEsp();
    }
    return val;
  }

  // Factor → numero | ref_celda | '(' Expr ')'
  double parseFactor() {
    skipEsp();
    if (pos >= (int)src.size())
      throw std::runtime_error("expresion incompleta");

    if (src[pos] == '(') {
      ++pos;
      double val = parseExpr();
      skipEsp();
      if (pos >= (int)src.size() || src[pos] != ')')
        throw std::runtime_error("falta parentesis de cierre");
      ++pos;
      return val;
    }

    if (src[pos] == '-') {
      ++pos;
      return -parseFactor();
    }

    if (std::isdigit((unsigned char)src[pos]) || src[pos] == '.') {
      size_t eaten = 0;
      double val = std::stod(src.substr(pos), &eaten);
      pos += (int)eaten;
      return val;
    }

    if (std::isalpha((unsigned char)src[pos])) {
      std::string token;
      while (pos < (int)src.size() && (std::isalpha((unsigned char)src[pos]) ||
                                       std::isdigit((unsigned char)src[pos])))
        token += (char)std::toupper((unsigned char)src[pos++]);

      int fila, col;
      if (!parsearRef(token, fila, col))
        throw std::runtime_error("referencia invalida: " + token);

      std::string val = matriz->consultarCelda(fila, col);
      if (val.empty())
        throw std::runtime_error("celda vacia: " + token);
      if (val[0] == '=') {
        FormulaEvaluator sub(matriz);
        val = sub.evaluar("=" + val.substr(1));
      }
      try {
        size_t eaten = 0;
        double d = std::stod(val, &eaten);
        if (eaten != val.size())
          throw std::runtime_error("celda no numerica: " + token);
        return d;
      } catch (const std::runtime_error &) {
        throw;
      } catch (...) {
        throw std::runtime_error("celda no numerica: " + token);
      }
    }

    throw std::runtime_error(std::string("caracter inesperado: ") + src[pos]);
  }
};
