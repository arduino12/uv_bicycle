import os
import logging
import datetime
import platform
import serial
import serial.threaded


from infra.app import app
from infra.old_modules.sim800 import sim800
from infra.modules.google.sheets import sheets
from uv_bicycle.src.gsm_to_arduino import constants
from infra.old_modules.arduino.uv_bicycle import UvBicycle


class GsmToArduino(app.App):
    _logger = logging.getLogger('gsm_to_arduino')

    def __init__(self):
        app.App.__init__(self, constants, spam_loggers=sheets.Sheets.SPAM_LOGGERS)
        self._modules.extend((sim800, sheets))
        # google sheets logger
        try:
            self.sheets = sheets.Sheets(constants.SERVICE_ACCOUNT_PATH)
        except:
            self._logger.exception('self.sheet')
            self.sheets = None
        # google sheets name is the hostname
        constants.WORKSHEET_SMS_NAME = platform.node()
        # print app banner
        print(constants.APP_BANNER)
        # connect to uv bicycle
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
        # connect to gsm
        try:
            self._gsm_uart = serial.serial_for_url(**constants.GSM_UART)
            self._gsm_reader = serial.threaded.ReaderThread(self._gsm_uart, sim800.Sim800)
            self._gsm_reader.start()
            self.gsm = self._gsm_reader.connect()[1]
        except:
            self._logger.exception('gsm')
            self.gsm = None
        else:
            self.gsm.status_changed = self.gsm_status_changed
            self.gsm.sms_recived = self.gsm_sms_recived

    def gsm_status_changed(self):
        self._logger.info('gsm_status_changed: %s', self.gsm.status)
        if self.gsm.status == 'ALIVE':
            self._logger.info(constants.GSM_DATA_FORMAT,
                self.gsm.get_csq(), self.gsm.get_vbat(), self.gsm.get_temperature())
        elif self.gsm.status == 'TIMEOUT':
            self._logger.warning('gsm did not respond')

    def gsm_sms_recived(self, number, send_time, text):
        # normalize sms text, number and send_time
        text = text.encode(errors='replace').decode().strip().replace('\n', ' ').replace('\t', ' ').replace('\r', '')
        normalize_number = self.gsm.normalize_phone_number(number)
        send_time = send_time.strftime(constants.DATETIME_FORMAT)
        self._logger.info('AT: %s FROM: %s MESSAGES: %s', send_time, normalize_number, text)
        if text == 'REBOOT':
            self.send_sms(number, constants.REBOOT_FORMAT, False)
            os.system('shutdown -r')
        elif text == 'GSM DATA':
            try:
                t = constants.GSM_DATA_FORMAT % (
                    self.gsm.get_csq(), self.gsm.get_vbat(), self.gsm.get_temperature())
            except:
                self._logger.warning('cant read gsm data')
                return
            # self._logger.info(t)
            self.send_sms(number, t.replace(', ', '\n'), False)
        else:
            # draw the sms text using the uv bicycle
            try:
                self.uv_bicycle.draw_text_rtl('  ' + text[:constants.UV_BICYCLE_MAX_DRAW_CHARS], constants.UV_BICYCLE_DRAW_RTL)
            except:
                pass
            # log to worksheet
            if self.sheets is not None:
                try:
                    self.sheets.append_worksheet_table(constants.SHEET_SMS_LOG_NAME, constants.WORKSHEET_SMS_NAME,
                        send_time, normalize_number, text)
                except:
                    self._logger.exception('self.sheets')
                    self.sheets = None
            if '@' in text and '.' in text and ' ' not in text:
                try:
                    path = constants.PICTURES_PATH % (datetime.datetime.now().strftime(constants.PICTURES_DATETIME_FORMAT),)
                    os.system('raspistill -n -o {0} && mpack -s "UV Bicycle" {0} {1} && rm {0} >/dev/null 2>&1&'.format(path, text))
                except:
                    pass

    def send_sms(self, number, text, raise_exception=True):
        try:
            self.gsm.send_sms(number, text)
        except:
            if raise_exception:
                raise

    def __exit__(self):
        try:
            self._gsm_reader.close()
        except:
            pass
        try:
            self._uv_bicycle_reader.close()
        except:
            pass
        app.App.__exit__(self)
