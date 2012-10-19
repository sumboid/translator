cxx      := g++
cxxflags := -O3
objs     := main.o \
			lexer/buffer.o \
			lexer/lexer.o \
			parser/parser.o \
			parser/astree.o \
			parser/syntaxunit.o
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
