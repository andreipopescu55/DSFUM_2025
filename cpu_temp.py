import wmi
import time
import os

# Conectare la interfața OpenHardwareMonitor
w = wmi.WMI(namespace="D:\Downloads\openhardwaremonitor-v0.9.6\OpenHardwareMonitor\OpenHardwareMonitor.exe")

while True:
    os.system("cls")  # curăță ecranul în Command Prompt
    print("=== Temperatura procesorului ===\n")

    # Parcurge senzorii
    temperature_infos = w.Sensor()
    found = False
    for sensor in temperature_infos:
        if sensor.SensorType == u'Temperature' and 'CPU' in sensor.Name:
            print(f"{sensor.Name}: {sensor.Value:.1f} °C")
            found = True

    if not found:
        print("Nu s-a găsit senzor CPU.")
        print("Asigură-te că OpenHardwareMonitor rulează.")

    time.sleep(2)
