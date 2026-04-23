"""
convergencia.py
Verificación de convergencia del integrador Leapfrog para el CR3BP.

Método 1: Comparación de trayectorias
  Se corre con h, h/2, h/4, h/8 y se mide la distancia
  entre posiciones al mismo instante. Para orden 2 se espera
  que el error escale como h^2.

Método 2: Variación de energía
  Se mide E(t) = T + V a lo largo del tiempo para cada paso.
  En el frame inercial la energía se conserva; en el frame
  rotante varía, pero su fluctuación indica la calidad numérica.
"""

import numpy as np
import matplotlib.pyplot as plt
import os

# -------------------------------------------------------
# Parámetros del sistema
# -------------------------------------------------------
mu      = 0.5
x0, y0  = 2.5, 0.0
vx0, vy0 = 0.0, -1.868
t_final  = 10.0 * 2.0 * np.pi

# -------------------------------------------------------
# Funciones del CR3BP
# -------------------------------------------------------
def r13(x, y): return np.sqrt((x + mu)**2 + y**2)
def r23(x, y): return np.sqrt((x - 1.0 + mu)**2 + y**2)

def accel(x, y, vx, vy):
    r1 = r13(x, y); r2 = r23(x, y)
    ax = 2.0*vy + x - (1-mu)*(x+mu)/r1**3 - mu*(x-1+mu)/r2**3
    ay = -2.0*vx + y - (1-mu)*y/r1**3 - mu*y/r2**3
    return ax, ay

def energia(x, y, vx, vy):
    """Energía cinética + potencial (por unidad de masa de m3)"""
    r1 = r13(x, y); r2 = r23(x, y)
    T = 0.5*(vx**2 + vy**2)
    V = -(1-mu)/r1 - mu/r2
    return T + V

def leapfrog(h, save_every=1):
    """
    Integra con paso h.
    Devuelve arrays de (t, x, y, E) submuestreados cada save_every pasos.
    """
    x, y, vx, vy = x0, y0, vx0, vy0
    N = int(t_final / h)

    ts, xs, ys, Es = [0.0], [x], [y], [energia(x, y, vx, vy)]

    for i in range(N):
        ax0, ay0 = accel(x, y, vx, vy)
        vxh = vx + 0.5*h*ax0
        vyh = vy + 0.5*h*ay0
        x  += h*vxh
        y  += h*vyh
        ax1, ay1 = accel(x, y, vxh, vyh)
        vx = vxh + 0.5*h*ax1
        vy = vyh + 0.5*h*ay1

        if (i+1) % save_every == 0:
            ts.append((i+1)*h)
            xs.append(x)
            ys.append(y)
            Es.append(energia(x, y, vx, vy))

    return np.array(ts), np.array(xs), np.array(ys), np.array(Es)

# -------------------------------------------------------
# Correr con 4 pasos distintos
# -------------------------------------------------------
h_base  = 2.0 * np.pi / 500
pasos   = [h_base, h_base/2, h_base/4, h_base/8]
labels  = [r"$h$", r"$h/2$", r"$h/4$", r"$h/8$"]
colores = ["#ff6b6b", "#ffa94d", "#69db7c", "#00cfff"]

# Para comparar trayectorias necesitamos los mismos instantes
# Usamos el paso más grueso como referencia de tiempos comunes
print("Integrando con distintos pasos...")
resultados = []
N_base = int(t_final / h_base)
for k, h in enumerate(pasos):
    factor = 2**k  # cuántos pasos finos por cada paso grueso
    ts, xs, ys, Es = leapfrog(h, save_every=factor)
    resultados.append((h, ts, xs, ys, Es))
    print(f"  h={h:.5f}  N={int(t_final/h)}  puntos guardados={len(ts)}")

# Truncar todos al mismo número de puntos (el más corto)
n_min = min(len(r[1]) for r in resultados)
resultados = [(h, ts[:n_min], xs[:n_min], ys[:n_min], Es[:n_min])
              for (h, ts, xs, ys, Es) in resultados]

t_ref = resultados[0][1]  # tiempos comunes

