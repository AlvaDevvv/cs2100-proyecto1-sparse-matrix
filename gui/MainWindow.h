#pragma once
#include "../include/SparseMatrix.h"
#include "FormulaEvaluator.h"
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QStatusBar>
#include <QTableWidget>
#include <QToolBar>

// ============================================================
// MainWindow — Ventana principal de la hoja de cálculo.
// Usa SparseMatrix (lista enlazada cruzada) como almacén
// y FormulaEvaluator para expresiones tipo =A1+B2.
// ============================================================

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

private slots:
  void onCellChanged(int row, int col);
  void onCellSelected(int row, int col);
  void onFormulaBarReturnPressed();

  void onSumaFila();
  void onSumaColumna();
  void onSumaRango();
  void onPromedioFila();
  void onPromedioColumna();
  void onPromedioRango();
  void onMaxRango();
  void onMinRango();
  void onEliminarFila();
  void onEliminarColumna();
  void onEliminarRango();
  void onLimpiarTodo();

private:
  void setupUI();
  void setupToolbar();
  void setupTable();
  void connectSignals();

  void syncTableToMatrix();
  void syncCellToMatrix(int row, int col);
  void refreshCell(int row, int col);
  void setStatusMessage(const QString &msg, bool esError = false);

  QString evaluarFormula(const QString &formula);

  bool columnaValida(int c) const;
  struct Rango {
    int f1, c1, f2, c2;
    bool valido;
  };
  Rango parseRango(const QString &texto) const;
  int parseCol(const QString &letras) const;
  QString colToLetras(int col) const;

  // Widgets
  QTableWidget *table = nullptr;
  QLineEdit *formulaBar = nullptr;
  QLabel *cellLabel = nullptr;
  QStatusBar *statusBar = nullptr;

  // Datos: matriz dispersa real (lista enlazada cruzada)
  SparseMatrix matrix;
  FormulaEvaluator evaluator{&matrix};

  bool ignoreCellChanges = false;
  static constexpr int ROWS = 50;
  static constexpr int COLS = 26;
};
