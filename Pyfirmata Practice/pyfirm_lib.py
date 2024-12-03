import time


class LCDDisplay:
    def __init__(self, board, rs, enable, d4, d5, d6, d7, pin_type="d"):

        # NOTE - these pins are PWM currently, code 'p'
        # https://pyfirmata.readthedocs.io/en/latest/pyfirmata.html
        self.board = board
        self.rs = board.get_pin(f"{pin_type}:{rs}:o")
        self.enable = board.get_pin(f"{pin_type}:{enable}:o")
        self.d4 = board.get_pin(f"{pin_type}:{d4}:o")
        self.d5 = board.get_pin(f"{pin_type}:{d5}:o")
        self.d6 = board.get_pin(f"{pin_type}:{d6}:o")
        self.d7 = board.get_pin(f"{pin_type}:{d7}:o")
        self.initialize()

    def initialize(self):
        self.write_command(0x33)  # Initialize
        self.write_command(0x32)  # Set to 4-bit mode
        self.write_command(0x28)  # 2 line, 5x7 matrix
        self.write_command(0x0C)  # Turn cursor off
        self.write_command(0x06)  # Increment cursor
        self.write_command(0x01)  # Clear display
        time.sleep(0.005)

    def write_command(self, cmd):
        self.rs.write(0)
        self.write_four_bits(cmd >> 4)
        self.write_four_bits(cmd & 0x0F)

    def write_char(self, char):
        self.rs.write(1)
        self.write_four_bits(ord(char) >> 4)
        self.write_four_bits(ord(char) & 0x0F)

    def write_four_bits(self, data):
        self.d4.write(data & 0x01)
        self.d5.write((data >> 1) & 0x01)
        self.d6.write((data >> 2) & 0x01)
        self.d7.write((data >> 3) & 0x01)
        self.pulse_enable()

    def pulse_enable(self):
        self.enable.write(0)
        time.sleep(0.0005)
        self.enable.write(1)
        time.sleep(0.0005)
        self.enable.write(0)
        time.sleep(0.0005)

    def clear(self):
        self.write_command(0x01)
        time.sleep(0.002)

    def set_cursor(self, row, col):
        offsets = [0x00, 0x40, 0x14, 0x54]
        self.write_command(0x80 | (offsets[row] + col))

    def write(self, text):
        for char in text:
            self.write_char(char)

    def format_write(self, input_string, alignment="left", fill_char=" ", row=0):
        """
        Format a string to a length of 16 characters with specified alignment.

        Parameters:
        input_string (str): The input string to be formatted.
        alignment (str): The alignment option ('left', 'right', or 'center'). Default is 'left'.
        fill_char (str): The character to use for padding. Default is space.

        Returns:
        str: A formatted string of length 16.
        """
        # Ensure the fill_char is a single character
        if len(fill_char) != 1:
            raise ValueError("fill_char must be a single character")

        # Truncate the input string if it's longer than 16 characters
        if len(input_string) > 16:
            return input_string[:16]

        output_str: str = ""

        # Perform the alignment
        if alignment == "left":
            output_str = input_string.ljust(16, fill_char)
        elif alignment == "right":
            output_str = input_string.rjust(16, fill_char)
        elif alignment == "center":
            output_str = input_string.center(16, fill_char)
        else:
            raise ValueError(
                "Invalid alignment option. Use 'left', 'right', or 'center'."
            )

        # print to the LCD display
        self.set_cursor(row, 0)
        self.write(output_str)
