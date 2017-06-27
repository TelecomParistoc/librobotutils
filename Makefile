TARGET = librobotutils.so
SRCS = i2c-functions.c timing.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h})
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
TESTS = tests/timing
CC=gcc
CFLAGS = -O2 -std=gnu99 -Wall -Werror -fpic
LDFLAGS= -shared -lwiringPi -lpthread
PREFIX = /usr/local
VPATH = build/

vpath %.c src/ tests/
vpath %.h src/

.PHONY: all build clean tests


all: build build/$(TARGET)

build:
	@mkdir -p build
build/%.o: %.c build/%.d
	@echo "$<"
	@$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.c
	@$(CC) $(CFLAGS) -MM -MF $@ -MP $<

build/$(TARGET): $(OBJECTS)
	@echo "\nLinking target $@"
	@$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

tests: LDFLAGS=-lrobotutils
tests: $(TESTS)

tests/%: tests/%.o $(OBJECTS)
	$(CC) $(LDFLAGS_EXE) $^ -o $@

clean:
	rm -f build/*.o build/*.so build/*.d
	rm -f $(TESTS)
	rm -f tests/*.o

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/robotutils
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/robotutils/
	cp src/robotutils.h $(DESTDIR)$(PREFIX)/include/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep robotutils

-include $(subst .c,.d,$(SRCS))
