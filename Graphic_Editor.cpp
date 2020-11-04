#include "D:\TX\TxLib.h"

#include "auxiliary.h"

#include "classes.h"

//=============================================================================

const int C_max_x_coord = 1200;
const int C_max_y_coord = 800;

//=============================================================================

void Create_Background ();

void Manager ();

//=============================================================================

int main ()
{
    Create_Background ();

    ClApplication appl;

    appl.Start_Program ();

//    ClRectButton wind({100, 101}, {1010, 705});
//    ClAbstractWindow& w = wind;
//
//    w.Draw ({{128, 0, 256}, "Denis", {59, 49, 157}});
//
//    int cnt = 0;
//    while (cnt >= 500)
//    {
//        while (!In (txMousePos(), {100, 101, 1010, 705})) {}
//
//        w.MouseOver ();
//
//        txSleep ();
//
////        while (In (txMousePos(), {100, 101, 1010, 705}))
////        {
////            w.MouseOver ();
////        }
//
//        w.MouseOut ();
//        txSleep ();
//        cnt++;
//    }
//
//    w.Delete ();
//
//    ClScrollbar sb({100, 100}, {125, 400});
//    ClAbstractWindow& ww = sb;

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

    return 0;
}

//=============================================================================

void Create_Background ()
{
    txCreateWindow (C_max_x_coord, C_max_y_coord);
    txSetFillColor (TX_WHITE);
    txRectangle (0, 0, C_max_x_coord, C_max_y_coord);
}
