// common tests part
#include <cassert>

#define __attribute__(x)
#pragma warning(disable:4101 4189)
#define VERIFY(e) assert(e)

#include <memory>
#include <nt/new.hxx>

template class std::unique_ptr<int>;
template class std::unique_ptr<int[]>;

namespace uniqueptr_test {

  struct base { virtual ~base() {} };
  struct derived : base {};

#ifdef NTL__CXX
  void
    test01()
  {
    std::unique_ptr<derived> p1(new derived);
    std::unique_ptr<derived> p2(new derived);
      p2 = p1;  // should not compile
    p2 = std::move(p1);
    std::unique_ptr<base> p3(new base);
    //  p3 = p2;  // should not compile
    p3 = std::move(p2);
  }
#endif

  void
    test02()
  {
    std::unique_ptr<int[]> p1(new int(420));
    std::unique_ptr<int[]> p2 = p1; // { dg-error "within this context" }
  }

  void
    test03()
  {
    std::unique_ptr<int[2]> p1(new int[3]);
    std::unique_ptr<int[2]> p2 = p1; // { dg-error "within this context" }
  }
  // { dg-excess-errors "is private" }

  struct B0 { virtual ~B0() {} };
  struct D : public B0 {};

#ifdef NTL__CXX
  void
    test04()
  {
    bool test __attribute__((unused)) = true;

    D *d = new D;
    std::unique_ptr<D> p1(d);
    std::unique_ptr<D> p2(new D);
    p2 = std::move(p1);

    VERIFY( p1.get() == 0 );
    VERIFY( p2.get() == d );

    std::unique_ptr<B0> p3(new B0);
    p3 = std::move(p2);

    VERIFY( p2.get() == 0 );
    VERIFY( p3.get() == d );
  }


  void
    test05()
  {
    bool test __attribute__((unused)) = true;

    D *d = new D[3];
    std::unique_ptr<D[]> p1(d);
    std::unique_ptr<D[]> p2;
    p2 = std::move(p1);

    VERIFY( p1.get() == 0 );
    VERIFY( p2.get() == d );
  }
#endif
  struct A
  {
    A() { ++ctor_count; }
    virtual ~A() { ++dtor_count; }
    static long ctor_count;
    static long dtor_count;
  };
  long A::ctor_count = 0;
  long A::dtor_count = 0;

  struct B : A
  {
    B() { ++ctor_count; }
    virtual ~B() { ++dtor_count; }
    static long ctor_count;
    static long dtor_count;
  };
  long B::ctor_count = 0;
  long B::dtor_count = 0;


  struct reset_count_struct
  {
    ~reset_count_struct()
    {
      A::ctor_count = 0;
      A::dtor_count = 0;
      B::ctor_count = 0;
      B::dtor_count = 0;
    }
  };

  // 20.6.11.2.1 unique_ptr constructors [unique.ptr.single.ctor]

  // Construction from pointer
  void
    test06()
  {
    reset_count_struct __attribute__((unused)) reset;
    bool test __attribute__((unused)) = true;

    std::unique_ptr<A> A_default;
    VERIFY( A_default.get() == 0 );
    VERIFY( A::ctor_count == 0 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );

    std::unique_ptr<A> A_from_A(new A);
    VERIFY( A_from_A.get() != 0 );
    VERIFY( A::ctor_count == 1 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );

    std::unique_ptr<A> A_from_B(new B);
    VERIFY( A_from_B.get() != 0 );
    VERIFY( A::ctor_count == 2 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 1 );
    VERIFY( B::dtor_count == 0 );
  }

  void
    test07()
  {
    reset_count_struct __attribute__((unused)) reset;
    bool test __attribute__((unused)) = true;

    A * const A_default = 0;
    std::unique_ptr<A> p1(A_default);
    VERIFY( p1.get() == 0 );
    VERIFY( A::ctor_count == 0 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );

    A * const A_from_A = new A;
    std::unique_ptr<A> p2(A_from_A);
    VERIFY( p2.get() == A_from_A );
    VERIFY( A::ctor_count == 1 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );
  }

