import serial
import serial.threaded
import logging
import argparse


import infra.app.app
from uv_bicycle.src.gsm_to_arduino import constants
# from infra.modules.gsm_ai_a6 import GsmAiA6
# from infra.modules.arduino_uv_bicycle import ArduinoUvBicycle
from infra.old_modules.gsm.a6_gsm import A6Gsm
from infra.old_modules.arduino.uv_bicycle import UvBicycle
from infra.core.logor import Logor


class GsmToArduino(infra.app.app.App):

    def __init__(self):
        infra.app.app.App.__init__(self)
        print('GsmToArduino.__init__')
        parser = argparse.ArgumentParser(
            description='UV Bicycle')
        parser.add_argument(
            '--arduino_port',
            metavar='<arduino port>',
            dest='arduino_port',
            type=str,
            default=constants.ARDUINO_PORT,
            help='the arduino (BT) serial port')
        parser.add_argument(
            '--gsm_port',
            metavar='<gsm port>',
            dest='gsm_port',
            type=str,
            default=constants.GSM_PORT,
            help='the A6 gsm module serial port')

        self._args, _ = parser.parse_known_args()
        
        print(constants.BANNER)
        Logor(constants.LOGOR_FORMATS, constants.LOGOR_LEVEL, constants.LOGOR_COLOR_MAP)
        
        constants.ARDUINO_SERIAL['url'] = self._args.arduino_port
        constants.A6_GSM_SERIAL['url'] = self._args.gsm_port

        try:
            self._a6_serial = serial.serial_for_url(**constants.A6_GSM_SERIAL)
            self._a6_gsm_reader = serial.threaded.ReaderThread(self._a6_serial, A6Gsm)
            self._a6_gsm_reader.start()
            self.a6_gsm = self._a6_gsm_reader.connect()[1]
            self.a6_gsm.handle_sms = self.handle_sms
            self.a6_gsm.dummy()
        except Exception:
            print('error: a6_gsm did not respond')
            
        try:
            self._uv_bicycle_serial = serial.serial_for_url(**constants.ARDUINO_SERIAL)
            self._uv_bicycle_reader = serial.threaded.ReaderThread(self._uv_bicycle_serial, UvBicycle)
            self._uv_bicycle_reader.start()
            self.uv_bicycle = self._uv_bicycle_reader.connect()[1]
            self.uv_bicycle.set_screen_saver_sec(constants.SCREEN_SAVER_SEC)
            self.uv_bicycle.set_slice_on_ms(constants.SLICE_ON_MS)
            self.uv_bicycle.set_slice_off_ms(constants.SLICE_OFF_MS)
            self.uv_bicycle.set_char_off_ms(constants.CHAR_OFF_MS)
        except Exception:
            print('error: uv_bicycle did not respond')

    def handle_sms(self, number, send_time, text):
        text = text.encode(errors='replace').decode()
        print(u'== SMS message received ==\nFrom: {}\nTime: {}\nMessage:\n{}\n'.format(
            number, send_time, text))
        text = text.strip().replace('\n', ' ').replace('\t', ' ').replace('\r', '')[:constants.MAX_DRAW_CHARS]
        try:
            self.uv_bicycle.draw_text_rtl(text, constants.DRAW_RTL)
        except Exception:
            pass

    def __exit__(self):
        print('GsmToArduino.__exit__')
        try:
            self._a6_gsm_reader.close()
        except Exception:
            pass
        try:
            self._uv_bicycle_reader.close()
        except Exception:
            pass
