import logging
import os.path

from infra.core.ansi import Ansi
from infra.run.common import *


# LOGOR_LEVEL = logging.DEBUG

GSM_UART = {'url': 'loop://' if IS_WINDOWS else '/dev/ttyGsmUart', 'baudrate': 115200, 'timeout': 1}
UV_BICYCLE_SERIAL = {'url': 'loop://' if IS_WINDOWS else '/dev/ttyUvBicycle', 'baudrate': 115200, 'timeout': 1}

UV_BICYCLE_MAX_DRAW_CHARS = 40
UV_BICYCLE_DRAW_RTL = False
UV_BICYCLE_SCREEN_SAVER_SEC = 90
UV_BICYCLE_SLICE_ON_MS = 50
UV_BICYCLE_SLICE_OFF_MS = 0
UV_BICYCLE_CHAR_OFF_MS = 70

PICTURES_PATH = '/tmp/%s.png'
PICTURES_DATETIME_FORMAT = DATETIME_FORMAT.replace('-', '_').replace(':', '_').replace(' ', '_')

REBOOT_FORMAT = 'system will reboot in a minute'
GSM_DATA_FORMAT = 'csq: %s, vbat: %s, temperature: %s'

KEYS_PATH = os.path.join(BASIC_PATH, 'keys')
SERVICE_ACCOUNT_PATH = os.path.join(KEYS_PATH, 'old/logger-995ad2d4b91d.json')
# SERVICE_ACCOUNT_PATH = os.path.join(KEYS_PATH, 'old/logger-176517-f8677dbcd71f.json')
# SERVICE_ACCOUNT_PATH = os.path.join(KEYS_PATH, 'google_service_account.json')
SHEET_SMS_LOG_NAME = 'sms_log'
WORKSHEET_SMS_NAME = '0'

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
