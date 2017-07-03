@echo off
..\infra\scripts\run_cmd.bat python ..\infra\run\app_runner.py --interface ipython --app "uv_bicycle.src.gsm_to_arduino.gsm_to_arduino" %*
