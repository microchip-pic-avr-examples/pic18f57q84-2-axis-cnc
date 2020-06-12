import math_utils


class NcoCalculator:
    MAX_INC_VALUE = 2**20 - 1

    def __init__(self, ticks_per_meter, clock_frequency, inc_values):
        self.ticks_per_meter = ticks_per_meter
        self.clock_frequency = clock_frequency
        self.inc_values = inc_values

    def mm_per_second_to_inc_value(self, mm_per_second):
        meters_per_second = mm_per_second / 1000.0
        ticks_per_second = meters_per_second * self.ticks_per_meter
        clocks_per_tick = ticks_per_second / self.clock_frequency
        inc_value = clocks_per_tick * self.MAX_INC_VALUE
        return inc_value

    def get_x_y_inc(self, x_factor, y_factor):
        total_steps = math_utils.get_hypotenuse(x_factor, y_factor)

        x_inc = abs(int(x_factor * self.MAX_INC_VALUE / total_steps))
        y_inc = abs(int(y_factor * self.MAX_INC_VALUE / total_steps))

        return x_inc, y_inc

    def get_accel_value(self, mm_per_second):
        required_inc_value = self.mm_per_second_to_inc_value(mm_per_second)

        target_index = next(x for x, val in enumerate(self.inc_values) if val >= required_inc_value)

        return target_index
