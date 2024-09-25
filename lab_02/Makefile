SRCS := $(wildcard ./*.c)
OBJS := $(SRCS:%.c=%.o)

EXE := app.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS:%.c=%.o)
	$(CC) $(CFLAGS) $^ -o $(EXE)

debug: CFLAGS += -g
debug: $(EXE)

release: CFLAGS += -O0
release: $(EXE)

.PHONY: clean
clean:
	$(RM) *.exe *.o
