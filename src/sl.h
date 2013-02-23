#ifndef SL_H
#define SL_H

#include "sl_error.h"
#include <snlsys/snlsys.h>

#ifndef NDEBUG
  #define SL(func) ASSERT(SL_NO_ERROR == sl_##func)
#else
  #define SL(func) sl_##func
#endif

#if defined(SL_SHARED_BUILD)
  #define SL_API EXPORT_SYM
#else 
  #define SL_API IMPORT_SYM
#endif

#endif /* SL_H */

