#ifndef BUFFER_H
#define BUFFER_H

#include <istream>

namespace
{
    const size_t DEFAULT_BUFFER_SIZE = 512;
}

class buffer_t
{
public:
    buffer_t(std::istream& stream, size_t buffer_size = DEFAULT_BUFFER_SIZE);
    ~buffer_t();

    char peek();
    void next();
private:
    size_t buffer_size;
    size_t real_buffer_size;
    size_t current_buffer_position;

    char* buffer;

    std::istream& stream;

    bool end();
    bool fill_buffer();
};

#endif /* end of include guard: BUFFER_H */
