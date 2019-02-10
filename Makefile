
BIN = /opt/gbdk-n-master/bin

make:
	$(BIN)/gbdk-n-compile.sh src/main.c -o main.rel
	$(BIN)/gbdk-n-link.sh main.rel -o main.ihx
	$(BIN)/gbdk-n-make-rom.sh main.ihx build/main.gb

clean:
	rm *.ihx *.asm *.lk *.lst *.map *.noi *.rel *sym;

.PHONY: clean