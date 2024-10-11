import serial
import time
import keyboard

# Replace 'COMX' with the appropriate serial port of your Bluetooth module
# On Windows, it's usually 'COMX', and on Linux, it might be '/dev/ttyACM0'
ser = serial.Serial('COM4', 9600)  # Adjust the baud rate if needed

def send_data(data):
    try:
        ser.write(data.encode())
        print(f"Sent: {data}")
    except Exception as e:
        print(f"Error sending data: {str(e)}")

def receive_data():
    try:
        while ser.in_waiting:
            data = ser.readline().decode().strip()
            print(f"Received: {data}")
    except Exception as e:
        print(f"Error receiving data: {str(e)}")

try:
    w_key_pressed = False  # Track the state of the 'w' key
    s_key_pressed = False
    a_key_pressed = False
    d_key_pressed = False
    up_key_pressed = False
    down_key_pressed = False

    while True:
        # moving
        if keyboard.is_pressed('w'):
            if not w_key_pressed:
                send_data('w')
                receive_data()
                w_key_pressed = True

        elif keyboard.is_pressed('s'):
            if not s_key_pressed:
                send_data('s')
                receive_data()
                s_key_pressed = True
        else:
            if w_key_pressed or s_key_pressed:
                send_data('0')
                receive_data()
                w_key_pressed = False
                s_key_pressed = False

        # rotating
        if keyboard.is_pressed('a'):
            if not a_key_pressed:
                send_data('a')
                receive_data()
                a_key_pressed = True

        elif keyboard.is_pressed('d'):
            if not d_key_pressed:
                send_data('d')
                receive_data()
                d_key_pressed = True
        else:
            if a_key_pressed or d_key_pressed:
                send_data('1')
                receive_data()
                a_key_pressed = False
                d_key_pressed = False

        # speed control
        if keyboard.is_pressed('up'):
            if not up_key_pressed:
                send_data('2')
                receive_data()
                up_key_pressed = True

        elif keyboard.is_pressed('down'):
            if not down_key_pressed:
                send_data('8')
                receive_data()
                down_key_pressed = True
        else:
            if up_key_pressed or down_key_pressed:
                send_data('5')
                receive_data()
                up_key_pressed = False
                down_key_pressed = False
        if keyboard.is_pressed('q'):
            break

except KeyboardInterrupt:
    print("Keyboard Interrupt. Exiting...")

finally:
    # Send release data for any keys that are still pressed when the program exits
    if w_key_pressed or s_key_pressed:
        send_data('0')
        receive_data()
    if up_key_pressed or down_key_pressed:
        send_data('5')
        receive_data()
    if a_key_pressed or d_key_pressed:
        send_data('1')
        receive_data()
    ser.close()
