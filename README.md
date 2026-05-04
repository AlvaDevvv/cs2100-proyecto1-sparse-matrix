# SparseSpreadsheet

Proyecto Nro. 1 — CS2100 | Brenner Ojeda / Heider Sánchez

Hoja de cálculo simple que usa una **matriz dispersa con lista enlazada cruzada** como estructura de almacenamiento interna, en lugar de un arreglo bidimensional normal.

---

## ¿Cómo compilar?

Necesitas tener instalado:
- CMake 3.16 o mayor
- Qt 6 (módulo Widgets)
- Un compilador con soporte C++17 (GCC 11+, Clang 13+ o MSVC 2022)

```bash
# Clonar
git clone <url-del-repo>
cd cs2100-proyecto1-sparse-matrix

# Compilar
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

# Ejecutar la GUI
./build/spreadsheet_gui        # Linux / macOS
build\spreadsheet_gui.exe      # Windows

# Ejecutar pruebas de la matriz (sin GUI)
./build/test_app
```

Si Qt no se detecta automáticamente:
```bash
cmake -B build -DCMAKE_PREFIX_PATH=/ruta/a/Qt/6.x.x/gcc_64
```

---

## Estructura de archivos

```
.
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── SparseMatrix.h      ← interfaz que usamos los 3
│   └── CellProvider.h      ← interfaz para el parser de B
├── src/
│   ├── SparseMatrix.cpp    ← implementación de A
│   ├── main.cpp            ← pruebas de consola
│   └── main_gui.cpp        ← punto de entrada Qt
└── gui/
    ├── MockSparseMatrix.h      ← reemplaza a A mientras desarrollamos
    ├── FormulaEvaluator.h      ← evalúa expresiones tipo =A1+B2
    ├── MainWindow.h / .cpp     ← ventana principal
    └── SpreadsheetDelegate.h   ← maneja la edición de celdas
```

---

## Por qué usamos lista enlazada cruzada

La idea era no desperdiciar memoria. Una hoja de cálculo puede tener millones de celdas pero en la práctica casi ninguna tiene datos. Si usáramos un arreglo `valor[FILAS][COLS]` estaríamos reservando memoria para todas aunque estén vacías.

Con la lista enlazada cruzada solo guardamos los nodos que tienen contenido. Cada nodo tiene punteros al siguiente en su fila y al siguiente en su columna, así:

```cpp
struct Node {
    int    fila, col;
    string valor;
    Node*  nextInRow;
    Node*  nextInCol;
};
```

Por ejemplo, en una hoja de 1000×1000 con solo 1% de celdas ocupadas (10,000 celdas), un arreglo normal usaría ~8 MB. La lista cruzada usa proporcional a esas 10,000 celdas, unas 100 veces menos. s

---

## Complejidad de las operaciones

`k` = celdas no vacías en la fila/columna objetivo, `F` = filas activas, `C` = columnas activas.

| Operación | Complejidad | Por qué |
|---|---|---|
| `insertarCelda` | O(F + C) | Tiene que encontrar la posición en las cabeceras |
| `consultarCelda` | O(F + k) | Busca la fila y luego recorre sus nodos |
| `modificarCelda` | O(F + k) | Igual que consultar |
| `eliminarCelda` | O(F + k) | Busca y desenlaza en fila y columna |
| `eliminarFila` | O(F + k·C) | Borra k nodos ajustando punteros de columna |
| `eliminarColumna` | O(C + k·F) | Lo mismo pero al revés |
| `eliminarRango` | O(F + C + k) | Solo toca los nodos dentro del rango |
| `sumaFila` / `promedioFila` | O(F + k) | Recorre los k nodos de esa fila |
| `sumaRango` / `maxRango` | O(F + C + k) | Recorre los nodos del rectángulo |
| Evaluar `=A1+B2` | O(n) | El parser es lineal sobre la expresión |

---

## Cómo usar la GUI

| Acción | Cómo |
|---|---|
| Escribir un valor | Clic en la celda y escribir, Enter para confirmar |
| Escribir una fórmula | Empezar con `=`, por ejemplo `=A1+B2*3` |
| Ver la fórmula de una celda | Seleccionarla, aparece en la barra de fórmulas arriba |
| Suma, promedio, máx, mín | Botones en la barra de herramientas, pide el rango |
| Eliminar fila o columna | Botones `Del fila` / `Del col` |
| Eliminar un rango | Botón `Del rango` |
| Limpiar todo | Botón `Limpiar` |

Los rangos se escriben así: `A1:C4`, `B2:D10`, `A1:A50`.

### Errores que puede mostrar una celda

| Mensaje | Qué pasó |
|---|---|
| `#ERR: celda vacia` | La fórmula apunta a una celda que no tiene nada |
| `#ERR: celda no numerica` | La celda tiene texto, no un número |
| `#ERR: division por cero` | División por 0 |
| `#ERROR: caracter inesperado` | La fórmula está mal escrita |

---

## Ramas de trabajo

```
main                    ← estable, tiene la interfaz que acordamos
├── feature/matriz      ← A trabaja aquí la lista enlazada real
├── feature/operaciones ← B trabaja aquí las operaciones y el parser
└── feature/gui         ← C trabaja aquí la GUI y la integración
```

Cada uno trabaja en su rama y cuando algo está estable hace PR a main. La GUI usa `MockSparseMatrix` para no depender de nada por el momento