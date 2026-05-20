RVCC=riscv64-unknown-linux-gnu
SOURCES=$(wildcard *.S)
OUT=$(patsubst %.S,%.bin,$(SOURCES))

all: $(OUT)

%.bin: %.S Makefile
	$(RVCC)-as -c $< -o $@.o
	$(RVCC)-ld -Triscv.lds -o $@.elf $@.o
	$(RVCC)-objcopy -O binary $@.elf $@

.PHONY: clean

clean:
	$(RM) *.o *.elf *.bin
