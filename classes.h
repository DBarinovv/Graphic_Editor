
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

namespace { namespace TX { namespace Win32 {
    _TX_DLLIMPORT ("GDI32", HRGN,  CreateRectRgn, (int x0, int y0, int x1, int y1));
    _TX_DLLIMPORT ("GDI32", DWORD, GetRegionData, (HRGN hrgn, DWORD nCount, LPRGNDATA lpRgnData));
}}}

#include <vector>

#define UNPACKING_DRAW_ARGS(args)               \
    const Color_t color = args.color;           \
    char *buf = args.buf;                       \
    const Color_t text_color = args.text_color; \
    bool mouse_over = args.mouse_over;

//=============================================================================

class ClViewport
{
public:
    ClViewport (const Coord_t lu = {0, 0},
                const Coord_t rd = {ns_global_vars::C_max_x_coord, ns_global_vars::C_max_y_coord}) :
                new_region (Win32::CreateRectRgn (lu.x, lu.y, rd.x, rd.y)),
                left_up (lu), right_down (rd) {};

    virtual ~ClViewport ()
    {
        Win32::DeleteObject (new_region);
    }

    // change
    ClViewport (const ClViewport&) = default;
    ClViewport (ClViewport&& dead_viewport) : new_region (std::move (dead_viewport.new_region)) {}

    ClViewport& operator=(const ClViewport&) = default;
    ClViewport& operator=(ClViewport&& dead_viewport)
    {
        new_region = std::move (dead_viewport.new_region);

        return *this;
    }


    HRGN Ret_Region () { return new_region; }

    void Make_Region ()
    {
        Win32::SelectObject (txDC(), new_region);
    }

    void Remove_Region ()
    {
        Win32::SelectObject (txDC(), ns_global_vars::main_region);
    }

    int Get_And_Change_LU_Y (int offset = 0) { left_up.y    += offset; return left_up.y; }
    int Get_And_Change_RD_Y (int offset = 0) { right_down.y += offset; return right_down.y; }
    int Get_And_Change_LU_X (int offset = 0) { left_up.x    += offset; return left_up.x; }
    int Get_And_Change_RD_X (int offset = 0) { right_down.x += offset; return right_down.x; }

private:
    HRGN new_region;
    Coord_t left_up;
    Coord_t right_down;
};

//=============================================================================

class ClAbstractWindow // interface in Java
{
public:
//    ClAbstractWindow () {}
    ClAbstractWindow (const Coord_t lu = {}, const Coord_t rd = {}) {}

    virtual ~ClAbstractWindow ()                         = default;

    ClAbstractWindow (const ClAbstractWindow&)           = default;
    ClAbstractWindow (ClAbstractWindow&&)                = default;

    ClAbstractWindow& operator=(const ClAbstractWindow&) = default;
    ClAbstractWindow& operator=(ClAbstractWindow&&)      = default;

    virtual bool Draw (const Draw_Args_t args) { PR_LOG }         // return true if color has been changed
    virtual bool MouseOver  () { PR_LOG } // return true if mouse is over window
    virtual bool MouseOut   () { PR_LOG } // return true if mouse is no longer over window
    virtual bool Delete     () { PR_LOG } // return true if window has been deleted
    virtual bool MouseClick (const Coord_t coords, const ClAbstractWindow *last_window) { PR_LOG }
    virtual bool Coord_In   (const Coord_t coords) { PR_LOG }

//    void Print () { printf ("%p\n", this); }
};

//-----------------------------------------------------------------------------

class ClRectWindow : public ClAbstractWindow
{
public:
    ClRectWindow () {}
    ClRectWindow (const Coord_t lu, const Coord_t rd) {}

    virtual bool Draw (const Draw_Args_t args) { PR_LOG }
    virtual bool MouseOver  () { PR_LOG }
    virtual bool MouseOut   () { PR_LOG }
    virtual bool Delete     () { PR_LOG }
    virtual bool MouseClick (const Coord_t coords, const ClAbstractWindow *last_window) { PR_LOG }
    virtual bool Coord_In   (const Coord_t coords) { PR_LOG }

    virtual ~ClRectWindow() = default;
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

    virtual bool MouseClick (const Coord_t coords, const ClAbstractWindow *last_window) { PR_LOG }

    virtual bool Coord_In (const Coord_t coords)
    {
        return coords.x >= right_down.x && coords.x <= left_up.x &&
               coords.y >= right_down.y && coords.x <= left_up.y;
    }

