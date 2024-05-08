# =========================================================
# ------------------------ Bibliotecas --------------------
import pandas as pd
import matplotlib.pyplot as plt

# ------------------------ Carga.csv ----------------------
file_path = 'datasets.csv'
data = pd.read_csv(file_path, header=None, names=['Setpoint', 'PID Output', 'Plant Output'])

# ------------------ Configurar el gráfico-----------------
plt.figure(figsize=(10, 5)) 
plt.plot(data['Setpoint'], label='Setpoint (Temperatura de operación)')
plt.plot(data['PID Output'], label='Salida de PID')
plt.plot(data['Plant Output'], label='Salida de la planta')

plt.title('Gráfico de Control de Temperatura')
plt.xlabel('Tiempo')
plt.ylabel('Valores')

plt.legend()

plt.show()
