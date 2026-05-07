#include "MainWindow.h"
#include "SpreadsheetDelegate.h"
#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>

// ============================================================
//  Constructor
// ============================================================

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("SparseSpreadsheet — CS2100");
  resize(1100, 700);
  setupUI();
  connectSignals();
}

// ============================================================
//  Construcción de la UI
// ============================================================

void MainWindow::setupUI() {
  auto *central = new QWidget(this);
  setCentralWidget(central);
  auto *layout = new QVBoxLayout(central);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  // Barra de fórmulas
  auto *fbar = new QFrame(central);
  fbar->setFixedHeight(34);
  fbar->setStyleSheet("background:#ffffff; border-bottom:1px solid #dadce0;");
  auto *flay = new QHBoxLayout(fbar);
  flay->setContentsMargins(6, 2, 6, 2);
  flay->setSpacing(6);

  cellLabel = new QLabel("A1", fbar);
  cellLabel->setFixedWidth(52);
  cellLabel->setAlignment(Qt::AlignCenter);
  cellLabel->setStyleSheet(
      "font-weight:600; font-size:13px; color:#1a1a1a;"
      "border:1px solid #dadce0; border-radius:3px; padding:2px 4px;"
      "background:#ffffff;");

  auto *fxLbl = new QLabel("fx", fbar);
  fxLbl->setStyleSheet("color:#888; font-style:italic; font-size:13px;");

  formulaBar = new QLineEdit(fbar);
  formulaBar->setPlaceholderText("Ingrese un valor o fórmula  (ej: =A1+B2*3)");
  formulaBar->setStyleSheet("QLineEdit { border:none; background:#ffffff; "
                            "color:#1a1a1a; font-size:13px; padding: 0 4px; }");

  flay->addWidget(cellLabel);
  flay->addWidget(fxLbl);
  flay->addWidget(formulaBar);

  setupToolbar();

  table = new QTableWidget(ROWS, COLS, central);
  setupTable();

  statusBar = new QStatusBar(this);
  setStatusBar(statusBar);
  statusBar->showMessage("Listo");

  layout->addWidget(fbar);
  layout->addWidget(table);
}

void MainWindow::setupToolbar() {
  auto *tb = addToolBar("Operaciones");
  tb->setMovable(false);

  auto add = [&](const QString &txt, const QString &tip, auto slot) {
    auto *a = tb->addAction(txt);
    a->setToolTip(tip);
    connect(a, &QAction::triggered, this, slot);
  };

  add("Σ Fila", "Suma de una fila", &MainWindow::onSumaFila);
  add("Σ Col", "Suma de una columna", &MainWindow::onSumaColumna);
  add("Σ Rango", "Suma de un rango (A1:C4)", &MainWindow::onSumaRango);
  tb->addSeparator();
  add("Avg Fila", "Promedio de una fila", &MainWindow::onPromedioFila);
  add("Avg Col", "Promedio de una columna", &MainWindow::onPromedioColumna);
  add("Avg Rango", "Promedio de un rango", &MainWindow::onPromedioRango);
  tb->addSeparator();
  add("Máx", "Máximo de un rango", &MainWindow::onMaxRango);
  add("Mín", "Mínimo de un rango", &MainWindow::onMinRango);
  tb->addSeparator();
  add("Del Fila", "Eliminar fila", &MainWindow::onEliminarFila);
  add("Del Col", "Eliminar columna", &MainWindow::onEliminarColumna);
  add("Del Rango", "Eliminar rango", &MainWindow::onEliminarRango);
  tb->addSeparator();
  add("Limpiar", "Borrar toda la hoja", &MainWindow::onLimpiarTodo);
}

