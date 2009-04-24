/**\file*********************************************************************
 *                                                                     \brief
 *  Class template stack [23.2.5.3 lib.stack]
 *
 ****************************************************************************
 */

#ifndef STLX_STACK
#define STLX_STACK

#if 0//def - what macro???
#include "deque.hxx"
#define STLX__STACK_CONTAINER deque<T>  // standard behavior
#else
#include "vector.hxx"
#define STLX__STACK_CONTAINER vector<T> // faster
#endif

namespace stlx {

/**\addtogroup  lib_containers ********* 23 Containers library [containers] ************
 *@{*/

/**\addtogroup  lib_sequence *********** 23.2 Sequence containers [sequences] *******************
 *@{*/

/**\defgroup  lib_container_adaptors *** 23.2.5 Container adaptors [container.adaptors] *********
 *@{*/

/// Class template stack 23.2.5.3
template <class T, class Container = STLX__STACK_CONTAINER>
class stack
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    typedef typename Container::value_type  value_type;
    typedef typename Container::size_type   size_type;
    typedef          Container              container_type;

    explicit stack(const Container& c = Container()) : c(c) {}
    
    #ifdef NTL__CXX_RV
    explicit stack(Container&& = Container());
    #endif

    template <class Alloc>
    explicit stack(const Alloc&);
    template <class Alloc>
    stack(const Container&, const Alloc&);
    
    #ifdef NTL__CXX_RV
    template <class Alloc>
    stack(Container&&, const Alloc&);
    template <class Alloc>
    stack(stack&&, const Alloc&);
    #endif

    bool              empty() const             { return c.empty(); }
    size_type         size()  const             { return c.size(); }
    value_type&       top()                     { return c.back(); }
    const value_type& top()   const             { return c.back(); }
    void              push(const value_type& x) { c.push_back(x); }
    void              pop()                     { c.pop_back(); }

    #ifdef NTL__CXX_RV
    void push(value_type&& x);
    #endif
    #ifdef NTL__CXX_VT
    template <class... Args>
    void emplace(Args&&... args);
    #endif

    #ifdef NTL__CXX_RV
    void swap(stack&& s) { c.swap(s.c); }
    #else
    void swap(stack&  s) { c.swap(s.c); }
    #endif


  ///////////////////////////////////////////////////////////////////////////
  protected:

    Container c;

  ///////////////////////////////////////////////////////////////////////////

  ///\name  Stack operators
  ///@{

  friend
    bool operator==(const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c == y.c; }

  friend
    bool operator< (const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c < y.c; }

  friend
    bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c != y.c }

  friend
    bool operator> (const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c > y.c; }

  friend
    bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c >= y.c; }

  friend
    bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y)
      { return x.c <= y.c; }

  ///@}

};//class stack

// misprint?
template <class T, class Allocator>
void swap(stack<T,Allocator>& x, stack<T,Allocator>& y);

template <class T, class Container>
void swap(stack<T, Container>& x, stack<T, Container>& y)
{
  x.swap(y);
}

#ifdef NTL__CXX_RV
template <class T, class Allocator>
void swap(stack<T,Allocator>&& x, stack<T,Allocator>& y);
template <class T, class Allocator>
void swap(stack<T,Allocator>& x, stack<T,Allocator>&& y);
#endif


template <class T, class Container, class Alloc>
struct uses_allocator<stack<T, Container>, Alloc>
  : uses_allocator<Container, Alloc>::type { };

template <class T, class Container>
struct constructible_with_allocator_suffix<stack<T, Container> >
  : true_type { };


/**@} lib_container_adaptors */

/**@} lib_sequence */

/**@} lib_containers */

}//namespace stlx

#endif//#ifndef STLX_STACK