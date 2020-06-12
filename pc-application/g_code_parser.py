import nco_calc
import math_utils
import machine_model


def remove_non_content(content):
    if '(' in content:
        start = content.find('(')
        end = content.find(')') + 1
        content = content[0:start] + content[end:]
        content = remove_non_content(content)
        return content
    else:
        return remove_whitespace(content)


def remove_whitespace(content):
    content = content.strip()
    content = ' '.join(content.split())
    return content


def inch_to_mm(inches):
    mm = inches / 25.4
    return mm


def int_to_byte(value, bytes, endian='big'):
    byte_val = value.to_bytes(bytes, endian)
    return byte_val


class Parser:
    PACKET_SIZE = 10

    ABSOLUTE = 0
    RELATIVE = 1

    INCHES = 0
    MILLIMETERS = 1

    END_COMMAND = 0xFF
    HOME_COMMAND = 0x06
    RAISE_COMMAND = 0x07
    LOWER_COMMAND = 0x08

    def __init__(self, max_x, max_y, inc_values, ticks_per_meter):
        self.speed = 0
        self.ticks_per_meter = ticks_per_meter
        self.machine = machine_model.Machine(max_x, max_y)
        self.max_speed = inc_values[-1]

        self.mode = self.ABSOLUTE
        self.units = self.INCHES

        self.instructions = []

        frequency = 64000000

        self.calc = nco_calc.NcoCalculator(ticks_per_meter, frequency, inc_values)

    def set_speed(self, speed):
        self.speed = float(speed)

    def process_travel(self, phrase):
        try:
            travel = float(phrase[1:])
        except ValueError:
            travel = 0

        if self.units is self.INCHES:
            travel = inch_to_mm(travel)

        travel *= self.ticks_per_meter / 1000

        return int(travel)

    def add_special(self, value):
        end_command = int_to_byte(value, 1) * self.PACKET_SIZE
        self.instructions.append(end_command)

    def add_movement(self, x, y):
        command = 0

        if x >= 0:
            command += 1

        if y >= 0:
            command += 2

        command = int_to_byte(command, 1)

        distance = math_utils.get_hypotenuse(x, y)
        distance_byte = distance.to_bytes(2, byteorder='little')

        x_inc, y_inc = self.calc.get_x_y_inc(x, y)

        x_inc = x_inc.to_bytes(3, byteorder='little')
        y_inc = y_inc.to_bytes(3, byteorder='little')

        accel_value = int_to_byte(self.calc.get_accel_value(self.speed), 1)

        packet = command + distance_byte + accel_value + x_inc + y_inc

        self.instructions.append(packet)

    def parse_line(self, line):
        is_movement = False
        x_travel = None
        y_travel = None

        phrases = line.split()

        for phrase in phrases:
            code = phrase[0]
            argument = phrase[1:]

            if code == 'G':
                self.g_command(argument)
                continue

            if code == 'X':
                is_movement = True
                x_travel = self.process_travel(phrase)

            if code == 'Y':
                is_movement = True
                y_travel = self.process_travel(phrase)

            if code == 'F':
                self.set_speed(phrase[1:])

            if code == 'Z':
                value = int(phrase[1:])
                if value == 0:
                    self.add_special(self.LOWER_COMMAND)
                else:
                    self.add_special(self.RAISE_COMMAND)

        if is_movement:
            if self.mode is self.ABSOLUTE:
                x_travel, y_travel = self.machine.move_absolute(x_travel, y_travel)
            else:
                self.machine.move_relative(x_travel, y_travel)

            self.add_movement(x_travel, y_travel)

    def parse_file(self, input_file):
        self.instructions = []
        commands = []

        with open(input_file) as file:
            for line in file:
                commands.append(line)

        for command in commands:
            command = remove_non_content(command)
            if not command:
                continue

            self.parse_line(command)

        self.add_special(self.END_COMMAND)

    def home(self):
        self.machine.home()

    def g_command(self, code):
        code = int(code)

        if code == 0:  # Go max speed
            self.speed = self.max_speed
            return

        if code == 1:  # Feed rate will be provided for speed
            return

        if code == 17:  # XY plane selection
            return

        if code == 20:
            self.units = self.INCHES
            return

        if code == 21:
            self.units = self.MILLIMETERS
            return

        if code == 28:
            self.home()
            self.add_special(self.HOME_COMMAND)
            return

        if code == 90:
            self.mode = self.ABSOLUTE
            return

        if code == 91:
            self.mode = self.RELATIVE
            return

        print("Unknown command G" + str(code))
