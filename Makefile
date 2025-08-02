install:
	bash -c "gcc src/main.c -o img $$(pkg-config --cflags --libs gtk+-3.0)"
	install -Dm755 img ~/.local/bin/img
	rm -f img

uninstall:
	rm -f ~/.local/bin/img
