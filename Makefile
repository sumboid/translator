CXX      := g++
CXXFLAGS := -O0 -g  -Wall
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
	$(CXX) $(objs) -o $(bin)

clean:
	rm -f $(objs) $(bin)

.PHONY: all clean
.DEFAULT: all
