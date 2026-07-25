TARGET   := widescreen_toggle
OBJS     := source/main.c

all: $(TARGET).3dsx

$(TARGET).3dsx: $(OBJS)
	arm-none-eabi-gcc -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -O2 -mword-relocations -fomit-frame-pointer -ffast-math -I$(DEVKITPRO)/libctru/include -D__3DS__ -c source/main.c -o main.o
	arm-none-eabi-gcc -specs=3dsx.specs -g -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -Wl,-Map,$(TARGET).map main.o -L$(DEVKITPRO)/libctru/lib -lctru -lm -o $(TARGET).elf
	3dsxtool $(TARGET).elf $(TARGET).3dsx

clean:
	rm -f *.o *.elf *.3dsx *.map
