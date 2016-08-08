
all: build

build:
	platformio run

install:
	platformio run --target=upload

monitor: build install
	platformio serialports monitor -p /dev/ttyUSB0


clean:
	platformio run --target=clean

.PHONY: all build install clean monitor
