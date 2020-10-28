#include "D:\TX\TxLib.h"

#include "auxiliary.h"

/*
                SCEME OF EDITOR

                ClAbstractWindow
                        |
                        v
                     window
                        |
                        v
                     button


Color_t (size_t red, size_t green, size_t blue)

Coord_t (int x, int y)

Button_t (Coord_t left_up, Coord_t right_down)

Events
*/


//=============================================================================

const int C_max_x_coord = 1200;
const int C_max_y_coord = 800;

//=============================================================================


class ClAbstractWindow // interface in Java
{
public:
    ClAbstractWindow () {}
    ClAbstractWindow (const Coord_t lu, const Coord_t rd) {}

    virtual bool Draw (const Color_t color = {},
                       char *buf = nullptr, const Color_t text_color = {}, bool mouse_over = false) {};         // return true if color  has been changed
    virtual bool MouseOver () {}; // return true if mouse is over window
    virtual bool MouseOut  () {}; // return true if mouse is no longer over window
    virtual bool Delete () {};    // return true if window has been deleted

    ~ClAbstractWindow() = default;
};

//-----------------------------------------------------------------------------

class ClRectWindow : public ClAbstractWindow
{
public:
    ClRectWindow () {}
    ClRectWindow (const Coord_t lu, const Coord_t rd) {}

    virtual bool Draw (const Color_t color = {},
                       char *buf = nullptr, const Color_t text_color = {}, bool mouse_over = false) {};
    virtual bool MouseOver () {};
    virtual bool MouseOut () {};
    virtual bool Delete () {};

    ~ClRectWindow() = default;
};

//-----------------------------------------------------------------------------

class ClRectButton : public ClRectWindow
{
public:
    ClRectButton (const Coord_t lu, const Coord_t rd) :
                  left_up (lu), right_down (rd), bg_color({}), text(nullptr), text_color({}), prev_text_color({1, 8, 7}),
                  sz_text (std::min ((right_down.y - left_up.y) / 3, (right_down.x - left_up.x) / 3))
    {
        txRectangle (lu.x, lu.y, rd.x, rd.y);
    }

    virtual bool Draw (const Color_t color = {},
                       char *buf = nullptr, const Color_t tx_color = {}, bool mouse_over = false)
    {
        bg_color = color;
        text = buf;
        text_color = tx_color;

        txSetFillColor (RGB (bg_color.red, bg_color.green, bg_color.blue));
        txRectangle (left_up.x, left_up.y, right_down.x, right_down.y);

        if (text != nullptr)
        {
            txSetColor (RGB (text_color.red, text_color.green, text_color.blue));

            txSelectFont ("Comic Sans MS", sz_text);
            if (mouse_over)
            {
                txSelectFont ("Times New Roman", sz_text);
            }


            const int sz_buf = strlen (text);
            txTextOut ((left_up.x + right_down.x) / 2 - sz_buf / 2 * sz_text / 2,
                       (left_up.y + right_down.y) / 2 - sz_buf / 2 * sz_text / 4,
                       text);


            txSetColor (TX_WHITE);
            txSelectFont ("Comic Sans MS", sz_text);
        }

    }

    virtual bool MouseOver ()
    {
        Color_t def = {1, 8, 7};
        if (prev_text_color == def)
        {
            prev_text_color = text_color;
        }

        Draw (bg_color, text, {}, 1);
    }

    virtual bool MouseOut ()
    {
        Draw (bg_color, text, {prev_text_color.red,
                               prev_text_color.green,
                               prev_text_color.blue});
    }

    virtual bool Delete ()
    {
        Draw ();
    }


    ClRectButton& operator= (const ClRectButton& other)
    {
        left_up          = other.left_up;
        right_down       = other.right_down;
        bg_color         = other.bg_color;
        text             = other.text;
        text_color       = other.text_color;
        prev_text_color  = other.prev_text_color;

        return *this;
    }

    ~ClRectButton() = default;

private:
    Coord_t left_up;
    Coord_t right_down;
    Color_t bg_color;
    char *text;
    Color_t text_color;
    Color_t prev_text_color;
    int sz_text;
};

//-----------------------------------------------------------------------------

void Create_Background ();

//=============================================================================

int main ()
{
    Create_Background ();

    ClRectButton wind({100, 101}, {1010, 705});
    ClAbstractWindow& w = wind;

    w.Draw ({128, 0, 256}, "Denis", {59, 49, 157});

    int cnt = 0;
    while (cnt != 50)
    {
//        while (!In (txMousePos(), {100, 101, 1010, 705})) {}

        w.MouseOver ();

        txSleep ();

//        while (In (txMousePos(), {100, 101, 1010, 705}))
//        {
//            w.MouseOver ();
//        }

        w.MouseOut ();
        txSleep ();
        cnt++;
    }

    w.Delete ();

//    w.Draw ({128, 0, 256}, "Denis", {5, 49, 157});
//    txSleep (1000);
//    w.MouseOver ();
//    txSleep (1000);
//    w.MouseOut ();


//    w.Draw ({128, 0, 256});

    return 0;
}

//=============================================================================

void Create_Background ()
{
    txCreateWindow (C_max_x_coord, C_max_y_coord);
    txSetFillColor (TX_WHITE);
    txRectangle (0, 0, C_max_x_coord, C_max_y_coord);
}