void MainWindow::setupTable() {
  QStringList cols;
  for (int c = 0; c < COLS; ++c)
    cols << colToLetras(c);
  table->setHorizontalHeaderLabels(cols);

  QStringList rows;
  for (int r = 0; r < ROWS; ++r)
    rows << QString::number(r + 1);
  table->setVerticalHeaderLabels(rows);

  table->setItemDelegate(new SpreadsheetDelegate(table));
  table->horizontalHeader()->setDefaultSectionSize(90);
  table->verticalHeader()->setDefaultSectionSize(22);
  table->setStyleSheet(
      "QTableWidget { color: #1a1a1a; background: #ffffff; gridline-color: "
      "#dadce0; }"
      "QTableWidget::item { color: #1a1a1a; }"
      "QTableWidget::item:selected { background: #d2e3fc; color: #1a1a1a; }"
      "QHeaderView::section { background: #f1f3f4; color: #3c4043; border: "
      "none; border-right: 1px solid #dadce0; border-bottom: 1px solid "
      "#dadce0; }");
  table->setAlternatingRowColors(false);
  table->setSelectionMode(QAbstractItemView::ContiguousSelection);
  table->setEditTriggers(QAbstractItemView::DoubleClicked |
                         QAbstractItemView::AnyKeyPressed);

  ignoreCellChanges = true;
  for (int r = 0; r < ROWS; ++r)
    for (int c = 0; c < COLS; ++c)
      table->setItem(r, c, new QTableWidgetItem(""));
  ignoreCellChanges = false;
}

// ============================================================
//  Señales
// ============================================================