  // 20.4.5.1 unique_ptr constructors [unique.ptr.cons]

  // Construction from pointer
  void
    test08()
  {
    reset_count_struct __attribute__((unused)) reset;
    bool test __attribute__((unused)) = true;

    std::unique_ptr<A[]> A_default;
    VERIFY( A_default.get() == 0 );
    VERIFY( A::ctor_count == 0 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );

    std::unique_ptr<A[]> A_from_A(new A[3]);
    VERIFY( A_from_A.get() != 0 );
    VERIFY( A::ctor_count == 3 );
    VERIFY( A::dtor_count == 0 );
    VERIFY( B::ctor_count == 0 );
    VERIFY( B::dtor_count == 0 );
  }

  namespace a2 {
  struct A
  {
  };

  struct B : A
  {
    virtual ~B() { }
  };

  }
  // 20.4.5.1 unique_ptr constructors [unique.ptr.cons]

  // Construction from pointer of derived type
  void
    test09()
  {
    //std::unique_ptr<a2::B[]> B_from_A(new a2::A[3]); //{ dg-error "invalid conversion from" }
  }

  namespace a3 {
  struct A
  {
    virtual ~A() { }
  };

  struct B : A
  {
  };
  }

  // 20.6.11.5 unqiue_ptr specialized algorithms [unique.ptr.special]

  void
    test10()
  {
    bool test __attribute__((unused)) = true;

    std::unique_ptr<a3::A> p1;
    std::unique_ptr<a3::A> p2;

    VERIFY( p1 == p2 );
    VERIFY( !(p1 != p2) );
    VERIFY( !(p1 < p2) && !(p1 > p2) );
  }

  void
    test11()
  {
    bool test __attribute__((unused)) = true;

    std::unique_ptr<a3::A> p1;
    std::unique_ptr<a3::A> p2(new a3::A);

    VERIFY( p1 != p2 );
    VERIFY( !(p1 == p2) );
    VERIFY( (p1 < p2) || (p1 > p2) );
    VERIFY( ((p1 <= p2) && (p1 != p2)) || ((p1 >= p2) && (p1 != p2)) );
  }

  // 20.6.11.5 unqiue_ptr specialized algorithms [unique.ptr.special]

  void
    test12()
  {
    bool test __attribute__((unused)) = true;

    std::unique_ptr<a3::A[]> p1;
    std::unique_ptr<a3::A[]> p2;

    VERIFY( p1 == p2 );
    VERIFY( !(p1 != p2) );
    VERIFY( !(p1 < p2) && !(p1 > p2) );
  }

  void
    test13()
  {
    bool test __attribute__((unused)) = true;

    std::unique_ptr<a3::A[]> p1;
    std::unique_ptr<a3::A[]> p2(new a3::A[3]);

    VERIFY( p1 != p2 );
    VERIFY( !(p1 == p2) );
    VERIFY( (p1 < p2) || (p1 > p2) );
    VERIFY( ((p1 <= p2) && (p1 != p2)) || ((p1 >= p2) && (p1 != p2)) );
  }

  void
    test14()
  {
    bool test __attribute__((unused)) = true;

    std::unique_ptr<a3::A> p1;
    std::unique_ptr<a3::A> p2(new a3::A);
    std::unique_ptr<a3::A> p3;

    std::swap(p3, p2);

    VERIFY( p1 != p3 );
    VERIFY( p2 != p3 );
    VERIFY( p1 == p2 );
  }

  void main()
  {
#ifdef NTL__CXX
    test01();
#endif
    test02();
    test03();
#ifdef NTL__CXX
    test04();
    test05();
#endif
    test06();
    test07();
    test08();
    test09();
    test10();
    test11();
    test12();
    test13();
    test14();
  }
}