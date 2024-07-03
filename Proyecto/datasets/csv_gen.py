import csv
import random
from datetime import datetime, timedelta

# Definición de las listas para simular las calles y conexiones entre los cruces
Entrada_Norte_A = []
Salida_Norte_A = []
Entrada_Oeste_A = []
Salida_Oeste_A = []

Entrada_Norte_B = []
Salida_Norte_B = []
Entrada_Este_B = []
Salida_Este_B = []

Entrada_Oeste_C = []
Salida_Oeste_C = []
Entrada_Sur_C = []
Salida_Sur_C = []

Entrada_Sur_D = []
Salida_Sur_D = []
Entrada_Este_D = []
Salida_Este_D = []

# Vías compartidas entre los cruces
Via_AE_BO = []  # Calle que va de A este a B oeste
Via_BO_AE = []  # Calle que va de B oeste a A este

Via_AS_CN = []  # Calle que va de A sur a C norte
Via_CN_AS = []  # Calle que va de C norte a A sur

Via_BS_DN = []  # Calle que va de B sur a D norte
Via_DN_BS = []  # Calle que va de D norte a B sur

Via_DO_CE = []  # Calle que va de D oeste a C este
Via_CE_DO = []  # Calle que va de C este a D oeste

# Nombre del archivo CSV a generar
nombre_archivo = "datos_trafico.csv"

# Función para definir las reglas de paso
def permite_paso(desde, hacia):
    # Implementar las reglas según las listas proporcionadas
    if (desde == Via_AS_CN and hacia == Salida_Oeste_A) or \
       (desde == Via_BS_DN and hacia == Salida_Oeste_B) or \
       (desde == Via_DO_CE and hacia == Salida_Norte_C) or \
       (desde == Via_CN_AS and hacia == Salida_Sur_A) or \
       (desde == Via_DN_BS and hacia == Salida_Norte_B) or \
       (desde == Via_CE_DO and hacia == Salida_Oeste_D):
        return False
    else:
        return True

# Escribir los datos al archivo CSV
with open(nombre_archivo, mode='w', newline='') as archivo_csv:
    escritor_csv = csv.writer(archivo_csv)

    # Escribir encabezado
    encabezado = ['Hora', 'Vía', 'Cantidad de Vehículos']
    escritor_csv.writerow(encabezado)

    # Generar datos para cada cruce y cada vía
    tiempo_actual = datetime.strptime("2024-07-03 00:00:00", "%Y-%m-%d %H:%M:%S")
    tiempo_final = tiempo_actual + timedelta(days=1)

    while tiempo_actual < tiempo_final:
        hora_actual = tiempo_actual.strftime("%H:%M")

        # Simular tráfico para cada vía y aplicar reglas de paso
        for vehiculos, v_nombre in [
            (Entrada_Norte_A, "Entrada_Norte_A"), (Salida_Norte_A, "Salida_Norte_A"),
            (Entrada_Oeste_A, "Entrada_Oeste_A"), (Salida_Oeste_A, "Salida_Oeste_A"),
            (Entrada_Norte_B, "Entrada_Norte_B"), (Salida_Norte_B, "Salida_Norte_B"),
            (Entrada_Este_B, "Entrada_Este_B"), (Salida_Este_B, "Salida_Este_B"),
            (Entrada_Oeste_C, "Entrada_Oeste_C"), (Salida_Oeste_C, "Salida_Oeste_C"),
            (Entrada_Sur_C, "Entrada_Sur_C"), (Salida_Sur_C, "Salida_Sur_C"),
            (Entrada_Sur_D, "Entrada_Sur_D"), (Salida_Sur_D, "Salida_Sur_D"),
            (Entrada_Este_D, "Entrada_Este_D"), (Salida_Este_D, "Salida_Este_D"),
            (Via_AE_BO, "Via_AE_BO"), (Via_BO_AE, "Via_BO_AE"),
            (Via_AS_CN, "Via_AS_CN"), (Via_CN_AS, "Via_CN_AS"),
            (Via_BS_DN, "Via_BS_DN"), (Via_DN_BS, "Via_DN_BS"),
            (Via_DO_CE, "Via_DO_CE"), (Via_CE_DO, "Via_CE_DO")
        ]:
            # Limitar la lista a 18 vehículos.
            if len(vehiculos) > 18:
                vehiculos = vehiculos[:18]

            # Generar cantidad aleatoria de vehículos (max 18)
            cantidad_vehiculos = random.randint(0, 18)
            vehiculos.append(cantidad_vehiculos)

            entrada_csv = [hora_actual, v_nombre, cantidad_vehiculos]
            escritor_csv.writerow(entrada_csv)

        # Avanzar al siguiente intervalo de 1 minuto
        tiempo_actual += timedelta(minutes=1)

print(f"Archivo CSV '{nombre_archivo}' generado exitosamente.")
