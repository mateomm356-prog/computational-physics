import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import maxwell

# === Parámetros ===
archivo = "results/datos.txt"
salida = "results/histograma_velocidades.png"
bins = 40  # número de barras en el histograma

# === Cargar los datos ===
data = np.loadtxt(archivo)
t = data[:, 0]
num_pepas = (data.shape[1] - 1) // 4  # columnas por pepa

# === Calcular magnitud de velocidad ===
velocidades = []
for i in range(num_pepas):
    vx = data[:, 1 + 4*i + 2]
    vy = data[:, 1 + 4*i + 3]
    v = np.sqrt(vx**2 + vy**2)
    velocidades.append(v)

velocidades = np.concatenate(velocidades)

# === Histograma ===
plt.figure(figsize=(8, 5))
conteo, bordes, _ = plt.hist(
    velocidades,
    bins=bins,
    color='steelblue',
    edgecolor='black',
    alpha=0.8,
    density=True,  # normaliza el histograma para compararlo con la distribución teórica
    label='Datos simulados'
)

# === Distribución de Maxwell-Boltzmann ===
scale = np.sqrt(np.mean(velocidades**2) / 2)
x = np.linspace(0, np.max(velocidades), 400)
y = maxwell.pdf(x, scale=scale)

plt.plot(x, y, color='red', linewidth=2.5, alpha=0.6, label='Distribución Maxwell–Boltzmann')

# === Estética ===
plt.xlabel("Magnitud de la velocidad |v|", fontsize=12)
plt.ylabel("Densidad de probabilidad", fontsize=12)
plt.title(f"Distribución de velocidades ({num_pepas} partículas)", fontsize=13)
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()

# === Guardar ===
plt.savefig(salida, dpi=300)
plt.show()

print(f"✅ Histograma guardado en {salida}")

