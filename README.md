# UV Bicycle
This repository was created for a rotating laser array above circle colored with [photochromic](https://en.wikipedia.org/wiki/Photochromism) paint.
We were able to draw fading SMS messages on the circle from above by using the ultra violet lasers that activates the photochromatic dye for short periods of time.

The project was done for and in collaboration with the [Bloomfield Science Museum in Jerusalem](http://www.mada.org.il/en) that provided us with the hardware and photochromatic paint.
It was created to serve as a science exhibit that demonstrates the properties of photochromatic dyes.
The code is licensed with the MIT license so you are free to use the code for your own needs.

See the results: https://goo.gl/photos/oJwscTjsn2JnW7Ck9

## Repository Content
[/src/arduino_to_uv/](https://github.com/arduino12/uv_bicycle/tree/master/src/arduino_to_uv) - Arduino code for rotating led/laser array.

[/src/auto_cts/](https://github.com/arduino12/uv_bicycle/tree/master/src/auto_cts) - Arduino code for toggling CTS line to GSM module.

[/src/gsm_to_arduino/](https://github.com/arduino12/uv_bicycle/tree/master/src/gsm_to_arduino) - Pi main code for receiving logging and drawing the SMS messages. 

## Hardware
<img src="https://lh3.googleusercontent.com/Vc9EMIPHA8w_r2cUavsGSQO-o_p-7ncmXYEqYOy1NqUkL5eqGi37OpkXadukhLj22xZ-gRMF1kTlX_NGuJq8BMt3eHUF64rf8kQAkVgELNtqQZ1RMopb4X-rpCWFp0X3qmpbkKMwe0I_WGfFK4Y0_PC1MjWjIJ3T4Ibz2jPdR744nrNS3TW4o-I8bP2ekQa05fymQVjbHknS2VRSZ0DTwl79BUZwA_VtFqYXUJwPtxD64q70xKaM9rZHWuroJr_1Tvtd-X4EULkwvXKUlg1P7ToxBazeEolZ0sMSxTwca8a46mI9U5H4igfo2b5B4D7_b6VAIZAvufg7lnp_SHirEBCOUqLExb3ahMAZRm8fPo9suOPBamb8nU4uetgNMl8iYfZWosMtypwYz0UoY1q7np4bJ6RxMoacF1YhWF4UVPSs5c2owiwA5MacgWr0QcjLhOXokvbC5WcQ38CB4p-cbZNxGNrWULT_6ONU7YTXHPvQaKNxr3c_F1Vqt-jB5_UeZOAFuIAoNEPa4kCFHji4aErF4kzA6CDGKILxqji498P7u0TZostGEw3Fn8uNkkDoxx1JTORTo-OQx-Je5zLs8fBRl57VO6N1IjqmTV3mxX8bAGLKvdmcFl1H1_UraBbP3oRyOuiZWikjajVKhDTIFSlbFYo9egeu3pkob9Kr_HS7YDQ">
<img src="https://lh3.googleusercontent.com/Ws7JQw-yEKeiUe4MuJQAN-q8wx_f5K7RBDDxZ9f8wQQIW_8lA9Yw4n6IqJK_GnT0hbwdW7TLM-m-gelrgOvau5pxB6vvcL0IUWs8IOO5ovuf7cEzAAFZcjh-KP-J3fhh4H8FxVKoyaPNvuyuJGwxkWvGBafbCa8wj3TknDk1y6XuR6KxQ_zH66dlrq7bNpso4gAWbYyNidm6teT94X6_2um_1w_LaygNzx0y9oYFp9pRaNshwGENiCASdAd84XJyoEfFHm6JqLjo-RjeU93c4V7C7xSTb1NT4A-c7qdPuSsIp1EbnigUo7ll56vmy7caqoNtTOS2SGiKBetD4fq8ZSKgZ6Fa4dgx-YZiuOd23pawARR5CGtpw04KW2g3HyMuWi070UGC1EdrO68SzervdrS9uVxDwNguE_PY4yW4LJEckRW4u6W9sxowAWTuWKZvJDicob2_tlzZAGOmuAZKlW8aKlHq40BI1b61WPKIbscPgb4QIG1kB7d0ltgF0tWaYij5iwDBfQq8FyUTvc5_OfMTae2ZswM36q5LhByYsK_KRg5VsyydxD20RPjYVLZ8fsF3lGvVw8Zd_pon3TpEVTmDYJnUnama0IZVXObShmHL_anDgvr7-ycj7bS5GCZ7KQlDQ0UBXkGgK_qk0FnERgxz9sPKLAx6wxCItfvWbOjPZnQ">

### Pi Side
* Raspberry pi 3 with two USB-UART converters, provide both power and communication to BT and GSM modules.
* Extra arduino is needed because of AI-A6 GSM module bug that sends garbage in uart if CTS is not toggled.

### Bicycle Side
* The laser driver is ULN2003 - 7 NPN transistors in a single package.

## Software
* Must clone [infra](https://github.com/arduino12/infra) near this repo clone.
* For Google Sheets logger, put the json keys file near this repo.
* Better to auto run it with systemctl and see logs with journalctl on pi.
