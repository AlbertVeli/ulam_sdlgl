# (k) ALL RIGHTS REVERSED - Reprint what you like

X_ULAM = ulam
OBJS_ULAM = $(X_ULAM).o primes.o sdlgl.o

X_KOCH = koch
OBJS_KOCH = $(X_KOCH).o sdlgl.o

X_LINDENMAYER = lindenmayer
OBJS_LINDENMAYER = $(X_LINDENMAYER).o sdlgl.o

# Uncomment line below to enable debug build
#DEBUG = yes

ifdef DEBUG
DBGFLAGS = -g -O0
else
DBGFLAGS = -O2 -march=native
endif

WFLAGS = -W -Wall

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

all: $(X_KOCH) $(X_LINDENMAYER) $(X_ULAM)

$(X_KOCH): $(OBJS_KOCH)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_KOCH) $(LIBS)

$(X_LINDENMAYER): $(OBJS_LINDENMAYER)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_LINDENMAYER) $(LIBS)

$(X_ULAM): $(OBJS_ULAM)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_ULAM) $(LIBS)

.PHONY: clean

clean:
	rm -f $(X_ULAM) $(OBJS_ULAM) $(X_KOCH) $(OBJS_KOCH) $(X_LINDENMAYER) $(OBJS_LINDENMAYER) *~