    virtual int Get_And_Change_LU_Y (int offset = 0) { left_up.y    += offset; return left_up.y; }
    virtual int Get_And_Change_RD_Y (int offset = 0) { right_down.y += offset; return right_down.y; }
    virtual int Get_And_Change_LU_X (int offset = 0) { left_up.x    += offset; return left_up.x; }
    virtual int Get_And_Change_RD_X (int offset = 0) { right_down.x += offset; return right_down.x; }


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

    virtual ~ClRectButton() = default;

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

class ClTextureRectButton : public ClRectButton
{
public:
    ClTextureRectButton (const Coord_t lu = {}, const Coord_t rd = {}, const std::string& image = "") :
                        ClRectButton (lu, rd), left_up(lu), right_down(rd)
    {
        std::string path = "Resources\\Images\\";
//        std::string path = "D:\\TX\\Examples\\Tennis\\Resources\\Images\\";
        path += image;
        texture = NOtxLoadImage (path.c_str(), IMAGE_BITMAP, LR_LOADFROMFILE,
                                                                            right_down.x - left_up.x,
                                                                            right_down.y - left_up.y);
//        texture = txLoadImage ("Resources\\Images\\button_up.bmp");

        if (!texture)
        {
            std::string out = "�� ���� ��������� ��������(\nPath = ";
            out += path;

            txMessageBox (out.c_str(), "��, � ���������� ��� �� �������");
        }
    }

    virtual ~ClTextureRectButton ()
    {
        txDeleteDC (texture);
    };

    ClTextureRectButton (const ClTextureRectButton&) = default;
    ClTextureRectButton (ClTextureRectButton&& dead_button) :
        left_up    (std::move (dead_button.left_up)),
        right_down (std::move (dead_button.right_down)),
        texture    (std::move (dead_button.texture)) {}

    ClTextureRectButton& operator=(const ClTextureRectButton&) = default;
    ClTextureRectButton& operator=(ClTextureRectButton&& deleted_button)
    {
        // ?
    }


    virtual bool Draw (const Draw_Args_t args)
    {
        if (left_up.y < 0)
        {
            txBitBlt (txDC(), left_up.x, 0, right_down.x, right_down.y, texture, 0, -left_up.y);
        }
        else
        {
            txBitBlt (txDC(), left_up.x, left_up.y, right_down.x, right_down.y, texture, 0, 0);
        }


    }

    virtual bool MouseOver  () { PR_LOG }
    virtual bool MouseOut   () { PR_LOG }
    virtual bool Delete     () { PR_LOG }
    virtual bool MouseClick (const Coord_t coords, const ClAbstractWindow *last_window) { PR_LOG }

    virtual bool Coord_In (const Coord_t coords)
    {
        return coords.x >= right_down.x && coords.x <= left_up.x &&
               coords.y >= right_down.y && coords.x <= left_up.y;
    }

    virtual int Get_And_Change_LU_Y (int offset = 0) { left_up.y    += offset; return left_up.y; }
    virtual int Get_And_Change_RD_Y (int offset = 0) { right_down.y += offset; return right_down.y; }
    virtual int Get_And_Change_LU_X (int offset = 0) { left_up.x    += offset; return left_up.x; }
    virtual int Get_And_Change_RD_X (int offset = 0) { right_down.x += offset; return right_down.x; }

private:
    Coord_t left_up;
    Coord_t right_down;
    HDC texture;
};

//=============================================================================

class ClScrollbar : public ClAbstractWindow
{
private:
    ClTextureRectButton up;
    ClTextureRectButton down;
    ClRectButton        back_ground;
    ClRectButton        slider;
    size_t              max_cnt_of_string;
    size_t              number_of_first_string;
    bool                is_focused;
    double              step;

    ClTextureRectButton image;
    ClViewport          view;

public:
    ClScrollbar (const Coord_t lu = {}, const Coord_t rd = {}) :
                 up          (lu,                                   {rd.x, lu.y + (rd.y - lu.y) / 6}, "button_up.bmp"),
                 back_ground ({lu.x, lu.y + (rd.y - lu.y) / 6},     {rd.x, lu.y + (rd.y - lu.y) / 6 * 5}),
                 slider      ({lu.x, lu.y + (rd.y - lu.y) / 6},     {rd.x, lu.y + (rd.y - lu.y) / 6 + (rd.y - lu.y) / 12}),
                 down        ({lu.x, lu.y + (rd.y - lu.y) / 6 * 5},  rd, "button_down.bmp"),
                 view        ({rd.x, lu.y},                         {rd.x + 600, rd.y}),
                 image       ({rd.x, lu.y},                         {rd.x + 600, rd.y + 200}, "cat.bmp"),
                 is_focused  (false), step (0.0)
    {
        number_of_first_string = 1;
        max_cnt_of_string = 10; // random number for test (depends on text)

        txRectangle (lu.x, lu.y, rd.x, rd.y);
    }

