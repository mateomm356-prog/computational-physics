# ==============================
#       MAKEFILE GENERAL
# ==============================

CXX = g++
CXXFLAGS = -O2 -std=c++17

# ----- Carpetas -----
PENDULO_DIR = PenduloS
POINCARE_DIR = PoincareS
LYAPUNOV_DIR = lyapunov

# ----- Archivos fuente -----
PENDULO_SRC = $(PENDULO_DIR)/PenduloDobleS.cpp
PENDULO_PY = $(PENDULO_DIR)/EspacFase.py

POINCARE_SRC = $(POINCARE_DIR)/Integrador.cpp $(POINCARE_DIR)/Poincare.cpp
POINCARE_PLT = $(POINCARE_DIR)/poincare.plt

LYAPUNOV_SRC = $(LYAPUNOV_DIR)/Lyapunov.cpp
LYAPUNOV_GP = $(LYAPUNOV_DIR)/lyapunov_plot.gp

# ----- PHONY targets -----
.PHONY: all pendulo fase lyapunov poincare clean

# ----- Regla principal -----
all: pendulo fase lyapunov poincare

# -------- PÉNDULO DOBLE --------
pendulo:
	$(CXX) $(CXXFLAGS) -o pendulo $(PENDULO_SRC)
	./pendulo

fase:
	python3 $(PENDULO_PY)

# -------- LYAPUNOV --------
lyapunov:
	$(CXX) $(CXXFLAGS) -o lyapunov_exec $(LYAPUNOV_SRC)
	./lyapunov_exec
	gnuplot $(LYAPUNOV_GP)

# -------- POINCARÉ --------
poincare:
	$(CXX) $(CXXFLAGS) -o poincare $(POINCARE_SRC)
	./poincare
	gnuplot $(POINCARE_PLT)

# -------- LIMPIAR --------
clean:
	rm -f pendulo poincare lyapunov *.dat *.png


