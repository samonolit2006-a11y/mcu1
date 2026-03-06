import time
import serial
import matplotlib.pyplot as plt


PORT = "COM15"
BAUDRATE = 115200


def read_value(ser):
    while True:
        try:
            line = ser.readline().decode('ascii')
            v, t = map(float, line.split())
            return v, t
        except ValueError:
            continue


def main():
    time_values = []
    voltage_values = []
    temp_values = []

    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    time.sleep(2)

    start_time = time.time()

    ser.write("tm_start\n".encode('ascii'))

    try:
        while True:
            voltage_V, temp_C = read_value(ser)

            current_time = time.time() - start_time

            time_values.append(current_time)
            voltage_values.append(voltage_V)
            temp_values.append(temp_C)

            print(f"{current_time:.3f} {voltage_V:.6f} {temp_C:.6f}")

    except KeyboardInterrupt:
        print("Stopped by user")

    finally:
        ser.write("tm_stop\n".encode('ascii'))
        ser.close()

    plt.figure()
    plt.plot(time_values, voltage_values)
    plt.title("График зависимости напряжения от времени")
    plt.xlabel("время, с")
    plt.ylabel("напряжение, В")
    plt.show()

    plt.figure()
    plt.plot(time_values, temp_values)
    plt.title("График зависимости температуры от времени")
    plt.xlabel("время, с")
    plt.ylabel("температура, C")
    plt.show()


if __name__ == "__main__":
    main()