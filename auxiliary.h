

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

//-----------------------------------------------------------------------------

struct Draw_Args_t
{
    Draw_Args_t (const Color_t color = {}, char *buf = nullptr, const Color_t text_color = {},
                 bool mouse_over = false, const int step = 0) :
                 color (color), buf (buf), text_color (text_color), mouse_over (mouse_over), step (step) {}


    const Color_t color;
    char *buf;
    const Color_t text_color;
    bool mouse_over;
    int step;

    void Print ()
    {
        printf ("color.red        = [%d]\n", color.red);
        printf ("color.green      = [%d]\n", color.green);
        printf ("color.blue       = [%d]\n", color.blue);

        printf ("buf              = [%p]\n", buf);

        printf ("text_color.red   = [%d]\n", text_color.red);
        printf ("text_color.green = [%d]\n", text_color.green);
        printf ("text_color.blue  = [%d]\n", text_color.blue);

        printf ("Mouse over       = [%d]\n", mouse_over);

        printf ("Step             = [%d]\n", step);
    }
};
