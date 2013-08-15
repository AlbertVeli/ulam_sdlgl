# (k) ALL RIGHTS REVERSED - Reprint what you like

X_LINDENMAYER = lindenmayer
OBJS_LINDENMAYER = $(X_LINDENMAYER).o lsystems.o sdlgl.o

X_ULAM = ulam/ulam
X_KOCH = recursive/koch

# Uncomment line below to enable debug build
#DEBUG = yes

ifdef DEBUG
DBGFLAGS = -g -O0
else
DBGFLAGS = -O2 -march=native
endif

WFLAGS = -W -Wall -Wno-unused-result

MACHINE = $(shell uname -s)

ifeq ($(MACHINE),Darwin)
   OPENGL_INC = -FOpenGL
   OPENGL_LIB = -framework OpenGL
else
   OPENGL_INC =
   OPENGL_LIB = -lGL -lGLU
endif
SDL_INC = $(shell sdl-config --cflags)
SDL_LIB = $(shell sdl-config --libs)

CFLAGS = $(WFLAGS) $(DBGFLAGS) $(SDL_INC) $(OPENGL_INC)
LIBS = $(SDL_LIB) $(OPENGL_LIB)

all: $(X_LINDENMAYER) $(X_KOCH) $(X_ULAM)

$(X_LINDENMAYER): $(OBJS_LINDENMAYER)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_LINDENMAYER) $(LIBS)

$(X_KOCH):
	make -C recursive

$(X_ULAM):
	make -C ulam

.PHONY: clean check

check:
	clang --analyze $(CFLAGS) $(OBJS_LINDENMAYER:.o=.c)
	make -C ulam check
	make -C recursive check

clean:
	rm -f $(X_LINDENMAYER) $(OBJS_LINDENMAYER) *.plist *~
	make -C ulam clean
	make -C recursive clean
	make -C webgl clean
