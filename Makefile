cxx      := g++
cxxflags := -Ofast -march=native -Wall
objs     := main.o \
			lexer/buffer.o \
			lexer/lexer.o \
			parser/parser.o \
			parser/astree.o \
			parser/syntaxunit.o \
			translator/translator.o
bin      := t

all: $(bin)

$(bin): $(objs)
	$(cxx) $(objs) -o $(bin)

%.o: %.c
	$(cxx) $(cxxflags) -c $<

clean:
	rm -f $(objs) $(bin)

.PHONY: all clean
.DEFAULT: all
