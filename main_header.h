
// main_header.h

#include "config.h"

//=============================================================================

#define PR_LOG \
Print_Log (__FILE__, __func__, __LINE__);

//=============================================================================

template<typename T, typename V, typename W>
inline void Clamp (T& var, const V& down, const W& up)
{
    if (var < down)
    {
        var = down;
    }
    else if (var > up)
    {
        var = up;
    }
}

//=============================================================================

inline void Print_Log (const char file[], const char name_of_function[], const int line)
{
    std::string res_file = "FILE = (";
    res_file += strrchr (file, '\\') + 1;
    res_file += ")";

    std::string res_name = "FUNC = (";
    res_name += name_of_function;
    res_name += ")";

    fprintf (ns_global_vars::log_file, "TIME = [%s]  %-25s  %-25s  LINE OF CALL = [%0.4d]\n", __TIME__, res_file.c_str(), res_name.c_str(), line);
}

//=============================================================================

//#define NOtxLoadImage(...) ( _txLocCurSet(), NOtxLoadImage (__VA_ARGS__) )

HDC NOtxLoadImage (const char filename[], unsigned imageFlags /*= IMAGE_BITMAP*/, unsigned loadFlags /*= LR_LOADFROMFILE*/, const int x, const int y)
    {
  if (_TX_ARGUMENT_FAILED (filename && *filename)) return NULL;

   HBITMAP image = (HBITMAP) Win32::LoadImage ((loadFlags & LR_LOADFROMFILE)? NULL : GetModuleHandle (NULL),
                                                 filename, imageFlags, x, y, loadFlags);
   if (!image) return NULL;

   HDC dc = txCreateCompatibleDC (0, 0, image);

   if (!(loadFlags & LR_LOADFROMFILE)) return dc;

   static std::map <std::string, unsigned> loadTimes;
   std::string file = filename;
   unsigned time    = GetTickCount();

   if ((long) (time - loadTimes [file]) < _TX_TIMEOUT)
        { txNotifyIcon (NIIF_WARNING, NULL, "Вы загружаете \"%s\" слишком часто, программа будет тормозить.", filename); }

   loadTimes [file] = time;

   return dc;
}


//=============================================================================


#include "auxiliary.h"
#include "classes.h"


#undef PR_LOG
