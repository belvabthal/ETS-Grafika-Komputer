CC     = gcc
CFLAGS = -Wall -O2 -I. $(shell pkg-config --cflags raylib)
LIBS   = $(shell pkg-config --libs raylib) -lm -ldl -lpthread

TARGET = simulasi_camping

# Menyesuaikan dengan arsitektur FSM dan Objek kita
SRCS = main.c \
       coords.c \
       src/algo/dda.c \
       src/algo/bresenham.c \
       src/algo/midcircle.c \
       src/ui/button.c \
       src/objects/car.c \
       src/objects/tent.c \
       src/objects/env.c \
       src/screens/screen_menu.c \
       src/screens/screen_sim.c \
       src/screens/screen_obj.c \
       src/screens/screen_about.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	LIBGL_ALWAYS_SOFTWARE=1 ./$(TARGET)