#include <istream>


class buffer_t
{
public:
    buffer_t(std::istream& stream, size_t buffer_size);
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
