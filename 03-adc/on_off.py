import serial
import sys
import time

serialName = "COM13"   # <-- поменяй на свой COM
serialBaudRate = 115200

print("start")
print("e - LED ON")
print("d - LED OFF")
print("q - quit")
print(f"Opening {serialName}...")

try:
    ser = serial.Serial(serialName, baudrate=serialBaudRate, timeout=1)
except Exception as e:
    print("ERROR opening port:", e)
    input("Press Enter to exit...")
    sys.exit(1)

time.sleep(0.2)
ser.reset_input_buffer()
print("Connected!")

try:
    while True:
        cmd = input("> ").strip()

        if cmd == "q":
            print("bye")
            break

        if not cmd:
            continue

        # отправляем только 1 символ
        ser.write(cmd[0].encode("ascii"))

finally:
    ser.close()
    print("COM closed")