    virtual bool Draw (const Draw_Args_t args)
    {
        UNPACKING_DRAW_ARGS(args)

        slider.Delete();
//        image.Delete();

        up.Draw ({{154, 154, 154}});
        down.Draw ({{154, 154, 154}});
        back_ground.Draw ({{255, 255, 0}});


        slider.Get_And_Change_LU_Y (step); // equal to  { slider.left_up.y    += step;
        slider.Get_And_Change_RD_Y (step); //           { slider.right_down.y += step;

//        printf ("RD = [%d],  LU = [%d],  height = [%d]\n\n", slider.Get_And_Change_RD_Y (), down.Get_And_Change_LU_Y (), slider_height);

        slider.Draw ({{237, 48, 0}});

        view.Make_Region ();
        image.Draw ({});
        view.Remove_Region ();
    }

    virtual bool MouseOver () { PR_LOG }

    virtual bool MouseOut () { PR_LOG }

    virtual bool Delete ()
    {
        up.Delete ();
        down.Delete ();
        back_ground.Delete ();
        slider.Delete ();
    }

    virtual bool MouseClick (const Coord_t coords, const ClAbstractWindow *last_window)
    {
//        printf ("number_of_first_string = [%d]\n", number_of_first_string);
        if (last_window != this) is_focused = false;

        int slider_height = slider.Get_And_Change_RD_Y() - slider.Get_And_Change_LU_Y();
        int bg_height     = down.Get_And_Change_LU_Y() - up.Get_And_Change_RD_Y();

        step = (bg_height - slider_height) / max_cnt_of_string;

//        int rd = (rand() % 3 - 1);
//        step *= rd;
        double way = 0.0;
        bool change = false;

        if (coords.x >= up.Get_And_Change_LU_X () && coords.x <= down.Get_And_Change_RD_X ())
        {
            if (coords.y >= up.Get_And_Change_RD_Y () && coords.y <= slider.Get_And_Change_LU_Y ())          // higher than slider
            {
                way = -1.0 / max_cnt_of_string;
                is_focused = false;
            }
            else if (coords.y >= slider.Get_And_Change_RD_Y () && coords.y <= down.Get_And_Change_LU_Y ())   // below slider
            {
                way = 1.0 / max_cnt_of_string;
                is_focused = false;
            }
            else if (coords.y >= slider.Get_And_Change_LU_Y () && coords.y <= slider.Get_And_Change_RD_Y ()) // over slider
            {
                if (is_focused)
                {
                    int mid_of_slider = (slider.Get_And_Change_LU_Y () + (slider_height / 2));
                    step = coords.y - mid_of_slider;

//                    printf ("1Step = [%d]\n", step);

                    Clamp (step, up.Get_And_Change_RD_Y () + (slider_height / 2) - mid_of_slider,
                               down.Get_And_Change_LU_Y () - (slider_height / 2) - mid_of_slider);

//                    printf ("2Step = [%d]\n\n", step);

                    way = 1.0;
                }
                else
                {
                    way = 0.0;
                    change = true;
                }

            }
            else if (coords.y >= up.Get_And_Change_LU_Y ()   && coords.y <=  up.Get_And_Change_RD_Y ())  // up button
            {
                way = -2.0 / (max_cnt_of_string * 3);
                is_focused = false;
            }
            else if (coords.y >= down.Get_And_Change_LU_Y () && coords.y <= down.Get_And_Change_RD_Y ()) // down button
            {
                way = 2.0 / (max_cnt_of_string * 3);
//                way = 1.0 / (max_cnt_of_string);
                is_focused = false;
            }
        }

        step *= way;

//        printf ("Step = [%f]\n", step);

        Clamp (step, up.Get_And_Change_RD_Y () - slider.Get_And_Change_LU_Y (),
                   down.Get_And_Change_LU_Y () - slider.Get_And_Change_RD_Y ());

//        printf ("Step = [%f]\n\n", step);

//        printf ("RD = [%d],  LU = [%d],  height = [%d]\n", slider.Get_And_Change_RD_Y (), down.Get_And_Change_LU_Y (), slider_height);

//        if (!is_focused)
//        {
//            if (number_of_first_string >= max_cnt_of_string)
//            {
//    //            printf ("Offset = [%d]\n", step);
//                step = down.Get_And_Change_LU_Y () - slider.Get_And_Change_RD_Y ();
//    //            printf ("Offset = [%d]\n", step);
//            }
//            else if (number_of_first_string <= 1)
//            {
//                step = up.Get_And_Change_RD_Y () - slider.Get_And_Change_LU_Y ();
//            }
//        }


//        printf ("number_of_first_string = [%d],   step = [%d]\n", number_of_first_string, step);
//        printf ("1Step = [%d]\n", step);

//        if (((slider.Get_And_Change_RD_Y () == down.Get_And_Change_LU_Y ()) && step > 0) ||
//            ((slider.Get_And_Change_LU_Y () ==   up.Get_And_Change_RD_Y ()) && step < 0))
//        {
//            step = 0.0;
////            printf ("AAA\n");
//        }

        double image_step = (((image.Get_And_Change_RD_Y () - image.Get_And_Change_LU_Y ()) -
                              ( view.Get_And_Change_RD_Y () -  view.Get_And_Change_LU_Y ())) / (bg_height - slider_height)) * step;

        if (step == 0.0) image_step = 0.0;
//        printf ("image_step = [%d]\n", image_step);

        image.Get_And_Change_LU_Y (-image_step); //}
        image.Get_And_Change_RD_Y (-image_step); //} - for reverse

//        if ()
//        {

//        printf ("2Step = [%d]\n\n", step);


        Draw ({});

        step = 0.0;
//        }

//        number_of_first_string += way;
//
//        Clamp (number_of_first_string, 1, max_cnt_of_string);

        if (change) is_focused = true;

//        if (number_of_first_string > max_cnt_of_string)
//        {
//            number_of_first_string = max_cnt_of_string;
//        }
//        else if (number_of_first_string < 1)
//        {
//            number_of_first_string = 1;
//        }
    }

