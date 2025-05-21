import serial
import tkinter as tk
from tkinter import messagebox
import threading
import cv2
import time
import os
import csv

class PinEntry(tk.Tk):
    def __init__(self, serial_port):
        super().__init__()
        self.title("PIN Entry")
        self.geometry("300x450")  
        
        self.display = tk.Entry(self, font=("Arial", 24), show="*", bd=8, relief=tk.RIDGE, justify=tk.CENTER)
        self.display.pack(expand=True, fill=tk.BOTH, padx=10, pady=10)

        self.correct_pin = "1234"
        self.incorrect_attempts = 0
        
        self.ser = serial.Serial(serial_port, 9600)

        self.output_folder = "captures"
        if not os.path.exists(self.output_folder):
            os.makedirs(self.output_folder)

        self.keypad_frame = tk.Frame(self)

        self.create_keypad()

        self.serial_thread = threading.Thread(target=self.listen_to_serial)
        self.serial_thread.daemon = True
        self.serial_thread.start()

    def create_keypad(self):
        buttons = [
            ["1", "2", "3"],
            ["4", "5", "6"],
            ["7", "8", "9"],
            ["C", "0", "Enter"],
        ]

        for row in range(4):
            for col in range(3):
                button = tk.Button(
                    self.keypad_frame,
                    text=buttons[row][col],
                    font=("Arial", 18),
                    command=lambda val=buttons[row][col]: self.on_button_click(val),
                    width=5,
                    height=2,
                )
                button.grid(row=row, column=col, padx=5, pady=5, sticky=tk.NSEW)

        for i in range(4):
            self.keypad_frame.rowconfigure(i, weight=1)
        for j in range(3):
            self.keypad_frame.columnconfigure(j, weight=1)

        close_button = tk.Button(self, text="Close", font=("Arial", 18), command=self.close_servo, width=5, height=2)
        close_button.pack(padx=10, pady=10)

    def on_button_click(self, value):
        if value == "C":
            self.display.delete(0, tk.END)
        elif value == "Enter":
            self.validate_pin()
        else:
            self.display.insert(tk.END, value)

    def validate_pin(self):
        entered_pin = self.display.get()

        if entered_pin == self.correct_pin:
            messagebox.showinfo("Access Granted", "PIN correct!")
            self.send_command_to_arduino("OPEN!")
            self.capture_and_save_photo("Access Granted")

        else:
            messagebox.showerror("Access Denied", "Incorrect PIN. Try again.")
            self.incorrect_attempts += 1

        if self.incorrect_attempts == 3:
            self.send_command_to_arduino("DANGER!")
            self.capture_and_save_photo("Access Denied")

        self.display.delete(0, tk.END)

    def send_command_to_arduino(self, command):
        print(f"Sending command to Arduino: {command}")  # Debugging output
        self.ser.write((command + "\n").encode())

    def close_servo(self):
        self.send_command_to_arduino("CLOSE!")
    
    def write_data_to_files(self, sensor, measurement):
        with open('Sensor_Arduino.csv', 'a', newline='') as file: 
            csv_writer = csv.writer(file)
            csv_writer.writerow([sensor, measurement])

    def listen_to_serial(self):
            self.write_data_to_files("Sensor", "Nilai Sensor")
            while True:
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode("utf-8").strip()
                    print(f"Received from Arduino: {line}")  # Debugging output
                    if ":" in line:
                        sensor, measurement = line.split(":")
                        # print(f"Sensor: {sensor}, Measurement: {measurement}")  # Debugging output
                        self.write_data_to_files(sensor, measurement)
                        if sensor == "PIR" and measurement == "Activate!":
                            self.activate_pin_entry()
                    elif line == "Activate!":
                        self.show_numpad()

    def activate_pin_entry(self):
        self.deiconify()

    def show_numpad(self):
        self.keypad_frame.pack(expand=True, fill=tk.BOTH, padx=10, pady=10)

    def capture_and_save_photo(self, event):
        cap = cv2.VideoCapture(0)
        ret, frame = cap.read()
        cap.release()
        
        if ret:
            timestamp = time.strftime("%Y%m%d-%H%M%S")
            image_path = f"{self.output_folder}/{event}_capture_{timestamp}.jpg"
            cv2.imwrite(image_path, frame)
            print(f"Photo taken and saved as {image_path}")

if __name__ == "__main__":
    serial_port = "COM10"
    app = PinEntry(serial_port)
    app.mainloop()
