/**\file*********************************************************************
*                                                                     \brief
*  NT Types support library
*
****************************************************************************
*/

#ifndef NTL__NT_BASEDEF
#define NTL__NT_BASEDEF

#include "status.hxx"
#include "../basedef.hxx"
#include "../stdlib.hxx"
#include "string.hxx"
#include "../pe/image.hxx"

namespace ntl {

  /// Exception filter return values
  enum exception_filter
  {
    exception_continue_execution = -1,
    exception_continue_search,
    exception_execute_handler
  };


  namespace nt {

    /**\addtogroup  native_types_support *** NT Types support library ***********
    *@{*/


    struct list_entry
    {
      union { list_entry * Flink; list_entry * next; };
      union { list_entry * Blink; list_entry * prev; };

      void link(list_entry * prev, list_entry * next)
      {
        this->prev = prev; this->next = next;
        prev->next = this; next->prev = this;
      }

      void unlink()
      {
        next->prev = prev;
        prev->next = next;
      }
    };


    struct list_head : public list_entry
    {
      list_head()
      {
        next = this;
        prev = this;
      }

      bool empty() const { return next == this; }

      void insert(list_entry * position, list_entry * entry)
      {
        entry->link(position, position->next);
      }

      list_entry * erase(list_entry * position)
      {
        list_entry * const next = position->next;
        position->unlink();
        return next;
      }

      void insert_tail(list_entry * entry)
      {
        entry->link(prev, this);
      }

      list_entry * begin() { return next; }
      list_entry * end()   { return this; }

    };


    struct single_list_entry
    {
      single_list_entry * Next;

      single_list_entry * next() const { return Next; }
      void next(single_list_entry * p)  { Next = p; }
    };

    typedef single_list_entry slist_entry;

    __align(8)
    struct slist_header : public slist_entry
    {
      uint16_t    Depth;
      uint16_t    Sequence;
    };


    /// Masks for the predefined standard access types
    enum access_mask
    {
      no_access = 0,
      delete_access             = 0x00010000L,
      read_control              = 0x00020000L,
      write_dac                 = 0x00040000L,
      write_owner               = 0x00080000L,
      synchronize               = 0x00100000L,
      standard_rights_required  = delete_access | read_control | write_dac | write_owner,
      standard_rights_read      = read_control,
      standard_rights_write     = read_control,
      standard_rights_execute   = read_control,
      standard_rights_all       = standard_rights_required | synchronize,
      specific_rights_all       = 0x0000FFFFL,
      access_system_security    = 0x01000000L,
      maximum_allowed           = 0x02000000L,
      generic_read              = 0x80000000L,
      generic_write             = 0x40000000L,
      generic_execute           = 0x20000000L,
      generic_all               = 0x10000000L
    };

    static inline
      access_mask  operator | (access_mask m, access_mask m2)
    {
      return bitwise_or(m, m2);
    }

    static inline
      access_mask  operator & (access_mask m, access_mask m2)
    {
      return bitwise_and(m, m2);
    }


    struct generic_mapping
    {
      access_mask generic_read;
      access_mask generic_write;
      access_mask generic_execute;
      access_mask generic_all;
    };


    struct io_status_block
    {
      union { ntstatus  Status; void * Pointer; };
      uintptr_t Information;
    };


    typedef
      void __stdcall
      io_apc_routine(
      const void *            ApcContext,
      const io_status_block * IoStatusBlock,
      uint32_t                Reserved
      );


    /**@} native_types_support */

    enum system_power_state
    {
      PowerSystemUnspecified,
      PowerSystemWorking,
      PowerSystemSleeping1,
      PowerSystemSleeping2,
      PowerSystemSleeping3,
      PowerSystemHibernate,
      PowerSystemShutdown,
      PowerSystemMaximum
    };


    enum power_action
    {
      PowerActionNone,
      PowerActionReserved,
      PowerActionSleep,
      PowerActionHibernate,
      PowerActionShutdown,
      PowerActionShutdownReset,
      PowerActionShutdownOff,
      PowerActionWarmEject
    };

    enum device_power_state
    {
      PowerDeviceUnspecified,
      PowerDeviceD0,
      PowerDeviceD1,
      PowerDeviceD2,
      PowerDeviceD3,
      PowerDeviceMaximum
    };


