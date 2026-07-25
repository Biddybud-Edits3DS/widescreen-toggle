ifneq ($(words $(CURDIR)),1)
$(error Spaces are not allowed in the project path.)
endif

TARGET      := widescreen-toggle
BUILD       := build
SOURCES     := source
INCLUDES    := include

include $(DEVKITPRO)/devkitARM/3ds_rules

export OUTPUT   := $(CURDIR)
export VPATH    := $(dir $(wildcard $(SOURCES)/*))
export CFILES   := $(notdir $(wildcard $(SOURCES)/*.c))

export OFILES   := $(CFILES:.c=.o)
export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(DEVKITPRO)/libctru/include

ifeq ($(strip $(ICON)),)
    export NOT_FOUND :=
else
    export NOT_FOUND := $(wildcard $(ICON))
endif

.PHONY: clean all

all: $(TARGET).3dsx

$(TARGET).3dsx: $(TARGET).elf

$(TARGET).elf: $(OFILES)

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).3dsx
