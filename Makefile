CPPFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline -I -D_XOPEN_SOURCE
CFLAGS += -std=c11 -lm
ARFLAGS += -U

DEBUG = -DDEBUG -g

BINS = ticker

FILES = ticker.o

all: build

.PHONY: all

debug: CFLAGS += -DDEBUG -g
debug: CPPFLAGS += -DDEBUG -g
debug:  $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

profile: CFLAGS += -pg
profile: CPPFLAGS += -pg
profile: $(FILES)
	gcc -o $(BINS) $(DECODE) $(CPPFLAGS) $(CFLAGS)

build: $(FILES)
	gcc -o $(BINS) $(FILES) $(CPPFLAGS) $(CFLAGS)
	$(MAKE) clean

clean:
	$(RM) *.o *.a

cleanAll:
	$(RM) $(BINS) *.o *.a