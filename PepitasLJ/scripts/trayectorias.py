import numpy as np
import matplotlib.pyplot as plt
import sys
import os

# =============================
# Entrada: archivo de datos
# =============================
if len(sys.argv) < 2:
    print("Uso: python trayectorias.py results/datos.txt")
    sys.exit(1)

archivo = sys.argv[1]

if not os.path.exists(archivo):
    print(f"❌ ERROR: No se encontró el archivo {archivo}")
    sys.exit(1)

print(f"📄 Leyendo datos de: {archivo}")

# =============================
# Cargar todo el archivo
# =============================
datos = np.loadtxt(archivo)
# datos tiene forma (T, 1 + 4*N)
# columna 0 = tiempo

T, total_cols = datos.shape
N = (total_cols - 1) // 4

print(f"➡️ Tiempos leídos: {T}")
print(f"➡️ Número de partículas detectadas: {N}")

# =============================
# Extraer todas las trayectorias
# =============================
x_tray = []
y_tray = []

for i in range(N):
    colx = 1 + 4 * i
    coly = 2 + 4 * i
    x_tray.append(datos[:, colx])
    y_tray.append(datos[:, coly])

# =============================
# Graficar
# =============================
plt.figure(figsize=(7, 7))

for i in range(N):
    plt.plot(x_tray[i], y_tray[i], linewidth=1)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Trayectorias de todas las partículas")
plt.grid(True)
plt.axis("equal")

# Crear carpeta results si no existe
os.makedirs("results", exist_ok=True)

output = "results/trayectorias.png"
plt.savefig(output, dpi=300)
print(f"✅ Imagen guardada en: {output}")

plt.show()
