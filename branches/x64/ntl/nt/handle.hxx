/**\file*********************************************************************
 *                                                                     \brief
 *  NT Handles support
 *
 ****************************************************************************
 */


#ifndef NTL__NT_HANDLE
#define NTL__NT_HANDLE

#include "../handle.hxx"
#include "../stdlib.hxx"
#include "basedef.hxx"

namespace ntl {
namespace nt {


//typedef const struct _opaque { } * legacy_handle;

static const legacy_handle null_handle;

static inline legacy_handle current_process()
{
  const legacy_handle current_process = legacy_handle(-1);
  return current_process;
}


NTL__EXTERNAPI
ntstatus __stdcall
NtDuplicateObject(
    legacy_handle   SourceProcessHandle,
    legacy_handle   SourceHandle,
    legacy_handle   TargetProcessHandle   __optional,
    legacy_handle * TargetHandle          __optional,
    access_mask     DesiredAccess,
    uint32_t        HandleAttributes,
    uint32_t        Options
    );


enum duplicate_options 
{
  duplicate_close_source    = 1,
  duplicate_same_access     = 2,
  duplicate_same_attributes = 4
};

static __forceinline
  duplicate_options operator|(duplicate_options m, duplicate_options m2) 
{ 
  return bitwise_or(m, m2);
}


enum handle_attributes
{
  handle_flag_none                = 0,
  handle_flag_inherit             = 1,
  handle_flag_protect_from_close  = 2,
};

static __forceinline
  handle_attributes operator|(handle_attributes m, handle_attributes m2) 
{ 
  return bitwise_or(m, m2);
}


static __forceinline
legacy_handle
  duplicate(
    legacy_handle     source_handle,
    legacy_handle     source_process  = current_process(),
    legacy_handle     target_process  = current_process(),
    access_mask       desired_access  = nt::no_access,
    handle_attributes attributes      = handle_flag_none,
    duplicate_options options         = duplicate_same_access | duplicate_same_attributes
    )
{
  legacy_handle h = { 0 };
  // returns valid handle or 0
  NtDuplicateObject(source_process, source_handle, target_process, &h,
                    desired_access, attributes, options);
  return h;
}


NTL__EXTERNAPI
ntstatus __stdcall
  NtClose(legacy_handle Handle);

static __forceinline
ntstatus close(legacy_handle handle)
{ 
  return NtClose(handle);
}


namespace aux {

__forceinline
void close(legacy_handle handle)
{ 
  nt::close(handle);
}

__forceinline
legacy_handle duplicate(legacy_handle handle)
{ 
  return nt::duplicate(handle);
}

} //namespace aux

typedef basic_handle<legacy_handle, aux::close, aux::duplicate> handle;


}//namespace nt
}//namespace ntl

#endif//#ifndef NTL__NT_HANDLE