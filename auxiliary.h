

// auxiliary.h

//=============================================================================

enum {
        E_MouseOver   = 0,
        E_MouseClick  = 1,
        E_Draw        = 2,
        E_Create      = 3,
        E_MouseOut    = 4,
     } Events;

//-----------------------------------------------------------------------------

struct Color_t
{
    Color_t (const int r = 255, const int gr = 255, const int bl = 255) : red(r), green(gr), blue(bl) {}

    size_t red;
    size_t green;
    size_t blue;

    bool operator!= (const Color_t& other)
    {
        return (this->red == other.red && this->green == other.green && this->blue == other.blue);
    }

    bool operator== (const Color_t& other)
    {
        return (*this != other);
    }

    ~Color_t() = default;
};

//-----------------------------------------------------------------------------

struct Coord_t
{
    Coord_t (const int x = 0, const int y = 0) : x(x), y(y) {}

    int x;
    int y;

    ~Coord_t() = default;
};
