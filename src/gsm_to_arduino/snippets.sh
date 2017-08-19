
### aliases ###
nano /home/pi/.bash_aliases
alias ll='ls -lhA'
alias ..='cd ..'
alias df='df -H'
alias du='du -ch'
chr() {
  printf \\$(printf '%03o' $1)\\n
}
ord() {
  printf '%d\n' "'$1"
}
alias uv_bicycle_log='sudo journalctl -e -f -n 50 -o cat -u uv_bicycle.service'
alias uv_bicycle_log_all='sudo journalctl --no-tail --no-pager -m -o cat -u uv_bicycle.service'
alias uv_bicycle_restart='sudo systemctl restart uv_bicycle.service'
alias uv_bicycle_test_gsm='/home/pi/Public/infra/scripts/run_client.sh --cmd "app.a6_gsm.dummy()"'
alias uv_bicycle_test_arduino='/home/pi/Public/infra/scripts/run_client.sh --cmd "app.uv_bicycle.draw_fix_text(1)"'
alias uv_bicycle_usb_restart='sudo systemctl stop uv_bicycle.service; sleep 4; for i in "unbind" "bind"; do sudo sh -c "echo 1-1 > /sys/bus/usb/drivers/usb/$i" && sleep 5; done; sleep 8; sudo systemctl start uv_bicycle.service'

### wifi ###
sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
network={
	ssid="MADA"
	key_mgmt=NONE
	# psk="madaorgil"
	# key_mgmt=WPA-PSK
}

### python3 packages ###
sudo pip3 install --upgrade pip
sudo pip3 install --upgrade ipython rpyc pyserial pygsheets

# Output file's content while it change
tail -f /home/pi/Public/sms_log.log

### serial ports ###
# auto serial port shortcuts
sudo nano /etc/udev/rules.d/90-usb-serial.rules
SUBSYSTEM=="tty",KERNELS=="1-1.2:1.0",SYMLINK+="ttyAiGsm"
SUBSYSTEM=="tty",KERNELS=="1-1.4:1.0",SYMLINK+="ttyUvBicycle"

### arduino ###
~/Public/arduino/arduino --board arduino:avr:pro --port /dev/ttyUSB0 --upload ~/Public/uv_bicycle/src/arduino_to_uv/arduino_to_uv.ino
