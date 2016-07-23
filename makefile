all: lspad

lspad: lspad.c
	gcc lspad.c -o lspad

clean:
	rm lspad

install: lspad
	sudo mv lspad /usr/bin/lspad
	@echo done! to uninstall, execute: make uninstall

uninstall:
	sudo rm /usr/bin/lspad
