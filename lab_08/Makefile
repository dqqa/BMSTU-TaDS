SRCS := $(notdir $(wildcard src/*.c))
OBJS := $(addprefix out/, $(notdir $(SRCS:%.c=%.o)))

$(shell mkdir -p out/)

EXE := app.exe

CC := gcc
CFLAGS += -Wstrict-prototypes -Wall -Wextra -Wpedantic -Werror -MMD -std=gnu99 -I./inc/

all: release

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

NODEPS := clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(addprefix out/, $(notdir $(SRCS:.c=.d) $(UNIT_SRCS:.c=.d) $(MEASURE_SRCS:.c=.d)))
endif

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean release debug

debug: CFLAGS += -O0 -ggdb
debug: $(EXE)

release: CFLAGS += -s -O2 -DRELEASE
release: $(EXE)

clean:
	$(RM) out/* *.exe *.gp *.png