# -------------------------------------------------------
# Método 1: distancia entre trayectorias
# Comparamos cada solución contra la más fina (h/8)
# -------------------------------------------------------
print("\nMétodo 1 — Error de trayectoria (vs solución más fina h/8):")
print(f"{'Paso':>8}  {'Error máx':>14}  {'Razón':>8}")
print("-" * 36)

x_ref = resultados[-1][2]
y_ref = resultados[-1][3]
errores_tray = []

for k, (h, ts, xs, ys, Es) in enumerate(resultados[:-1]):
    dist = np.sqrt((xs - x_ref)**2 + (ys - y_ref)**2)
    err_max = np.max(dist)
    errores_tray.append((h, err_max, dist))
    if k == 0:
        razon = "—"
    else:
        razon = f"{errores_tray[k-1][1] / err_max:.2f}"
    print(f"{h:>8.5f}  {err_max:>14.6e}  {razon:>8}")

print("Se espera razón ≈ 4 para Leapfrog (orden 2).")

# -------------------------------------------------------
# Método 2: variación de energía
# -------------------------------------------------------
print("\nMétodo 2 — Variación de energía E(t) = T + V:")
print(f"{'Paso':>8}  {'ΔE máx':>14}")
print("-" * 26)

for h, ts, xs, ys, Es in resultados:
    dE = np.max(np.abs(Es - Es[0]))
    print(f"{h:>8.5f}  {dE:>14.6e}")

# -------------------------------------------------------
# Figura
# -------------------------------------------------------
fig, axes = plt.subplots(1, 2, figsize=(14, 6))
fig.patch.set_facecolor("#0d0d1a")

# --- Panel izquierdo: error de trayectoria vs tiempo ---
ax1 = axes[0]
ax1.set_facecolor("#0d0d1a")

for k, (h, err_max, dist) in enumerate(errores_tray):
    ax1.semilogy(t_ref[:len(dist)], dist,
                 color=colores[k], linewidth=1.1,
                 label=labels[k] + f"  (h={h:.4f})", alpha=0.9)

ax1.set_xlabel("Tiempo [adim.]", color="white", fontsize=11)
ax1.set_ylabel(r"$\sqrt{(x_h - x_{h/8})^2 + (y_h - y_{h/8})^2}$",
               color="white", fontsize=11)
ax1.set_title("Método 1: Error de trayectoria\nvs solución de referencia $h/8$",
              color="white", fontsize=12)
ax1.tick_params(colors="white")
for spine in ax1.spines.values():
    spine.set_edgecolor("#444466")
ax1.legend(facecolor="#1a1a2e", edgecolor="#444466",
           labelcolor="white", fontsize=9)
ax1.grid(True, color="#222244", linewidth=0.4, linestyle="--")

# Anotación de la razón esperada
hs_plot   = np.array([r[0] for r in errores_tray])
errs_plot = np.array([r[1] for r in errores_tray])

# --- Panel derecho: variación de energía vs tiempo ---
ax2 = axes[1]
ax2.set_facecolor("#0d0d1a")

for k, (h, ts, xs, ys, Es) in enumerate(resultados):
    dE = np.abs(Es - Es[0])
    ax2.semilogy(ts, dE + 1e-16,
                 color=colores[k], linewidth=1.1,
                 label=labels[k] + f"  (h={h:.4f})", alpha=0.9)

ax2.set_xlabel("Tiempo [adim.]", color="white", fontsize=11)
ax2.set_ylabel(r"$|E(t) - E(0)|$", color="white", fontsize=11)
ax2.set_title("Método 2: Variación de energía $E = T + V$\nvs tiempo",
              color="white", fontsize=12)
ax2.tick_params(colors="white")
for spine in ax2.spines.values():
    spine.set_edgecolor("#444466")
ax2.legend(facecolor="#1a1a2e", edgecolor="#444466",
           labelcolor="white", fontsize=9)
ax2.grid(True, color="#222244", linewidth=0.4, linestyle="--")

plt.tight_layout()
out_path = os.path.join(os.path.dirname(__file__), "../results/convergencia.png")
plt.savefig(out_path, dpi=150, bbox_inches="tight",
            facecolor=fig.get_facecolor())
print(f"\nGráfica guardada en results/convergencia.png")
plt.show()

