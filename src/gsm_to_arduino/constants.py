import os
import logging
from infra.core.ansi import Ansi


IS_WINDOWS = (os.name == 'nt')

GSM_PORT = 'COM12' if IS_WINDOWS else '/dev/gsm_a6'
ARDUINO_PORT = 'COM13' if IS_WINDOWS else '/dev/arduino_uv'

A6_GSM_SERIAL = {'url': '', 'baudrate': 115200, 'timeout': 1} # 'spy://COM34'
ARDUINO_SERIAL = {'url': '', 'baudrate': 115200, 'timeout': 1}

MAX_DRAW_CHARS = 120
DRAW_RTL = True
SCREEN_SAVER_SEC = 90
SLICE_ON_MS = 70
SLICE_OFF_MS = 0
CHAR_OFF_MS = 90

LOGOR_LEVEL = logging.INFO
LOGOR_FORMATS = ('%(asctime)s %(name)s %(levelname)s:\n%(message)s', '%d/%m/%y %H:%M:%S')
LOGOR_COLOR_MAP = {
    logging.CRITICAL: (Ansi.YELLOW, Ansi.BACKGROUND_RED,),
    logging.ERROR: (Ansi.RED, Ansi.BRIGHT),
    logging.WARNING: (Ansi.YELLOW, Ansi.BRIGHT),
    logging.INFO: (Ansi.CYAN, Ansi.FAINT),
    logging.DEBUG: (Ansi.YELLOW, Ansi.FAINT),
    'name': (Ansi.GREEN, Ansi.FAINT),
    'levelname': (Ansi.MAGENTA, Ansi.FAINT),
}

FILE_LOGGER_PATH = r'C:\Users\arad-eee\Documents\GitHub\uv_bicycle\src\gsm_to_arduino\log_sms.log' # 'COM12' if IS_WINDOWS else '/dev/gsm_a6'
FILE_LOGGER_SIZE = 1024
FILE_LOGGER_COUNT = 10

BANNER = Ansi.style(
r"""
                  /'\
                 /
                /                 ,
             c-'                 /
            /'-._         ,____,' .-'''-.
       .-'.// \  '-;-========,"-,'       '
     ,`   /,   \_//\       ,/  (  '- *)   )
    (   ./  )   {,}========'===='- '     ,
     ,     ,   \/               ',      .
 _____'-.-`_______________________'-..-'____

  U V   B i c y c l e   I n t e r f a c e:
____________________________________________
""", Ansi.CYAN, Ansi.BRIGHT)
