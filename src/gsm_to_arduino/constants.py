import logging
import os.path

from infra.core.ansi import Ansi
from infra.run.common import *


A6_GSM_SERIAL = {'url': 'loop://' if IS_WINDOWS else '/dev/ttyAiGsm', 'baudrate': 115200, 'timeout': 1}
UV_BICYCLE_SERIAL = {'url': 'loop://' if IS_WINDOWS else '/dev/ttyUvBicycle', 'baudrate': 115200, 'timeout': 1}

UV_BICYCLE_MAX_DRAW_CHARS = 120
UV_BICYCLE_DRAW_RTL = True
UV_BICYCLE_SCREEN_SAVER_SEC = 90
UV_BICYCLE_SLICE_ON_MS = 70
UV_BICYCLE_SLICE_OFF_MS = 0
UV_BICYCLE_CHAR_OFF_MS = 90

GSM_SIM_NUMBER = '0'

SMS_LOGGER_PATH = os.path.join(BASIC_PATH, 'sms_log.txt')
SMS_LOGGER_SIZE = 1024
SMS_LOGGER_COUNT = 10

SHEET_FILE_ARGS = {'service_file': os.path.join(BASIC_PATH, 'logger-995ad2d4b91d.json'), 'no_cache': IS_WINDOWS}
SHEET_FILE_NAME = 'sms_log'

# LOGOR_LEVEL = logging.DEBUG

APP_BANNER = Ansi.style(
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
