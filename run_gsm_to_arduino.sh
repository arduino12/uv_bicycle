#!/usr/bin/env bash
cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ..
python3.6 ./infra/run/app_runner.py --interface ipython --app "uv_bicycle.src.gsm_to_arduino.gsm_to_arduino" "$@"
