
// main_header.h

#include "config.h"

//=============================================================================

#define PR_LOG \
Print_Log (__FILE__, __func__, __LINE__);

//=============================================================================

template<typename T, typename V>
inline void Clamp (T& var, const V& down, const V& up)
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

    fprintf (ns_global_vars::log_file, "TIME = [%s]  %-20s  %-20s  LINE OF CALL = [%0.4d]\n", __TIME__, res_file.c_str(), res_name.c_str(), line);
}

//=============================================================================

#include "auxiliary.h"
#include "classes.h"
