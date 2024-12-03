import time
from typing import NoReturn
from pyfirmata2 import ArduinoMega
import logging
from pyfirm_lib import LCDDisplay

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)

# Constants
LED_PIN = 13  # Built-in LED pin on Arduino Mega
BLINK_INTERVAL = 1  # Time in seconds for each blink state
BOARD_PORT = "COM21"

# NOTE
# run Arduino IDE as administrator
# upload .ino via file -> examples -> Firmata -> StandardFirmata.ino
# run this program and most importantly have fun :)


def connect_to_board() -> ArduinoMega:
    """
    Set up the connection to the Arduino Mega board.

    Returns:
        Arduino_Mega: The connected Arduino Mega board object.

    Raises:
        RuntimeError: If unable to connect to the Arduino Mega.
    """
    try:
        board = ArduinoMega(BOARD_PORT)
        logging.info("Successfully connected to Arduino Mega")
        return board
    except Exception as e:
        logging.error(f"Failed to connect to Arduino Mega: {e}")
        raise RuntimeError("Unable to connect to Arduino Mega") from e


def blink_led(board: ArduinoMega) -> NoReturn:
    """
    Continuously blink the LED.

    Args:
        board (Arduino_Mega): The connected Arduino Mega board object.
    """
    led = board.get_pin(f"d:{LED_PIN}:o")

    try:
        while True:
            led.write(1)  # Turn LED on
            logging.info("LED ON")
            time.sleep(BLINK_INTERVAL)

            led.write(0)  # Turn LED off
            logging.info("LED OFF")
            time.sleep(BLINK_INTERVAL)
    except KeyboardInterrupt:
        logging.info("Program terminated by user")
    finally:
        board.exit()


def main() -> None:
    """Main function to run the LED blinking program."""
    try:
        board = connect_to_board()

        # Create an instance of LCDDisplay
        lcd = LCDDisplay(
            board,
            rs=4,
            enable=6,
            d4=10,
            d5=11,
            d6=12,
            d7=13,
        )
        lcd.format_write("hello world", alignment="right")
        lcd.format_write("it's okay :)", row=1, alignment="center")

        # blink_led(board)

    except Exception as e:
        logging.error(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
