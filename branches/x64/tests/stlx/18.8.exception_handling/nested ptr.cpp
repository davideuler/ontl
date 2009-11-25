// { dg-options "-std=gnu++0x" }
// { dg-require-atomic-builtins "" }

// Copyright (C) 2009 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <exception>
#include <ntl-tests-common.hxx>

STLX_DEFAULT_TESTGROUP_NAME("std::nested_exception#nested_ptr");


template<> template<> void tut::to::test<01>() 
{
  bool test __attribute__((unused)) = true;

  try
  {
    throw std::nested_exception();
  }
  catch (const std::nested_exception& e)
  {
    VERIFY( e.nested_ptr() == 0 );
  }
}

template<> template<> void tut::to::test<02>() 
{
  bool test __attribute__((unused)) = true;
#ifdef __ICL
  tut::skip("isn't works with icc");
#endif

  try
  {
    throw 42;
  }
  catch (...)
  {
    try
    {
      throw std::nested_exception();
    }
    catch (const std::nested_exception& e)
    {
      try
      {
        std::rethrow_exception( e.nested_ptr() );
      }
      catch(const int& i)
      {
        VERIFY( i == 42 );
      }
    }
  }
}
