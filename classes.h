
// classes.h

/*
                            SCEME OF EDITOR

                            ClAbstractWindow
                                /      \
                               /        \
                              /          \
                             /            \
                            |              |
                            v              v
                         window        scrollbar
                            |
                            v
                         button


Color_t (size_t red, size_t green, size_t blue)

Coord_t (int x, int y)

Button_t (Coord_t left_up, Coord_t right_down)
*/

#include <vector>

#define UNPACKING_DRAW_ARGS(args)\
    const Color_t color = args.color;\
    char *buf = args.buf;\
    const Color_t text_color = args.text_color;\
    bool mouse_over = args.mouse_over;\
    int step = args.step;


class ClAbstractWindow // interface in Java
{
public:
    ClAbstractWindow () {}
    ClAbstractWindow (const Coord_t lu, const Coord_t rd) {}

    virtual bool Draw (const Draw_Args_t args) {}         // return true if color  has been changed
    virtual bool MouseOver  () {} // return true if mouse is over window
    virtual bool MouseOut   () {} // return true if mouse is no longer over window
    virtual bool Delete     () {} // return true if window has been deleted
    virtual bool MouseClick () { printf ("A"); }

//    void Print () { printf ("%p\n", this); }

    ~ClAbstractWindow() = default;
};

//-----------------------------------------------------------------------------

class ClRectWindow : public ClAbstractWindow
{
public:
    ClRectWindow () {}
    ClRectWindow (const Coord_t lu, const Coord_t rd) {}

    virtual bool Draw (const Draw_Args_t args) {}
    virtual bool MouseOver  () {}
    virtual bool MouseOut   () {}
    virtual bool Delete     () {}
    virtual bool MouseClick () {}

    ~ClRectWindow() = default;
};

//-----------------------------------------------------------------------------

class ClRectButton : public ClRectWindow
{
public:
    ClRectButton (const Coord_t lu = {}, const Coord_t rd = {}) :
                  left_up (lu), right_down (rd), bg_color({}), text(nullptr), text_color({}), prev_text_color({1, 8, 7}),
                  sz_text (std::min ((right_down.y - left_up.y) / 3, (right_down.x - left_up.x) / 3))
    {
        txRectangle (lu.x, lu.y, rd.x, rd.y);
    }

    virtual bool Draw (const Draw_Args_t args)
    {
        UNPACKING_DRAW_ARGS(args)

        bg_color = color;
        text = buf;

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

        Draw ({bg_color, text, {}, 1});
    }

    virtual bool MouseOut ()
    {
        Draw ({bg_color, text, {prev_text_color.red,
                               prev_text_color.green,
                               prev_text_color.blue}});
    }

    virtual bool Delete ()
    {
        Draw ({});
    }

    virtual bool MouseClick () {}

    int Get_And_Change_LU_Y (int offset = 0) { left_up.y    += offset; return left_up.y; }
    int Get_And_Change_RD_Y (int offset = 0) { right_down.y += offset; return right_down.y; }


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

//=============================================================================

class ClScrollbar : public ClAbstractWindow
{
public:
    ClScrollbar (const Coord_t lu = {}, const Coord_t rd = {}) :
                 up          (lu,                                   {rd.x, lu.y + (rd.y - lu.y) / 6}),
                 back_ground ({lu.x, lu.y + (rd.y - lu.y) / 6},     {rd.x, lu.y + (rd.y - lu.y) / 6 * 5}),
                 slider      ({lu.x, lu.y + (rd.y - lu.y) / 6},     {rd.x, lu.y + (rd.y - lu.y) / 6 + (rd.y - lu.y) / 12}),
                 down        ({lu.x, lu.y + (rd.y - lu.y) / 6 * 5},  rd)
    {
        number_of_first_string = 1;
        max_cnt_of_string = 10; // random number for test (depends on text)

        txRectangle (lu.x, lu.y, rd.x, rd.y);
    }

