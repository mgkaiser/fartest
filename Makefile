# Project settings
TARGET = fartest
CC     = vc
AS     = vasm6502_oldstyle     # change to the right vasm backend (e.g. vasm68k_mot, vasm6502_oldstyle, vasmppc_std, etc.)
LD     = vlink

#########################################################
## If there is code or BSS in banked memory
#########################################################
CFLAGS = +x16b -k -vv -g -c99
#CFLAGS = +x16b -c99 -Osir -size -final
ASFLAGS =
LDFLAGS = -m -b cbmreu -Cvbcc -T$(VBCC)/targets/6502-x16/vlinkb.cmd -L$(VBCC)/targets/6502-x16/lib $(VBCC)/targets/6502-x16/lib/startupb.o -o $(TARGET) -lvc -Mmapfile

#########################################################
## If there is no code or BSS in banked memory
#########################################################
#CFLAGS = +x16 -k -vv -g -c99
##CFLAGS = +x16 -c99 -Osir -size -final
#ASFLAGS =
#LDFLAGS = -m -b cbmreu -Cvbcc -T$(VBCC)/targets/6502-x16/vlink.cmd -L$(VBCC)/targets/6502-x16/lib $(VBCC)/targets/6502-x16/lib/startup.o -o $(TARGET) -lvc -Mmapfile

# Sources
C_SRCS  := 	main.c\
			application.c \
			linkedlist.c \
			event.c \
			panel.c \
			desktop.c \
			window.c \
			window_test1.c \
			farmalloc.c 
ASM_SRCS:= 
OBJS    := $(C_SRCS:%.c=obj/%.o) $(ASM_SRCS:%.s=obj/%.o)

# Rules
all: $(TARGET)	
	@cp $(TARGET).b1 BANKIMG || true
	@rm $(TARGET).b1 || true

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.s
	$(AS) $(ASFLAGS) -Felf $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) src/*.asm BANKIMG mapfile

.PHONY: all clean
