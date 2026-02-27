import serial
import time
import sys

serialName = "COM13"   # <-- поменяй на свой COM
serialBaudRate = 115200

print("=== BLINK SCRIPT START ===")

try:
    ser = serial.Serial(serialName, baudrate=serialBaudRate, timeout=1)
except Exception as e:
    print("ERROR opening port:", e)
    input("Press Enter to exit...")
    sys.exit(1)

print("Connected to", serialName)
print("Введите период мигания в миллисекундах:")

try:
    period_ms = int(input("Period (ms): "))
    period_sec = period_ms / 1000.0

    print("Blinking... Press Ctrl+C to stop")

    while True:
        ser.write(b"e")
        time.sleep(period_sec)

        ser.write(b"d")
        time.sleep(period_sec)

except KeyboardInterrupt:
    print("\nStopped")

finally:
    ser.close()
    print("COM closed")