    virtual bool Draw (const Draw_Args_t args)
    {
        UNPACKING_DRAW_ARGS(args)

        slider.Delete();

        up.Draw ({{154, 154, 154}});
        down.Draw ({{154, 154, 154}});
        back_ground.Draw ({{255, 255, 0}});


        slider.Get_And_Change_LU_Y (step); // equal to  { slider.left_up.y    += step;
        slider.Get_And_Change_RD_Y (step); //           { slider.right_down.y += step;

//        printf ("RD = [%d],  LU = [%d],  height = [%d]\n\n", slider.Get_And_Change_RD_Y (), down.Get_And_Change_LU_Y (), slider_height);

        slider.Draw ({{237, 48, 0}});
    }

    virtual bool MouseOver () {}

    virtual bool MouseOut () {}

    virtual bool Delete ()
    {
        up.Delete ();
        down.Delete ();
        back_ground.Delete ();
        slider.Delete ();
    }

    virtual bool MouseClick ()
    {
//        printf ("number_of_first_string = [%d]\n", number_of_first_string);

        int slider_height = slider.Get_And_Change_RD_Y() - slider.Get_And_Change_LU_Y();
        int bg_height     = down.Get_And_Change_LU_Y() - up.Get_And_Change_RD_Y();

        int step = (bg_height - slider_height) / max_cnt_of_string;

//        printf ("RD = [%d],  LU = [%d],  height = [%d]\n", slider.Get_And_Change_RD_Y (), down.Get_And_Change_LU_Y (), slider_height);

        if (number_of_first_string >= max_cnt_of_string)
        {
//            printf ("Offset = [%d]\n", step);
            step = down.Get_And_Change_LU_Y () - slider.Get_And_Change_RD_Y ();
//            printf ("Offset = [%d]\n", step);
        }
        else if (number_of_first_string <= 0)
        {
            step = up.Get_And_Change_RD_Y () - slider.Get_And_Change_LU_Y ();
        }

        int rd = (rand() % 3 - 1);
        step *= rd;

//        printf ("number_of_first_string = [%d],   step = [%d]\n", number_of_first_string, step);

        if (((slider.Get_And_Change_RD_Y () == down.Get_And_Change_LU_Y ()) && step > 0) ||
            ((slider.Get_And_Change_LU_Y () ==   up.Get_And_Change_RD_Y ()) && step < 0))
        {
            step = 0;
//            printf ("AAA\n");
        }

//        if ()
//        {


        Draw ({{}, {}, {}, {}, step});
//        }

        number_of_first_string += rd;

        if (number_of_first_string > max_cnt_of_string)
        {
            number_of_first_string = max_cnt_of_string;
        }
        else if (number_of_first_string < 1)
        {
            number_of_first_string = 1;
        }
    }

private:
    ClRectButton up;
    ClRectButton down;
    ClRectButton back_ground;
    ClRectButton slider;
    size_t max_cnt_of_string;
    size_t number_of_first_string;
};

//=============================================================================

class ClApplication
{
public:

    ClApplication ()
    {
//        ClAbstractWindow *ww = new ClAbstractWindow(w);

//        arr_of_windows[0]->Print ();

//        arr_of_windows[0]->Draw ({});
//        ww->Draw ({});
    }

    void Start_Program ()
    {
        ClScrollbar sb({100, 100}, {125, 400});
        ClAbstractWindow& ww = sb;

        while (!txGetAsyncKeyState (VK_ESCAPE))
        {
            int helper = 0;
            if ((helper = txMouseButtons()) != 3)
            {
                if (helper & 1)
                {
//                    arr_of_windows[0]->Print ();
//                    arr_of_windows[0]->Draw ({});
//                    arr_of_windows[0].MouseClick ();
                    for (int i = 0; i < 50; i++)
                    {
                        ww.MouseClick ();
                        txSleep (300);
                    }
                }
            }
        }
    }

private:
    std::vector<ClAbstractWindow> arr_of_windows;
//    ClManager
};

//-----------------------------------------------------------------------------

class ClManager : public ClAbstractWindow
{
public:
    virtual bool Draw (const Draw_Args_t args) {}
    virtual bool MouseOver  () {}
    virtual bool MouseOut   () {}
    virtual bool Delete     () {}
    virtual bool MouseClick () {}

//    operator() (const& ClAbstractWindow, )

private:
};
