
# define compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -g

# define include directories
INCLUDE =

# define LFLAGS
LFLAGS =

# define libs to be linked (may require LFLAGS)
LIBS = -lX11 -lpng

# define all source files to be compiled
SRCS = test.cpp filter.cpp

# define OSX framework flag
FRAMEWORK =

# define the executable
MAIN = edges

# set up OS specific defines
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	# nothing at the moment
endif
ifeq ($(UNAME_S),Darwin)
	# building with X11 requires special includes
    INCLUDE += -I/usr/X11/include
    LFLAGS += -L/usr/X11/lib

    # all screen manipulation requires OSX frameworks
    FRAMEWORK += -framework ApplicationServices
endif

all:
	$(CC) $(SRCS) $(LIBS) $(LFLAGS) $(INCLUDE) $(FRAMEWORK) -o $(MAIN)
clean:
	rm $(MAIN) *.png
