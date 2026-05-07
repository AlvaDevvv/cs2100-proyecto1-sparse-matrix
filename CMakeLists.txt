cmake_minimum_required(VERSION 3.16)
project(SparseSpreadsheet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Librería: matriz dispersa
add_library(sparsematrix STATIC
    src/SparseMatrix.cpp
    include/SparseMatrix.h
)
target_include_directories(sparsematrix PUBLIC include)

# Ejecutable de pruebas de consola
add_executable(test_app src/main.cpp)
target_link_libraries(test_app sparsematrix)

# Ejecutable GUI
add_executable(spreadsheet_gui
    src/main_gui.cpp
    gui/MainWindow.cpp
    gui/MainWindow.h
    gui/SpreadsheetDelegate.h
    gui/FormulaEvaluator.h
)
target_include_directories(spreadsheet_gui PRIVATE include gui)
target_link_libraries(spreadsheet_gui PRIVATE sparsematrix Qt6::Widgets)
