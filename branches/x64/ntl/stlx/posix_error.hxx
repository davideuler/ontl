/**\file*********************************************************************
 *                                                                     \brief
 *  19.4 System error support [syserr]
 *
 ****************************************************************************
 */

#ifndef NTL__STLX_SYSTEM_ERROR
#error don't use this header, it included via <system_error>
#endif


namespace std
{
  /**
   *	@addtogroup syserr 19.4 System error support [syserr]
   *  @{
   **/

  namespace posix_error 
  {
    enum posix_errno {
      address_family_not_supported, // EAFNOSUPPORT
      address_in_use, // EADDRINUSE
      address_not_available, // EADDRNOTAVAIL
      already_connected, // EISCONN
      argument_list_too_long, // E2BIG
      argument_out_of_domain, // EDOM
      bad_address, // EFAULT
      bad_file_descriptor, // EBADF
      bad_message, // EBADMSG
      broken_pipe, // EPIPE
      connection_aborted, // ECONNABORTED
      connection_already_in_progress, // EALREADY
      connection_refused, // ECONNREFUSED
      connection_reset, // ECONNRESET
      cross_device_link, // EXDEV
      destination_address_required, // EDESTADDRREQ
      device_or_resource_busy, // EBUSY
      directory_not_empty, // ENOTEMPTY
      executable_format_error, // ENOEXEC
      file_exists, // EEXIST
      file_too_large, // EFBIG
      filename_too_long, // ENAMETOOLONG
      function_not_supported, // ENOSYS
      host_unreachable, // EHOSTUNREACH
      identifier_removed, // EIDRM
      illegal_byte_sequence, // EILSEQ
      inappropriate_io_control_operation, // ENOTTY
      interrupted, // EINTR
      invalid_argument, // EINVAL
      invalid_seek, // ESPIPE
      io_error, // EIO
      is_a_directory, // EISDIR
      message_size, // EMSGSIZE
      network_down, // ENETDOWN
      network_reset, // ENETRESET
      network_unreachable, // ENETUNREACH
      no_buffer_space, // ENOBUFS
      no_child_process, // ECHILD
      no_link, // ENOLINK
      no_lock_available, // ENOLCK
      no_message_available, // ENODATA
      no_message, // ENOMSG
      no_protocol_option, // ENOPROTOOPT
      no_space_on_device, // ENOSPC
      no_stream_resources, // ENOSR
      no_such_device_or_address, // ENXIO
      no_such_device, // ENODEV
      no_such_file_or_directory, // ENOENT
      no_such_process, // ESRCH
      not_a_directory, // ENOTDIR
      not_a_socket, // ENOTSOCK
      not_a_stream, // ENOSTR
      not_connected, // ENOTCONN
      not_enough_memory, // ENOMEM
      not_supported, // ENOTSUP
      operation_canceled, // ECANCELED
      operation_in_progress, // EINPROGRESS
      operation_not_permitted, // EPERM
      operation_not_supported, // EOPNOTSUPP
      operation_would_block, // EWOULDBLOCK
      owner_dead, // EOWNERDEAD
      permission_denied, // EACCES
      protocol_error, // EPROTO
      protocol_not_supported, // EPROTONOSUPPORT
      read_only_file_system, // EROFS
      resource_deadlock_would_occur, // EDEADLK
      resource_unavailable_try_again, // EAGAIN
      result_out_of_range, // ERANGE
      state_not_recoverable, // ENOTRECOVERABLE
      stream_timeout, // ETIME
      text_file_busy, // ETXTBSY
      timed_out, // ETIMEDOUT
      too_many_files_open_in_system, // ENFILE
      too_many_files_open, // EMFILE
      too_many_links, // EMLINK
      too_many_symbolic_link_levels, // ELOOP
      value_too_large, // EOVERFLOW
      wrong_protocol_type, // EPROTOTYPE
    };
  } // namespace posix_error

  template <> struct is_error_condition_enum<posix_error::posix_errno>
  : public true_type {};


  /// 19.4.2.6 Class error_code non-member functions [syserr.errcode.nonmembers]
  inline error_code make_error_code(posix_error::posix_errno e)
  {
    return error_code(static_cast<int>(e), generic_category);
  }

  inline error_condition make_error_condition(posix_error::posix_errno e)
  {
    return error_condition(static_cast<int>(e), generic_category);
  }

  /** @} syserr */
}
