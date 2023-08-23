CC = gcc
CFLAGS = -Wall -Wno-switch -Wno-multichar

SRC = console.c \
      error.c   \
      expressions.c  \
      flow_visit.c   \
      format_visit.c \
      fs.c       \
      hash.c     \
      hashmap.c  \
      main.c     \
      options.c  \
      osstream.c \
      parser.c   \
      pre_expressions.c \
      token.c     \
      tokenizer.c \
      type.c      \
      visit.c

OBJ = $(SRC:.c=.o)

EXE = cake

cake: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f cake $(OBJ)

cake_single.c: $(SRC)
	./amalgamate.sh > $@

cake_single: cake_single.o
	$(CC) $(LDFLAGS) -o $@ cake_single.o
