
---

## ⚙️ Descripción del Programa

Se simula un oscilador armónico clásico de masa \(m\) y constante elástica \(k\) a una temperatura \(T\).  
El algoritmo propone pasos aleatorios:

\[
x' = x + \delta x, \qquad p' = p + \delta p
\]

donde:

- \(\delta x \in [-D_x, D_x]\)
- \(\delta p \in [-D_p, D_p]\)

y se acepta o rechaza la nueva configuración usando el criterio Metropolis:

\[
A = e^{-\beta (E' - E)}
\]

donde:

\[
\beta = \frac{1}{k_B T}
\]

Los primeros \(N_\text{term}\) pasos se descartan (termalización), y los siguientes \(N_\text{prod}\) se usan para estadísticas.

---

# 🔢 Constantes Físicas Importantes (SI)

Estas constantes se usan en la simulación:

| Constante | Valor | Unidades |
|----------|--------|----------|
| Boltzmann \(k_B\) | \(1.380649\times10^{-23}\) | J/K |
| Masa del protón \(m_p\) | \(1.66053906660\times 10^{-27}\) | kg |
| 1 Joule | \(1\,\text{kg·m}^2/\text{s}^2\) | — |

---

# 🧮 Cálculo de \(D_x\) y \(D_p\)

Para lograr una tasa de aceptación razonable (30–60%), se utilizan las escalas térmicas del oscilador armónico.

## 1. Desviación estándar térmica en **posición**
La distribución teórica de \(x\) es:

\[
P(x) \propto e^{- \frac{k x^2}{2 k_B T}}
\]

Comparando con una gaussiana estándar se obtiene:

\[
\sigma_x = \sqrt{\frac{k_B T}{k}}
\]

## 2. Desviación estándar térmica en **momento**
\[
P(p) \propto e^{- \frac{p^2}{2 m k_B T}}
\]

lo que da:

\[
\sigma_p = \sqrt{m k_B T}
\]

## 3. Elección de los parámetros de Monte Carlo

\[
D_x = c_x\,\sigma_x,\qquad
D_p = c_p\,\sigma_p
\]

donde normalmente:

\[
c_x, c_p \in [0.5, 2.0]
\]

---

# 🧪 Resultados esperados (teoría)

### Distribución teórica de \(x\)
\[
P(x) = \sqrt{\frac{k}{2\pi k_B T}} e^{-kx^2/(2k_B T)}
\]

### Distribución teórica de \(p\)
\[
P(p) = \frac{1}{\sqrt{2\pi m k_B T}} e^{-p^2/(2m k_B T)}
\]

### Energía promedio clásica
\[
\langle E \rangle = k_B T
\]

---

# 🛠️ Makefile: comandos importantes

El proyecto incluye varios comandos auxiliares.

### **Compilar**


make


### **Ejecutar la simulación**


make run

Genera:



results/oscilador_canonico.dat


### **Graficar histogramas de x y p**


make histograma

Genera:


results/hist_xp.png


### **Graficar espacio de fase (scatter de x,p)**


make fase

Genera:


results/espacio_fase.png


make Energia

python3 scripts/HistogramaE.py

Genera:

results/histograma_E.

# 📊 Scripts incluidos

### `scripts/histogramas.py`
- Carga `oscilador_canonico.dat`
- Hace histogramas de \(x\) y \(p\)
- Superpone la teoría

### `scripts/EspacioFase.py`
- Grafica los puntos \((x,p)\) en el espacio de fase

### `scripts/HistogramaE.py`
- Histograma de energía y comparación con la distribución teórica lineal del ensamble canónico

---
