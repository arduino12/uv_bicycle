import serial
import serial.threaded
import logging
import logging.handlers
import argparse
import pygsheets


from infra.app import app
from uv_bicycle.src.gsm_to_arduino import constants
from infra.old_modules.gsm.a6_gsm import A6Gsm
from infra.old_modules.arduino.uv_bicycle import UvBicycle


class GsmToArduino(app.App):
    # colored stdout logger for app
    _logger = logging.getLogger('gsm_to_arduino')
    # rotating file logger for sms messages
    # _sms_logger = logging.getLogger('sms_logger')

    def __init__(self):
        app.App.__init__(self, constants)
        # config sms logger
        # self._sms_logger.setLevel(logging.INFO)
        # [self._sms_logger.removeHandler(i) for i in self._sms_logger.handlers]
        # self._sms_logger.addHandler(logging.handlers.RotatingFileHandler(
            # constants.SMS_LOGGER_PATH, constants.SMS_LOGGER_SIZE, constants.SMS_LOGGER_COUNT))
        # parse runtime arguments
        parser = argparse.ArgumentParser(
            description='UV Bicycle')
        parser.add_argument(
            '--arduino_port',
            metavar='<arduino port>',
            dest='arduino_port',
            type=str,
            default=constants.UV_BICYCLE_PORT,
            help='the arduino (BT) serial port')
        parser.add_argument(
            '--gsm_port',
            metavar='<gsm port>',
            dest='gsm_port',
            type=str,
            default=constants.GSM_PORT,
            help='the A6 gsm module serial port')
        self._args, _ = parser.parse_known_args()
        # print app banner
        print(constants.APP_BANNER)
        # set the chosen serial ports
        constants.UV_BICYCLE_SERIAL['url'] = self._args.arduino_port
        constants.A6_GSM_SERIAL['url'] = self._args.gsm_port

        try:
            for i in ('googleapiclient.discovery', 'oauth2client.transport', 'oauth2client.crypt', 'oauth2client.client'):
                logging.getLogger(i).setLevel(logging.WARNING)
            self._drive_sheets = pygsheets.authorize(service_file=constants.SHEET_FILE_SERVICE)
            self._sms_sheet = self._drive_sheets.open(constants.SHEET_FILE_NAME)
            self._update_sms_workseet()
        except:
            self._logger.exception('sms_workseet')
            # self._reload_()
        
        try:
            self._a6_serial = serial.serial_for_url(**constants.A6_GSM_SERIAL)
            self._a6_gsm_reader = serial.threaded.ReaderThread(self._a6_serial, A6Gsm)
            self._a6_gsm_reader.start()
            self.a6_gsm = self._a6_gsm_reader.connect()[1]
        except:
            self._logger.exception('a6_gsm')
        else:
            self.a6_gsm.status_changed = self.a6_gsm_status_changed
            self.a6_gsm.sms_recived = self.a6_gsm_sms_recived
            
        try:
            self._uv_bicycle_serial = serial.serial_for_url(**constants.UV_BICYCLE_SERIAL)
            self._uv_bicycle_reader = serial.threaded.ReaderThread(self._uv_bicycle_serial, UvBicycle)
            self._uv_bicycle_reader.start()
            self.uv_bicycle = self._uv_bicycle_reader.connect()[1]
            self.uv_bicycle.set_screen_saver_sec(constants.UV_BICYCLE_SCREEN_SAVER_SEC)
            self.uv_bicycle.set_slice_on_ms(constants.UV_BICYCLE_SLICE_ON_MS)
            self.uv_bicycle.set_slice_off_ms(constants.UV_BICYCLE_SLICE_OFF_MS)
            self.uv_bicycle.set_char_off_ms(constants.UV_BICYCLE_CHAR_OFF_MS)
        except TimeoutError:
            self._logger.warning('uv_bicycle did not respond')
        except:
            self._logger.exception('uv_bicycle')
            # self._reload_()

    def _update_sms_workseet(self):
        sms_workseet = self._sms_sheet.worksheet_by_title(constants.GSM_SIM_NUMBER)
        if sms_workseet is None:
            self._logger.warning('sms_workseet named %s didn\'t found', constants.GSM_SIM_NUMBER)
            return
        self.sms_workseet = sms_workseet

    def a6_gsm_status_changed(self):
        self._logger.info('a6_gsm_status_changed: %s', self.a6_gsm.status)
        if self.a6_gsm.status == 'ALIVE':
            if constants.GSM_SIM_NUMBER == '0':
                constants.GSM_SIM_NUMBER = self.a6_gsm.normalize_phone_number(self.a6_gsm.get_sim_number())
                self._logger.info('sim_number: %s', constants.GSM_SIM_NUMBER)
                self._update_sms_workseet()
        elif self.a6_gsm.status == 'TIMEOUT':
            self._logger.warning('a6_gsm did not respond')

    def a6_gsm_sms_recived(self, number, send_time, text):
        text = text.encode(errors='replace').decode().strip().replace('\n', ' ').replace('\t', ' ').replace('\r', '')
        number = self.a6_gsm.normalize_phone_number(number)
        send_time = send_time.strftime(constants.DATETIME_FORMAT)
        self._logger.info('AT: %s FROM: %s MESSAGES: %s', send_time, number, text)
        # self._sms_logger.info('AT: %s FROM: %s MESSAGES: %s', send_time, number, text)
        try:
            self.uv_bicycle.draw_text_rtl('  ' + text[:constants.UV_BICYCLE_MAX_DRAW_CHARS], constants.UV_BICYCLE_DRAW_RTL)
        except:
            pass
        try:
            self.sms_workseet.append_table(values=(send_time, number, text))
        except:
            pass
            # self._logger.exception('sms_workseet')

    def __exit__(self):
        try:
            self._a6_gsm_reader.close()
        except:
            pass
        try:
            self._uv_bicycle_reader.close()
        except:
            pass
        app.App.__exit__(self)
