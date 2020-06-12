import serial


def chunkify(contents, chunk_length, debug=False):
    if debug:
        chunk_length = 2

    chunks = []
    for i in range(0, len(contents), chunk_length):
        chunks.append(contents[i:i+chunk_length])

    return chunks


class PicComm:
    INITIAL_CONNECTION_PACKET = [1]
    START_PROGRAM_PACKET = [2]

    def __init__(self, port, baud):
        self.port = port
        self.baud = baud
        self.buffer_size = 0
        self.remaining = []

    def connect(self):
        with serial.Serial(port=self.port, baudrate=self.baud, timeout=3) as ser:
            print("Requesting metadata")
            ser.write(self.INITIAL_CONNECTION_PACKET)

            print("Awaiting metadata")
            self.buffer_size = int.from_bytes(ser.read(), byteorder='big')
            if self.buffer_size == 0:
                raise ConnectionError("No response on port " + self.port)
            print("Receiving data")

            ticks_per_meter = int.from_bytes(ser.read(4), byteorder='big')

            max_x = int.from_bytes(ser.read(2), byteorder='big')
            max_y = int.from_bytes(ser.read(2), byteorder='big')

            num_accel_points = int.from_bytes(ser.read(), byteorder='big')

            accel_points = []

            for i in range(num_accel_points):
                accel_point = int.from_bytes(ser.read(3), byteorder='big')
                accel_points.append(accel_point)

            accel_points = accel_points[::-1]

            max_vals = [max_x, max_y]

            print("Got metadata")

            return ticks_per_meter, max_vals, accel_points

    def get_metadata(self):
        with serial.Serial(port=self.port, baudrate=self.baud) as ser:
            ser.write('?')

    def write_message(self, message, debug=False, update_cb=None):
        chunks = chunkify(message, self.buffer_size, debug=debug)
        first_two = chunks[0:2]
        chunks = chunks[2:]

        with serial.Serial(port=self.port, baudrate=self.baud, timeout=0) as ser:
            print("Initiating communication")
            ser.write(self.START_PROGRAM_PACKET)
            print("Awaiting start sync packet")
            if not debug:
                while ser.read(1) != b'U':
                    update_cb()

            print("Start packet received")
            print("Writing initial buffer")
            for chunk in first_two:
                transmission = b''.join(chunk)
                ser.write(transmission)

            while ser.out_waiting:
                pass

            print("Initial buffer sent")

            self.remaining = chunks

            for chunk in chunks:
                print("Waiting for additional requests")
                transmission = b''.join(chunk)
                while ser.read(1) != b'U':
                    update_cb()
                print("Additional data requested")
                ser.write(transmission)
                print("Additional data sent")

            print("All data written")
            return


if __name__ == "__main__":
    comm = PicComm('COM15', 9600)

    #test = [[1,0,0],[2,0,0],[3,0,0],[4,0,0],[5,0,0],[6,0,0],[7,0,0],[8,0,0],[9,0,0], [10,0,0],[11,0,0],[12,0,0],[13,0,0],[14,0,0],[15,0,0],[16,0,0],[17,0,0],[18,0,0],[19,0,0], [20,0,0]]

    #comm.write_message(test)
    comm.connect()