void MainWindow::connectSignals() {
  connect(table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
  connect(table, &QTableWidget::currentCellChanged, this,
          [this](int r, int c, int, int) { onCellSelected(r, c); });
  connect(formulaBar, &QLineEdit::returnPressed, this,
          &MainWindow::onFormulaBarReturnPressed);
}

// ============================================================
//  Slots de celda
// ============================================================

void MainWindow::onCellChanged(int row, int col) {
  if (ignoreCellChanges)
    return;
  syncCellToMatrix(row, col);
}

void MainWindow::onCellSelected(int row, int col) {
  if (row < 0 || col < 0)
    return;
  cellLabel->setText(colToLetras(col) + QString::number(row + 1));
  // Mostrar fórmula/valor raw en la barra
  ignoreCellChanges = true;
  formulaBar->setText(QString::fromStdString(matrix.consultarCelda(row, col)));
  ignoreCellChanges = false;
}

void MainWindow::onFormulaBarReturnPressed() {
  int row = table->currentRow();
  int col = table->currentColumn();
  if (row < 0 || col < 0)
    return;

  QString raw = formulaBar->text();

  // Guardar valor raw en la matriz
  if (raw.isEmpty())
    matrix.eliminarCelda(row, col);
  else
    matrix.insertarCelda(row, col, raw.toStdString());

  // Mostrar resultado en la celda
  QString display = raw.startsWith('=') ? evaluarFormula(raw) : raw;

  ignoreCellChanges = true;
  auto *item = table->item(row, col);
  if (!item) {
    item = new QTableWidgetItem();
    table->setItem(row, col, item);
  }
  item->setText(display);
  bool esError = display.startsWith('#');
  item->setForeground(QBrush(esError ? QColor("#c0392b") : QColor("#1a1a2e")));
  ignoreCellChanges = false;

  if (row + 1 < ROWS)
    table->setCurrentCell(row + 1, col);
}

// ============================================================
//  Slots de operaciones
// ============================================================

void MainWindow::onSumaFila() {
  bool ok;
  int f = QInputDialog::getInt(
              this, "Suma de fila",
              "Número de fila (1-" + QString::number(ROWS) + "):", 1, 1, ROWS,
              1, &ok) -
          1;
  if (!ok)
    return;
  setStatusMessage(
      QString("Σ fila %1 = %2").arg(f + 1).arg(matrix.sumaFila(f)));
}

void MainWindow::onSumaColumna() {
  bool ok;
  QString col = QInputDialog::getText(
      this, "Suma de columna", "Letra de columna (A-Z):", QLineEdit::Normal,
      "A", &ok);
  if (!ok || col.isEmpty())
    return;
  int c = parseCol(col.toUpper());
  if (!columnaValida(c)) {
    setStatusMessage("Columna inválida: " + col.toUpper(), true);
    return;
  }
  setStatusMessage(
      QString("Σ col %1 = %2").arg(col.toUpper()).arg(matrix.sumaColumna(c)));
}

void MainWindow::onSumaRango() {
  bool ok;
  QString txt = QInputDialog::getText(this, "Suma de rango",
                                      "Rango (ej: A1:C4):", QLineEdit::Normal,
                                      "A1:B3", &ok);
  if (!ok || txt.isEmpty())
    return;
  auto r = parseRango(txt);
  if (!r.valido) {
    setStatusMessage("Rango inválido", true);
    return;
  }
  setStatusMessage(QString("Σ %1 = %2")
                       .arg(txt)
                       .arg(matrix.sumaRango(r.f1, r.c1, r.f2, r.c2)));
}

void MainWindow::onPromedioFila() {
  bool ok;
  int f = QInputDialog::getInt(this, "Promedio de fila", "Número de fila:", 1,
                               1, ROWS, 1, &ok) -
          1;
  if (!ok)
    return;
  setStatusMessage(
      QString("Avg fila %1 = %2").arg(f + 1).arg(matrix.promedioFila(f)));
}

void MainWindow::onPromedioColumna() {
  bool ok;
  QString col =
      QInputDialog::getText(this, "Promedio de columna",
                            "Letra de columna:", QLineEdit::Normal, "A", &ok);
  if (!ok || col.isEmpty())
    return;
  int c = parseCol(col.toUpper());
  if (!columnaValida(c)) {
    setStatusMessage("Columna inválida: " + col.toUpper(), true);
    return;
  }
  setStatusMessage(QString("Avg col %1 = %2")
                       .arg(col.toUpper())
                       .arg(matrix.promedioColumna(c)));
}

void MainWindow::onPromedioRango() {
  bool ok;
  QString txt = QInputDialog::getText(this, "Promedio de rango",
                                      "Rango (ej: A1:C4):", QLineEdit::Normal,
                                      "A1:B3", &ok);
  if (!ok || txt.isEmpty())
    return;
  auto r = parseRango(txt);
  if (!r.valido) {
    setStatusMessage("Rango inválido", true);
    return;
  }
  setStatusMessage(QString("Avg %1 = %2")
                       .arg(txt)
                       .arg(matrix.promedioRango(r.f1, r.c1, r.f2, r.c2)));
}

void MainWindow::onMaxRango() {
  bool ok;
  QString txt = QInputDialog::getText(this, "Máximo de rango",
                                      "Rango (ej: A1:C4):", QLineEdit::Normal,
                                      "A1:B3", &ok);
  if (!ok || txt.isEmpty())
    return;
  auto r = parseRango(txt);
  if (!r.valido) {
    setStatusMessage("Rango inválido", true);
    return;
  }
  setStatusMessage(QString("Máx %1 = %2")
                       .arg(txt)
                       .arg(matrix.maxRango(r.f1, r.c1, r.f2, r.c2)));
}

void MainWindow::onMinRango() {
  bool ok;
  QString txt = QInputDialog::getText(this, "Mínimo de rango",
                                      "Rango (ej: A1:C4):", QLineEdit::Normal,
                                      "A1:B3", &ok);
  if (!ok || txt.isEmpty())
    return;
  auto r = parseRango(txt);
  if (!r.valido) {
    setStatusMessage("Rango inválido", true);
    return;
  }
  setStatusMessage(QString("Mín %1 = %2")
                       .arg(txt)
                       .arg(matrix.minRango(r.f1, r.c1, r.f2, r.c2)));
}

void MainWindow::onEliminarFila() {
  bool ok;
  int f = QInputDialog::getInt(this, "Eliminar fila", "Número de fila:", 1, 1,
                               ROWS, 1, &ok) -
          1;
  if (!ok)
    return;
  matrix.eliminarFila(f);
  syncTableToMatrix();
  setStatusMessage(QString("Fila %1 eliminada").arg(f + 1));
}

void MainWindow::onEliminarColumna() {
  bool ok;
  QString col =
      QInputDialog::getText(this, "Eliminar columna",
                            "Letra de columna:", QLineEdit::Normal, "A", &ok);
  if (!ok || col.isEmpty())
    return;
  int c = parseCol(col.toUpper());
  if (!columnaValida(c)) {
    setStatusMessage("Columna inválida: " + col.toUpper(), true);
    return;
  }
  matrix.eliminarColumna(c);
  syncTableToMatrix();
  setStatusMessage(QString("Columna %1 eliminada").arg(col.toUpper()));
}

void MainWindow::onEliminarRango() {
  bool ok;
  QString txt = QInputDialog::getText(this, "Eliminar rango",
                                      "Rango (ej: A1:C4):", QLineEdit::Normal,
                                      "A1:B3", &ok);
  if (!ok || txt.isEmpty())
    return;
  auto r = parseRango(txt);
  if (!r.valido) {
    setStatusMessage("Rango inválido", true);
    return;
  }
  matrix.eliminarRango(r.f1, r.c1, r.f2, r.c2);
  syncTableToMatrix();
  setStatusMessage(QString("Rango %1 eliminado").arg(txt));
}

void MainWindow::onLimpiarTodo() {
  if (QMessageBox::question(this, "Limpiar hoja", "¿Eliminar todos los datos?",
                            QMessageBox::Yes | QMessageBox::No) !=
      QMessageBox::Yes)
    return;
  for (int f = 0; f < ROWS; ++f)
    matrix.eliminarFila(f);
  syncTableToMatrix();
  setStatusMessage("Hoja limpiada");
}

// ============================================================
//  Sincronización tabla <-> matriz
// ============================================================

void MainWindow::syncCellToMatrix(int row, int col) {
  auto *item = table->item(row, col);
  QString raw = item ? item->text() : "";

  if (raw.isEmpty()) {
    matrix.eliminarCelda(row, col);
    return;
  }

  // Guardar el raw (fórmula o valor) en la matriz
  matrix.insertarCelda(row, col, raw.toStdString());

  // Si es fórmula, mostrar el resultado en la celda
  if (raw.startsWith('=')) {
    QString display = evaluarFormula(raw);
    ignoreCellChanges = true;
    item->setText(display);
    bool esError = display.startsWith('#');
    item->setForeground(
        QBrush(esError ? QColor("#c0392b") : QColor("#1a1a2e")));
    ignoreCellChanges = false;
  }
}

void MainWindow::syncTableToMatrix() {
  ignoreCellChanges = true;
  for (int r = 0; r < ROWS; ++r)
    for (int c = 0; c < COLS; ++c)
      refreshCell(r, c);
  ignoreCellChanges = false;
}

void MainWindow::refreshCell(int row, int col) {
  std::string raw = matrix.consultarCelda(row, col);
  QString display =
      raw.empty() ? ""
                  : (raw[0] == '=' ? evaluarFormula(QString::fromStdString(raw))
                                   : QString::fromStdString(raw));

  auto *item = table->item(row, col);
  if (!item) {
    item = new QTableWidgetItem();
    table->setItem(row, col, item);
  }
  item->setText(display);
  item->setForeground(
      QBrush(display.startsWith('#') ? QColor("#c0392b") : QColor("#1a1a1a")));
}

// ============================================================
//  Evaluador de fórmulas
// ============================================================

QString MainWindow::evaluarFormula(const QString &formula) {
  return QString::fromStdString(evaluator.evaluar(formula.toStdString()));
}

// ============================================================
//  Utilidades de coordenadas
// ============================================================

void MainWindow::setStatusMessage(const QString &msg, bool esError) {
  statusBar->setStyleSheet(esError ? "color:red;" : "");
  statusBar->showMessage(msg, 6000);
}

// funcion auxiliar para validar columna
bool MainWindow::columnaValida(int c) const { return c >= 0 && c < COLS; }

int MainWindow::parseCol(const QString &letras) const {
  if (letras.isEmpty())
    return -1;
  int result = 0;
  for (QChar ch : letras) {
    if (!ch.isLetter())
      return -1;
    result = result * 26 + (ch.toUpper().unicode() - 'A' + 1);
  }
  return result - 1;
}

QString MainWindow::colToLetras(int col) const {
  QString s;
  ++col;
  while (col > 0) {
    s.prepend(QChar('A' + (col - 1) % 26));
    col = (col - 1) / 26;
  }
  return s;
}

MainWindow::Rango MainWindow::parseRango(const QString &texto) const {
  QStringList partes = texto.trimmed().toUpper().split(':');
  if (partes.size() != 2)
    return {0, 0, 0, 0, false};

  auto parseCelda = [&](const QString &ref, int &f, int &c) -> bool {
    int i = 0;
    QString letras, nums;
    while (i < ref.size() && ref[i].isLetter())
      letras += ref[i++];
    while (i < ref.size() && ref[i].isDigit())
      nums += ref[i++];
    if (letras.isEmpty() || nums.isEmpty())
      return false;
    c = parseCol(letras);
    f = nums.toInt() - 1;
    return f >= 0 && c >= 0;
  };

  Rango r{0, 0, 0, 0, false};
  if (!parseCelda(partes[0], r.f1, r.c1))
    return r;
  if (!parseCelda(partes[1], r.f2, r.c2))
    return r;
  if (r.f1 > r.f2)
    std::swap(r.f1, r.f2);
  if (r.c1 > r.c2)
    std::swap(r.c1, r.c2);
  r.valido = true;
  return r;
}
