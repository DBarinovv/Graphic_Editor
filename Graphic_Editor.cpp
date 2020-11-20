#include "D:\TX\TxLib.h"

#include "main_header.h"

//=============================================================================

void Create_Background ();

void Clear_And_Close_All ();

void Manager ();

//=============================================================================

int main ()
{
    // DO
    // is hidden, is focus
    // viewer controller
    //

//    ClAbstractWindow aw;
//    aw.MouseOver ();
//    aw.MouseOut ();
//    aw.MouseClick ();
//

//    ClScrollbar cls;
//    cls.MouseOver ();
//
//    ClTextureRectButton tb({100, 100}, {200, 300}, "Racket.bmp");
//    ClAbstractWindow& wtb = tb;
//    wtb.Draw ({});

    Create_Background ();

    ClApplication appl;

    appl.Start_Program ();

//    int sz_of_RGNDATA = Win32::GetRegionData (txDC(), 0, nullptr);
//
//    return 0;
////    RGNDATA *prev_reg = (RGNDATA *) calloc (sz_of_RGNDATA, sizeof (char));
////
////    GetRegionData (txDC(), sz_of_RGNDATA, prev_reg)
//
//    HRGN region = Win32::CreateRectRgn (500, 0, 600, 600);
//    Win32::SelectObject (txDC(), region);
//
//    ClRectButton wind({100, 101}, {1010, 705});
//    ClAbstractWindow& w = wind;
//
//    w.Draw ({{128, 0, 256}, "Denis", {59, 49, 157}});
//
//    txSleep (1000);
//
//    Win32::SelectObject (txDC(), prev_reg);
//    w.Draw ({{128, 0, 256}, "Denis", {59, 49, 157}});
//
//    Win32::DeleteObject (region);
//
////    w.Draw ({{128, 0, 256}, "Denis", {59, 49, 157}});
//
//    txSleep (1000);

//
//    int cnt = 0;
//    while (cnt <= 500)
//    {
//        while (!In (txMousePos(), {100, 101, 1010, 705})) {}
//
//        w.MouseOver ();
//
//        txSleep ();
//
//        while (In (txMousePos(), {100, 101, 1010, 705}))
//        {
//            w.MouseOver ();
//        }
//
//        w.MouseOut ();
//        txSleep ();
//        cnt++;
//    }
//
//    w.Delete ();
//
//    HDC texture = txLoadImage ("D:\\Cpp\\Graphic_Editor\\Resources\\Images\\button_up.bmp", IMAGE_BITMAP, LR_LOADFROMFILE, 100, 200);
//    if (!texture) printf ("Error!\n");
//
//    txBitBlt       (txDC(), 100, 100, 200, 200, texture, 0, 0);
//    txTransparentBlt (txDC(), 100, 100, 200, 200, texture, 0, 0);
//    txAlphaBlend     (txDC(), 100, 100, 200, 200, texture, 0, 0, 0);

//    return 0;

//    ClScrollbar sb({100, 100}, {125, 400});
//    ClAbstractWindow& ww = sb;
//
//    for (int i = 0; i < 10; i++)
//    {
//        ww.Draw ({});
//        txSleep (1000);
//    }
//    ww.Draw ({});

//    Draw_Args_t args = {{}, {} , {2, 4, 8}};
//
//    args.Print();

//    for (int i = 0; i < 10; i++)
//    {
//        ww.MouseClick ();
//        txSleep (1000);
//    }


//    w.Draw ({128, 0, 256}, "Denis", {5, 49, 157});
//    txSleep (1000);
//    w.MouseOver ();
//    txSleep (1000);
//    w.MouseOut ();


//    w.Draw ({128, 0, 256});

    Clear_And_Close_All ();

    return 0;
}

//=============================================================================

void Create_Background ()
{
    txCreateWindow (ns_global_vars::C_max_x_coord, ns_global_vars::C_max_y_coord);
    txSetFillColor (TX_WHITE);
    txRectangle (0, 0, ns_global_vars::C_max_x_coord, ns_global_vars::C_max_y_coord);
}

//-----------------------------------------------------------------------------

void Clear_And_Close_All ()
{
    fclose (ns_global_vars::log_file);

    Win32::DeleteObject (ns_global_vars::main_region);
}
