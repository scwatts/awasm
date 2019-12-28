TARGET = main.js

SRCS = main.c velveth.c velvetg.c disk_streamer.c utility.c
OBJS = $(addsuffix .o,$(basename $(SRCS)))
DEPS = $(OBJS:.o=.d)

VELVETLIB = ../lib/velvet/velvet.a

INC_FLAGS = -I../lib -I../include

EMFUNCS = -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall', 'UTF8ToString', '_malloc']"
EMLIBS = -s USE_ZLIB=1 -s ASYNCIFY
# NOTE: velvetg::correctGraph.c executes code which has bad alignment and fails with SAFE_HEAP
# and/or WARN_UNALIGNED
EMOPTS = -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_MEMORY=524288000 #-s SAFE_HEAP=1 -s WARN_UNALIGNED=1
EMINC = -lidbfs.js --js-library external.js
EMFLAGS = $(EMOPTS) $(EMFUNCS) $(EMINC) $(EMLIBS)

CC = emcc
CFLAGS = -O3 -Wall -Wextra -Wcast-align -Wover-aligned $(EMFLAGS) $(INC_FLAGS)

$(TARGET): $(OBJS) $(VELVETLIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) $(VELVETLIB) -o $@ $(LOADLIBES) $(LDLIBS)

$(VELVETLIB):
	$(MAKE) -C ../lib/velvet

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) main.wasm
