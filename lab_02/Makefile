SRCS := $(wildcard ./*.c)
OBJS := $(SRCS:%.c=%.o)

EXE := app.exe

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

clean:
	$(RM) *.exe *.o *.out
