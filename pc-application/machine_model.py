class Axis:
    def __init__(self, maximum):
        self.maximum = maximum
        self.current = 0

    def move_absolute(self, position):
        if position is None:
            position = self.current

        if position > self.maximum or position < 0:
            raise ValueError("Requested to move to position " + str(position))
        else:
            retval = position - self.current
            self.current = position
            return retval

    def move_relative(self, travel_distance):
        target_position = self.current + travel_distance
        retval = self.move_absolute(target_position)

        return retval


class Machine:
    def __init__(self, max_x, max_y):
        self.x = Axis(max_x)
        self.y = Axis(max_y)

    def move_relative(self, x, y):
        x_travel = self.x.move_relative(x)
        y_travel = self.y.move_relative(y)

        return x_travel, y_travel

    def move_absolute(self, x, y):
        x_travel = self.x.move_absolute(x)
        y_travel = self.y.move_absolute(y)

        return x_travel, y_travel

    def home(self):
        self.x.move_absolute(0)
        self.y.move_absolute(0)
