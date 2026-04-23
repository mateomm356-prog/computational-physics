# ============================================================
# Makefile — CR3BP Simulación Numérica
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude
SRC      = src/main.cpp
TARGET   = cr3bp

PYTHON   = python3
SCRIPT_GRAFICAR    = scripts/graficar.py
SCRIPT_ANIMAR      = scripts/animar.py
SCRIPT_CONVERGENCIA= scripts/convergencia.py

# -------------------------------------------------------
# make  →  compila y corre la simulación
# -------------------------------------------------------
all: $(TARGET)
	@echo "========================================"
	@echo "  Ejecutando simulación CR3BP..."
	@echo "========================================"
	./$(TARGET)
	@echo "========================================"
	@echo "  Simulación completa."
	@echo "  Resultados en results/trajectory.csv"
	@echo "========================================"

$(TARGET): $(SRC)
	@echo "Compilando..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilado OK."

# -------------------------------------------------------
# make graficar  →  genera la gráfica estática
# -------------------------------------------------------
graficar:
	@echo "Generando gráfica..."
	$(PYTHON) $(SCRIPT_GRAFICAR)

# -------------------------------------------------------
# make animar  →  genera la animación GIF
# -------------------------------------------------------
animar:
	@echo "Generando animación..."
	$(PYTHON) $(SCRIPT_ANIMAR)

espacio_fases:
	$(PYTHON) scripts/espacio_fases.py

# -------------------------------------------------------
# make convergencia  →  genera la gráfica de convergencia
# -------------------------------------------------------
convergencia:
	@echo "Verificando convergencia..."
	$(PYTHON) $(SCRIPT_CONVERGENCIA)

# -------------------------------------------------------
# make todo  →  compila, simula, grafica, anima y converge
# -------------------------------------------------------
todo: all graficar animar convergencia

# -------------------------------------------------------
# make clean  →  limpia binario y resultados
# -------------------------------------------------------
clean:
	@echo "Limpiando..."
	rm -f $(TARGET)
	rm -f results/trajectory.csv
	rm -f results/trayectoria.png
	rm -f results/convergencia.png

.PHONY: all graficar animar convergencia todo clean
