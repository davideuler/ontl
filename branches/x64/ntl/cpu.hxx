/**\file*********************************************************************
 *                                                                     \brief
 *  CPU specific stuff
 *
 ****************************************************************************
 */

#ifndef NTL__CPU
#define NTL__CPU


namespace ntl {
namespace cpu {

#ifdef _MSC_VER
  #ifdef _M_IX86

    static inline void pause() { __asm { pause } }

  #else // ! _M_IX86
    namespace intrinsic {
      extern "C" void __cdecl _mm_pause();
      #pragma intrinsic(_mm_pause)
    }
    static inline void pause() { intrinsic::_mm_pause(); }
  #endif
#else // ! _MSC_VER
#error unsupported compiler
#endif

}//namespace cpu
}//namespace ntl

#endif//#ifndef NTL__CPU
