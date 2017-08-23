# Arduino To Uv
Arduino code for rotating led/laser array.

## Files structure

[arduino_to_uv.ino](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/arduino_to_uv.ino) - Main arduino code, draw timing constants.

[cat4016.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/cat4016.h) - Lib for cat4016 constant current drivers.

[common.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/common.h) - Common macros.

[fix_texts.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/fix_texts.h) - Permanent text for screen saver, saved in flash.

[graphics.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/graphics.h) - Lib for drawing text slice by slice with leds.

[heb_5x7.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/heb_5x7.h) - Software generated file for english and hebrew font array, saved in flash.

[interactive_menu.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/interactive_menu.h) - Main busy loop for commands handling and text drawing. 

[leds.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/leds.h) - Abstraction for led / laser array drivers.

[utf8.h](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv/utf8.h) - utf8 and unicode conversation stuff.
