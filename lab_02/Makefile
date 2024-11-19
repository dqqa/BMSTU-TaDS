SRCS := $(wildcard ./*.c)
OBJS := $(SRCS:%.c=%.o)

EXE := app.exe

CFLAGS += -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wfloat-conversion

.PHONY: clean all
all: release

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS:%.c=%.o)
	$(CC) $(CFLAGS) $^ -o $(EXE)

debug: CFLAGS += -g3
debug: $(EXE)

release: CFLAGS += -O2 -s
release: $(EXE)

asan: debug
asan: CC := clang
asan: CFLAGS += -fno-omit-frame-pointer -fsanitize=address

msan: debug
msan: CC := clang
msan: CFLAGS += -fno-omit-frame-pointer -fsanitize=memory -fPIE

ubsan: debug
ubsan: CC := clang
ubsan: CFLAGS += -fno-omit-frame-pointer -fsanitize=undefined

clean:
	$(RM) *.exe *.o *.out
