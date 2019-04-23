includes := 
libraries := 
bindir := bin
debugflags := -g3 -O0 -Wall
srcs := $(wildcard *.c)
objs := $(addprefix $(bindir)/, $(srcs:.c=.o))

$(bindir)/main: $(objs)
	$(CC) $(CFLAGS) -g3 -O0 -Wall $(CPPFLAGS) $? -o $@ $(includes) $(libraries)

$(bindir)/main.o: main.c | binfolder
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(debugflags) $< -o $@ $(includes)

$(bindir)/%.o: %.c %.h | binfolder
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(debugflags) $< -o $@ $(includes)


binfolder:
	mkdir -p bin

clean:
	rm -r -f bin

PHONY: clean