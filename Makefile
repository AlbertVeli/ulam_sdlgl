# (k) ALL RIGHTS REVERSED - Reprint what you like

eXe = ulam
OBJS = $(eXe).o primes.o

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

$(eXe): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(eXe) $(OBJS) *~