    struct ldr_data_table_entry
    {
      /* 0x00 */ list_head            InLoadOrderLinks;
      /* 0x08 */ list_head            InMemoryOrderLinks;
      /* 0x10 */ list_head            InInitializationOrderLinks;
      /* 0x18 */ pe::image *          DllBase;
      /* 0x1c */ void *               EntryPoint;
      /* 0x20 */ uint32_t             SizeOfImage;
      /* 0x24 */ const_unicode_string FullDllName;
      /* 0x2c */ const_unicode_string BaseDllName;
      /* 0x34 */ uint32_t             Flags;
      /* 0x38 */ uint16_t             LoadCount;
      /* 0x3a */ uint16_t             TlsIndex;
      //  /* 0x3c */ list_entry           HashLinks;
      /* 0x3c */ void *               SectionPointer;
      /* 0x40 */ uint32_t             CheckSum;
      union {
        /* 0x44 */ uint32_t             TimeDateStamp;
        /* 0x44 */ void *               LoadedImports;
      };
      /* 0x48 */ void *               EntryPointActivationContext;
      /* 0x4c */ void *               PatchInformation;

#if 0
      // nt 6.0+
      /*<thisrel this+0x50>*/ /*|0x8|*/ list_entry ForwarderLinks;
      /*<thisrel this+0x58>*/ /*|0x8|*/ list_entry ServiceTagLinks;
      /*<thisrel this+0x60>*/ /*|0x8|*/ list_entry StaticLinks;
      // // <size 0x68>
#endif


      struct find_dll
      {
        find_dll(list_head * head) : head(head) {}

        list_head * head;

        const pe::image * operator()(const char name[]) const
        {
          if (head)
            for (list_entry * it = head->begin(); it != head->end(); it = it->next)
            {
              const ldr_data_table_entry * const entry =
                reinterpret_cast<ldr_data_table_entry *>(it);
              if(!entry->BaseDllName.size())
                continue;
              for (uint16_t i = 0; i != entry->BaseDllName.size(); ++i) {
                if ((entry->BaseDllName[i] ^ name[i]) & 0x5F)
                  goto other_name;
              }
              return entry->DllBase;
other_name:;
            }
            return 0;
        }
      };

    };
    STATIC_ASSERT(sizeof(ldr_data_table_entry) == 0x50 || sizeof(ldr_data_table_entry) == 0x98);

    typedef const struct _opaque { } * legacy_handle;

    enum SectionInherit
    {
      ViewShare = 1,
      ViewUnmap = 2
    };

    enum allocation_attributes
    {
      mem_commit            = 0x1000,
      mem_reserve           = 0x2000,
      mem_decommit          = 0x4000,
      mem_release           = 0x8000,
      mem_free             = 0x10000,
      mem_private          = 0x20000,
      mem_mapped           = 0x40000,
      mem_reset            = 0x80000,
      mem_top_down        = 0x100000,
      mem_image          = 0x1000000,
      sec_reserve        = 0x4000000,
      mem_large_pages   = 0x20000000,
      mem_4mb_pages     = 0x80000000,
      sec_large_pages   = 0x80000000
    };

    enum page_protection
    {
      page_noaccess           = 0x01,
      page_readonly           = 0x02,
      page_readwrite          = 0x04,
      page_writecopy          = 0x08,
      page_execute            = 0x10,
      page_execute_read       = 0x20,
      page_execute_readwrite  = 0x40,
      page_execute_writecopy  = 0x80,
      page_guard             = 0x100,
      page_nocache           = 0x200,
      page_writecombine      = 0x400
    };

    enum memory_information_class
    {
      MemoryBasicInformation
    };

    struct memory_basic_information
    {
      void*     BaseAddress;
      void*     AllocationBase;
      uint32_t  AllocationProtect;
      size_t    RegionSize;
      uint32_t  State;
      uint32_t  Protect;
      uint32_t  Type;
    };

NTL__EXTERNAPI
ntstatus __stdcall
  NtAllocateVirtualMemory(
    legacy_handle ProcessHandle,
    void**        BaseAddress,
    uint32_t      ZeroBits,
    size_t*       RegionSize,
    uint32_t      AllocationType,
    uint32_t      Protect
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtFreeVirtualMemory(
    legacy_handle ProcessHandle,
    void*         BaseAddress,
    uint32_t*     RegionSize,
    uint32_t      FreeType
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtFlushVirtualMemory(
    legacy_handle ProcessHandle,
    void**        BaseAddress,
    size_t*       RegionSize,
    io_status_block* IoStatus
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtLockVirtualMemory(
    legacy_handle ProcessHandle,
    void**        BaseAddress,
    size_t*       LockSize,
    uint32_t      LockOption
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtProtectVirtualMemory(
    legacy_handle ProcessHandle,
    void**        BaseAddress,
    size_t*       ProtectSize,
    uint32_t      NewAccessProtection,
    uint32_t*     OldAccessProtection
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtQueryVirtualMemory(
    legacy_handle ProcessHandle,
    void*         BaseAddress,
    memory_information_class MemoryInformationClass,
    void*         Buffer,
    uint32_t      Length,
    uint32_t*     ResultLength
    );

NTL__EXTERNAPI
ntstatus __stdcall
  NtUnlockVirtualMemory(
    legacy_handle ProcessHandle,
    void**        BaseAddress,
    size_t*       LockSize,
    uint32_t      LockType
    );

  }//namespace nt
}//namespace ntl

#endif//#ifndef NTL__NT_BASEDEF