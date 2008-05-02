/**\file*********************************************************************
 *                                                                     \brief
 *  18.7 Other runtime support [lib.support.runtime]
 *
 ****************************************************************************
 */

#ifndef NTL__STLX_CSTARG
#define NTL__STLX_CSTARG

namespace std {

/**\defgroup  lib_language_support ***** Language support library [18] ******
 *@{*/
/**\defgroup  lib_support_runtime ****** Types [18.7] ***********************
 *@{*/

#ifndef va_start

typedef struct { } * va_list;

#ifdef _MSC_VER

#if defined(_M_IX86)

//  stack parameters are aligned by 4 bytes

#define va_start(__argptr, __last_param)\
      (__argptr = (va_list)&__last_param + (sizeof(__last_param) + 3 & ~3))

#define va_arg(__argptr, __type)(*(__type*) __argptr += __type + 3 & ~3)

#ifdef _DEBUG
#define va_end(__argptr)(__argptr = (va_list)0)
#else
#define va_end(__argptr)()
#endif

#else//CPU type
#error unsupported CPU type
#endif

#endif//!_MSC_VER

#else//! va_start
#error define va_start & Co
#endif

/**@} lib_support_runtime */
/**@} lib_language_support */

}//namespace std

#endif//#ifndef NTL__STLX_CSTARG