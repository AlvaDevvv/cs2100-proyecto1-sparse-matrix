# SparseSpreadsheet

**Implementación de una hoja de cálculo simple usando matrices dispersas con listas enlazadas cruzadas**

Proyecto Nro. 1 — CS2100: Algoritmos y Estructuras de Datos  
Departamento de Ciencias de la Computación  
Universidad de Ingeniería y Tecnología (UTEC)

---

## Resumen

SparseSpreadsheet es una aplicación de hoja de cálculo funcional desarrollada en C++17 con interfaz gráfica Qt6. Utiliza una **matriz dispersa representada internamente mediante listas enlazadas cruzadas** para almacenar exclusivamente las celdas con contenido, logrando un uso eficiente de memoria incluso en cuadrículas de gran tamaño. El proyecto incluye operaciones completas de inserción, consulta, modificación y eliminación a nivel de celda, fila, columna y rango, así como funciones de agregación (suma, promedio, máximo, mínimo) y un motor de evaluación de fórmulas aritméticas con referencias entre celdas.

---

## Integrantes del Equipo

| Integrante                   | Rol               | Responsabilidades principales                                      |
|------------------------------|-------------------|--------------------------------------------------------------------|
| José Luis Alva Espinoza      | Estructura de datos | Diseño e implementación de la clase `SparseMatrix`, lista enlazada cruzada, operaciones básicas, eliminación de filas/columnas/rangos y funciones de agregación. |
| Valeria Lock Gadea           | Motor de fórmulas   | Desarrollo del parser y evaluador de expresiones aritméticas (`FormulaEvaluator`), conversión de referencias tipo Hoja de Cálculo, manejo de errores en fórmulas. |
| Aaron Adriano Romano Castro  | Interfaz gráfica    | Diseño de la GUI con Qt6, integración con la matriz dispersa, configuración de CMake, documentación del proyecto. |

---

## Requisitos del Sistema

- **Sistema operativo:** Linux (probado en Ubuntu 22.04+, ArchLinux), Windows 10/11, macOS.
- **Compilador:** C++17 o superior (GCC 11+, Clang 13+, MSVC 2022).
- **CMake:** versión 3.16 o superior.
- **Qt:** versión 6.x (módulo `Widgets`). En distribuciones Linux basadas en Debian/Ubuntu se instala con `sudo apt install qt6-base-dev`.

---

## Instrucciones de Compilación y Ejecución

1. **Descomprimir el archivo entregado:**
   ```bash
   unzip Proyecto_Nro1.zip
   cd Proyecto_Nro1
2. Compilar el proyecto:
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build --parallel
3.  Ejecutar la aplicación
    - Interfaz gráfica:
      ./build/spreadsheet_gui

# Estructura del Proyecto

```bash
.
├── CMakeLists.txt                  # Configuración de compilación
├── README.md                       # Documentación del proyecto
├── include/
│   ├── SparseMatrix.h              # Declaración de la matriz dispersa
│   └── CellProvider.h              # Interfaz abstracta para acceso a celdas
├── src/
│   ├── SparseMatrix.cpp            # Implementación de la lista enlazada cruzada
│   ├── FormulaEvaluator.cpp        # Parser y evaluador de fórmulas (versión consola)
│   ├── main.cpp                    # Programa de pruebas unitarias
│   └── main_gui.cpp                # Punto de entrada de la GUI
└── gui/
    ├── MainWindow.h / .cpp         # Ventana principal de la aplicación
    ├── SpreadsheetDelegate.h       # Delegado para edición de celdas
    └── FormulaEvaluator.h          # Motor de fórmulas específico para la GUI
```

---

# Guía de Uso de la Aplicación

## Interfaz Gráfica

La aplicación presenta una ventana que emula una hoja de cálculo convencional:

* Columnas: **A–Z**
* Filas: **1–50 visibles inicialmente** *(sin límite interno)*

---

## Acciones disponibles

| Acción                        | Cómo se realiza                                                |
| ----------------------------- | -------------------------------------------------------------- |
| **Ingresar un valor o texto** | Doble clic en la celda → escribir → <kbd>Enter</kbd>           |
| **Ingresar una fórmula**      | Escribir `=expresión` (ej: `=A1+B2*3`) → <kbd>Enter</kbd>      |
| **Ver contenido bruto**       | Seleccionar la celda → se muestra en la barra superior         |
| **Operaciones de agregación** | Botones: Σ Fila, Σ Col, Σ Rango, Avg Fila, Avg Rango, Máx, Mín |
| **Eliminar fila/col/rango**   | Botones: Del Fila, Del Col, Del Rango                          |
| **Limpiar hoja completa**     | Botón **Limpiar** → confirmar                                  |

---

## Rango de Celdas

Formato estándar:

```
A1:C4
```

Representa un rectángulo desde:

* Columna **A**, fila **1**
* hasta columna **C**, fila **4**

✔ El sistema valida el formato
❌ Si es incorrecto → error en la barra de estado

---

##  Errores en Fórmulas

Cuando una fórmula falla, la celda muestra un error en **rojo**:

| Mensaje                            | Causa                        |
| ---------------------------------- | ---------------------------- |
| `#ERR: celda vacia: X`             | Referencia a celda sin valor |
| `#ERR: celda no numerica: X`       | Contenido no numérico        |
| `#ERR: division por cero`          | División inválida            |
| `#ERR: expresion incompleta`       | Fórmula truncada (`=A1+`)    |
| `#ERR: caracter inesperado: X`     | Símbolo inválido             |
| `#ERR: falta parentesis de cierre` | Paréntesis desbalanceados    |

---

#  Manejo de Casos Borde

Se contemplaron los siguientes escenarios:

### 🔹 Celdas inexistentes

* `consultarCelda` → retorna cadena vacía
* `eliminarCelda` → retorna `false`
* GUI: no muestra errores

---

### 🔹 Eliminación de filas/columnas vacías

* No hay nodos → no se modifica nada
* Sin errores ni bloqueos

---

### 🔹 Agregación sin valores numéricos

* `suma`, `promedio`, `max`, `min` → retornan `0.0`
* GUI muestra `0.0`

---

### 🔹 Inserción de texto

* Se permite cualquier cadena
* Operaciones numéricas ignoran texto (`toDouble`)

---

### 🔹 Referencias entre fórmulas

* Evaluación **recursiva**
* Soporta dependencias encadenadas

---

### 🔹 Entradas inválidas en GUI

* Validación de columnas (**A–Z**)
* Validación de rangos
* Errores en barra de estado (color rojo)

---

#  Pruebas Realizadas

Se ejecutó un plan de pruebas completo:

## 🔹 Operaciones básicas

* Inserción
* Consulta
* Modificación
* Eliminación

## 🔹 Fórmulas

* Evaluación correcta
* Errores sintácticos
* División por cero
* Referencias circulares simples

## 🔹 Agregaciones

* Suma, promedio, máximo y mínimo
* Aplicadas a:

  * Filas
  * Columnas
  * Rangos
* Incluye casos con:

  * Texto
  * Celdas vacías

## 🔹 Eliminaciones

* Filas / columnas / rangos
* Verificación visual e integridad

## 🔹 Casos borde

* Entradas inválidas en GUI
* Eliminaciones redundantes
* Rangos fuera de vista

---

##  Resultado

Todas las pruebas fueron **superadas satisfactoriamente** en la versión final.


