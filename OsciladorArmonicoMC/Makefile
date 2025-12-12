# Compilador
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17 -Iinclude

# Carpetas
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
SRCI_DIR = scripts 

# Archivos
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/OsciladorArmonicoC.cpp
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/OsciladorArmonicoC.o
TARGET = $(BIN_DIR)/osciladorMC

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
	@echo ">>> Compilado en $(TARGET)"

# Compilar .cpp → .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/OsciladorArmonicoC.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

# Limpieza total
distclean: clean
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./bin/osciladorMC

histograma:
	python3 scripts/histogramas.py

fase:
	python3 scripts/EspacioFase.py

Energia:
	python3 scripts/Energia.py