    virtual bool Coord_In (const Coord_t coords)
    {
        PR_LOG

//        printf ("BOOL = [%d]  ", coords.x >= down.Get_And_Change_RD_X () && coords.x <= up.Get_And_Change_LU_X () &&
//               coords.y >= down.Get_And_Change_RD_Y () && coords.y <= up.Get_And_Change_LU_Y ());
//
//        printf ("coords.x = [%d] coords.y = [%d] ", coords.x, coords.y);
//
//        printf ("down RD = [%d], [%d]  ", down.Get_And_Change_RD_X (), down.Get_And_Change_RD_Y ());
//        printf ("up LU = [%d], [%d]\n", up.Get_And_Change_LU_X (), up.Get_And_Change_LU_Y ());

        return coords.x <= down.Get_And_Change_RD_X () && coords.x >= up.Get_And_Change_LU_X () &&
               coords.y <= down.Get_And_Change_RD_Y () && coords.y >= up.Get_And_Change_LU_Y ();
    }
};

//=============================================================================

class ClApplication
{
private:
    std::vector<ClAbstractWindow *> arr_of_windows;

public:

    ClApplication () = default;
    virtual ~ClApplication () = default;
//    {
//        for (auto& window : arr_of_windows)
//        {
//            window->~ClAbstractWindow();
//        }
//    }

    void Start_Program ()
    {
        ns_global_vars::main_region = Win32::CreateRectRgn (0, 0, ns_global_vars::C_max_x_coord, ns_global_vars::C_max_y_coord);
        arr_of_windows.emplace_back (new ClAbstractWindow);

        ClScrollbar sb({100, 100}, {150, 400});
        ClAbstractWindow& ww = sb;
        arr_of_windows.push_back (&ww);

//        ClScrollbar sb2({150, 120}, {240, 420});
//        ClAbstractWindow& ww2 = sb2;
//        arr_of_windows.push_back (&ww2);

        Draw_All();

        PR_LOG

        int helper = 0;
        ClAbstractWindow *last_window = arr_of_windows[0];

        txBegin ();

        Draw_All();
        txSleep ();

        while (!txGetAsyncKeyState (VK_ESCAPE))
        {
            helper = 0;
            if ((helper = txMouseButtons()) != 3)
            {
                if (helper & 1)
                {
                    Coord_t coords = txMousePos();

                    for (int i = arr_of_windows.size() - 1; i >= 0; i--)
                    {
                        if (arr_of_windows[i]->Coord_In (coords))
                        {
                            arr_of_windows[i]->MouseClick (coords, last_window);
                            arr_of_windows[i]->Draw (Draw_Args_t());

                            last_window = arr_of_windows[i];

                            if (i != 0)
                            {
                                for (int k = i; k < arr_of_windows.size() - 1; k++)       //}
                                {                                                         //|
                                    std::swap (arr_of_windows[k], arr_of_windows[k + 1]); //| Make i window last
                                    PR_LOG
                                }                                                         //}
                            }

                            break;
                        }
                    }

//                    printf ("Click!\n");

                    Draw_All();
                    txSleep ();
                }
            }
        }

        txEnd ();
    }

    void Draw_All ()
    {
        for (auto& window : arr_of_windows)
        {
            window->Draw(Draw_Args_t());
        }
    }
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
