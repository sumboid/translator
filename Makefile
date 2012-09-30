cxx      := g++
cxxflags := -O3
objs     := main.o \
			buffer.o \
			lexer.o \
			parser.o
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
