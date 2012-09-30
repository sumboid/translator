#include "buffer.h"

namespace
{
    const size_t DEFAULT_BUFFER_SIZE = 512;
}

using std::istream;

buffer_t::buffer_t(istream& _stream, size_t _buffer_size = DEFAULT_BUFFER_SIZE)
    :stream(stream), buffer_size(_buffer_size)
{
    buffer = new char[buffer_size];
    fill_buffer();
}

buffer_t::~buffer_t()
{
    delete[] buffer;
}

bool buffer_t::fill_buffer()
{
    if(stream.good())
    {
        stream.get(buffer, buffer_size);
        real_buffer_size = gcount();
        current_buffer_position = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool buffer_t::end()
{
    if(current_buffer_position == real_buffer_size)
    {
        return true;
    }
    return false;
}

char buffer_t::peek()
{
    if(end)
    {
        if(false == fill_buffer()) //need exception
        {
            return -1;
        }
    }

    return buffer[current_buffer_position];
}

void buffer_t::next()
{
    if(end)
    {
        if(false == fill_buffer()) //need exception
        {
            return -1;
        }
    }

    current_buffer_position++;
}
