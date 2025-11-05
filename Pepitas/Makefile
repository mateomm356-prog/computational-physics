# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17 -Iinclude -I/usr/include/eigen3

# Carpetas
INCLUDE_DIR = include
SRC_DIR = src
SCRIPTS_DIR = scripts
RESULTS_DIR = results

# Archivos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = sim

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

animar: $(EXEC)
	gnuplot $(SCRIPTS_DIR)/animate2D.gnu

# === Nueva regla para generar histogramas ===
histogramas:
	python3 $(SCRIPTS_DIR)/histogramas.py $(RESULTS_DIR)/datos.txt

trayectorias:
	python3 scripts/trayectorias.py

# Limpieza completa
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(EXEC)
	rm -f $(RESULTS_DIR)/*

.PHONY: all run animar histogramas clean

