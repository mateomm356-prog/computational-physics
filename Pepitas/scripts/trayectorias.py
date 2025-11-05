import numpy as np
import matplotlib.pyplot as plt

# === Parámetros ===
archivo = "results/datos.txt"
salida_x = "results/trayectorias_x_t.png"
salida_y = "results/trayectorias_y_t.png"
num_mostrar = 6  # número de partículas a graficar

# === Cargar los datos ===
data = np.loadtxt(archivo)
t = data[:, 0]
num_pepas = (data.shape[1] - 1) // 4  # columnas por pepa

num_mostrar = min(num_mostrar, num_pepas)

# === Graficar x(t) ===
plt.figure(figsize=(8, 5))
for i in range(num_mostrar):
    x = data[:, 1 + 4*i]   # columna de x_i
    plt.plot(t, x, lw=1.5, label=f'Pepa {i+1}')

plt.xlabel("Tiempo t (s)", fontsize=12)
plt.ylabel("Posición x (m)", fontsize=12)
plt.title(f"Trayectorias en x(t) para {num_mostrar} partículas", fontsize=13)
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.savefig(salida_x, dpi=300)
plt.show()

# === Graficar y(t) ===
plt.figure(figsize=(8, 5))
for i in range(num_mostrar):
    y = data[:, 1 + 4*i + 1]  # columna de y_i
    plt.plot(t, y, lw=1.5, label=f'Pepa {i+1}')

plt.xlabel("Tiempo t (s)", fontsize=12)
plt.ylabel("Posición y (m)", fontsize=12)
plt.title(f"Trayectorias en y(t) para {num_mostrar} partículas", fontsize=13)
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.savefig(salida_y, dpi=300)
plt.show()

print(f"✅ Trayectorias guardadas en {salida_x} y {salida_y}")
