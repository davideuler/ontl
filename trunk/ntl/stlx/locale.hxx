/**\file*********************************************************************
 *                                                                     \brief
 *  N2315 22 Localization library [localization]
 *
 ****************************************************************************
 */

#ifdef STLX__CONFORMING_LOCALE 
  #if STLX__CONFORMING_LOCALE
  #error no conforming locales support yet
  #endif
#else
#define STLX__CONFORMING_LOCALE 0
#endif

#ifndef NTL__STLX_LOCALE
#define NTL__STLX_LOCALE

#if STLX__CONFORMING_LOCALE
#define _NTL_LOC_VIRTUAL virtual
#else
#define _NTL_LOC_VIRTUAL
#endif

#include "string.hxx"
#include "stdexcept.hxx"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4820)// X bytes padding added... (class ctype<char>)
#endif

namespace std {

// 22.1 Locales [locales]

/// 22.1.1 Class locale [locale]
class locale
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    ///\name 22.1.1.1 locale types [locale.types]

    /// 22.1.1.1.1 Type locale::category [locale.category]
    typedef int category;
    static const category
      none = 0,   collate = 0x01,   ctype = 0x02,
      monetary = 0x04,  numeric = 0x08,   time = 0x10, messages = 0x20,
      all = collate | ctype | monetary | numeric | time | messages;

    /// 22.1.1.1.2 Class locale::facet [locale.facet]
    class facet
    {
      protected:
        explicit facet(size_t /*refs*/ = 0) {}
        _NTL_LOC_VIRTUAL ~facet() {}
      private:
        facet(const facet&); // not defined
        void operator=(const facet&); // not defined
    };

    /// 22.1.1.1.3 Class locale::id [locale.id]
    class id
    {
      public:
        id();
      private:
        void operator=(const id&); // not defined
        id(const id&); // not defined
    };

    ///\name 22.1.1.2 locale constructors and destructor [locale.cons]

    locale() __ntl_nothrow {/**/}
    locale(const locale& /*other*/) __ntl_nothrow {/**/}
    const locale& operator=(const locale& other) __ntl_nothrow;
    explicit locale(const char* std_name) __ntl_throws(runtime_error);
    locale(const locale& other, const char* std_name, category) __ntl_throws(runtime_error);
    template <class Facet> locale(const locale& other, Facet* f);
    locale(const locale& other, const locale& one, category);

    ~locale() __ntl_nothrow {} // non-virtual

    ///\name 22.1.1.3 locale members [locale.members]

    template <class Facet> locale combine(const locale& other) const;
    basic_string<char> name() const;

    ///\name 22.1.1.4 locale operators [locale.operators]

    bool operator==(const locale& other) const;
    bool operator!=(const locale& other) const;
    
    template <class charT, class Traits, class Allocator>
    bool operator()(const basic_string<charT, Traits, Allocator>& s1,
                    const basic_string<charT, Traits, Allocator>& s2) const
    {
      return use_facet<collate<charT> >(*this).compare
          (s1.data(), s1.data()+s1.size(), s2.data(), s2.data()+s2.size()) < 0;
    }

    ///\name 22.1.1.5 locale static members [locale.statics]

    static locale global(const locale&);
    static const locale& classic();

};// class locale

///\name 22.1.2 locale globals [locale.global.templates]

template <class Facet> const Facet& use_facet(const locale&);
template <class Facet> bool has_facet(const locale&) __ntl_nothrow;

///\name 22.1.3 Convenience interfaces [locale.convenience]

///\name 22.1.3.1 Character classification [classification]

template <class charT> bool isspace(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::space, c);
}

template <class charT> bool isprint(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::print, c);
}

template <class charT> bool iscntrl(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::cntrl, c);
}

template <class charT> bool isupper(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::upper, c);
}

template <class charT> bool islower(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::lower, c);
}

template <class charT> bool isalpha(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::alpha, c);
}

template <class charT> bool isdigit(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::digit, c);
}

template <class charT> bool ispunct(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::punct, c);
}

template <class charT> bool isxdigit(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::xdigit, c);
}

template <class charT> bool isalnum(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::alnum, c);
}

template <class charT> bool isgraph(charT c, const locale& loc)
{
  use_facet<ctype<charT> >(loc).is(ctype_base::graph, c);
}

// 22.1.3.2 Character conversions [conversions]

template <class charT>
charT
  toupper(charT c, const locale& loc)
{
  return use_facet<ctype<charT> >(loc).toupper(c);
}

template <class charT>
charT
  tolower(charT c, const locale& loc)
{
  return use_facet<ctype<charT> >(loc).tolower(c);
}

///\name 22.2 Standard locale categories [locale.categories]


// ensure GetStringTypeX API is usable
#if  defined C1_UPPER && defined C1_LOWER && defined C1_DIGIT \
  && defined C1_SPACE && defined C1_PUNCT && defined C1_CNTRL \
  && defined C1_BLANK && defined C1_XDIGIT \
  && defined C1_ALPHA && C1_DEFINED
static const int16_t __ct_upper = (int16_t)C1_UPPER;
static const int16_t __ct_lower = (int16_t)C1_LOWER;
static const int16_t __ct_digit = (int16_t)C1_DIGIT;
static const int16_t __ct_space = (int16_t)C1_SPACE;
static const int16_t __ct_punct = (int16_t)C1_PUNCT;
static const int16_t __ct_cntrl = (int16_t)C1_CNTRL;
static const int16_t __ct_blank = (int16_t)C1_BLANK;
static const int16_t __ct_xdigit = (int16_t)C1_XDIGIT;
static const int16_t __ct_alpha = (int16_t)C1_ALPHA;
#else
static const int16_t __ct_upper = 0x0001;
static const int16_t __ct_lower = 0x0002;
static const int16_t __ct_digit = 0x0004;
static const int16_t __ct_space = 0x0008;
static const int16_t __ct_punct = 0x0010;
static const int16_t __ct_cntrl = 0x0020;
static const int16_t __ct_blank = 0x0040;
static const int16_t __ct_xdigit = 0x0080;
static const int16_t __ct_alpha = 0x0100;
#endif

/// 22.2.1 The ctype category [category.ctype]
class ctype_base
{
  public:
    typedef int16_t mask;
    static const mask
      lower = __ct_lower,
      upper = __ct_upper,
      alpha = lower|upper|__ct_alpha,
      cntrl = __ct_cntrl,
      digit = __ct_digit,
      xdigit = __ct_xdigit,
      space = __ct_space|__ct_blank,
      punct = __ct_punct,
      print = alpha|digit|xdigit|punct|__ct_blank,// __ct_space?
      alnum = alpha|digit,
      graph = alnum|punct;
};

/// 22.2.1.1 Class template ctype [locale.ctype]
template <class charT>
class ctype : public locale::facet, public ctype_base
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    typedef charT char_type;

    explicit ctype(size_t refs = 0);

    ///\name 22.2.1.1.1 ctype members [locale.ctype.members]

    bool is(mask m, charT c) const { return do_is(m, c); }

    const charT* is(const charT* low, const charT* high, mask* vec) const
    {
      return do_is(low, high, vec);
    }

    const charT* scan_is(mask m, const charT* low, const charT* high) const
    {
      return do_scan_is(m, low, high);
    }

    const charT* scan_not(mask m, const charT* low, const charT* high) const
    {
      return do_scan_not(m, low, high);
    }

    charT toupper(charT c) const { return do_toupper(c); }

    const charT* toupper(charT* low, const charT* high) const
    {
      return do_toupper(low, high);
    }

    charT tolower(charT c) const { return do_tolower(c); }

    const charT* tolower(charT* low, const charT* high) const
    {
      return do_tolower(low, high);
    }

    charT widen(char c) const { return do_widen(c); }

    const char* widen(const char* low, const char* high, charT* to) const
    {
      return do_widen(low, high, to);
    }

    char narrow(charT c, char dfault) const { return do_narrow(c,dfault); }

    const charT* narrow(const charT* low, const charT*, char dfault, char* to) const
    {
      return do_narrow(low, high, dfault, to);
    }

    static locale::id id;

  ///////////////////////////////////////////////////////////////////////////
  protected:

    ///\name 22.2.1.1.2 ctype virtual functions [locale.ctype.virtuals]

    ~ctype(); //virtual

    virtual bool do_is(mask m, charT c) const;
    virtual const charT* do_is(const charT* low, const charT* high, mask* vec) const;
    virtual const charT* do_scan_is(mask m, const charT* low, const charT* high) const;
    virtual const charT* do_scan_not(mask m, const charT* low, const charT* high) const;
    virtual charT do_toupper(charT) const;
    virtual const charT* do_toupper(charT* low, const charT* high) const;
    virtual charT do_tolower(charT) const;
    virtual const charT* do_tolower(charT* low, const charT* high) const;
    virtual charT do_widen(char) const;
    virtual const char* do_widen(const char* low, const char* high, charT* dest) const;
    virtual char do_narrow(charT, char dfault) const;
    virtual const charT* do_narrow(const charT* low, const charT* high, char dfault, char* dest) const;

};// class ctype

/// 22.2.1.2 Class template ctype_byname [locale.ctype.byname]
template <class charT>
class ctype_byname : public ctype<charT>
{
  public:
    typedef ctype<charT>::mask mask;
    explicit ctype_byname(const char*, size_t refs = 0);
  protected:
    ~ctype_byname(); // virtual
    virtual bool do_is(mask m, charT c) const;
    virtual const charT* do_is(const charT* low, const charT* high, mask* vec) const;
    virtual const charT* do_scan_is(mask m, const charT* low, const charT* high) const;
    virtual const charT* do_scan_not(mask m, const charT* low, const charT* high) const;
    virtual charT do_toupper(charT) const;
    virtual const charT* do_toupper(charT* low, const charT* high) const;
    virtual charT do_tolower(charT) const;
    virtual const charT* do_tolower(charT* low, const charT* high) const;
    virtual charT do_widen(char) const;
    virtual const char* do_widen(const char* low, const char* high, charT* dest) const;
    virtual char do_narrow(charT, char dfault) const;
    virtual const charT* do_narrow(const charT* low, const charT* high, char dfault, char* dest) const;
};

/// 22.2.1.3 ctype specializations [facet.ctype.special]
template <> class ctype<char>
: public locale::facet, public ctype_base
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    template <class Facet> friend const Facet& use_facet(const locale&);
    
    typedef char char_type;

    explicit ctype(const mask* tab = 0, bool del = false, size_t refs = 0)
    : locale::facet(refs), tab(tab ? tab : classic_table()), del(del) {}

    bool is(mask m, char c) const
    {
      return 0 != (table()[static_cast<unsigned char>(c)] & m);
    }

    const char* is(const char* low, const char* high, mask* vec) const
    {
      while ( low != high )
        *vec++ = table()[static_cast<unsigned char>(*low++)];
      return high;
    }

    const char* scan_is(mask m, const char* low, const char* high) const
    {
      while ( low != high && !(table()[static_cast<unsigned char>(*low)] & m) )
          ++low;
      return low; 
    }

    const char* scan_not(mask m, const char* low, const char* high) const
    {
      while ( low != high && (table()[static_cast<unsigned char>(*low)] & m) )
          ++low;
      return low; 
    }

    char toupper(char c) const { return do_toupper(c); }

    const char* toupper(char* low, const char* high) const { return do_toupper(low, high); }
    
    char tolower(char c) const { return do_tolower(c); }

    const char* tolower(char* low, const char* high) const {return do_tolower(low, high); }

    char widen(char c) const { return do_widen(c); }

    const char* widen(const char* low, const char* high, char* to) const { return do_widen(low, high, to); }

    char narrow(char c, char dfault) const { return do_narrow(c, dfault); }

    const char* narrow(const char * low, const char * high, char dfault, char * to) const
    {
      return do_narrow(low, high, dfault, to);
    }

    static locale::id id;

    static const size_t table_size = 1 << CHAR_BIT;

  ///////////////////////////////////////////////////////////////////////////
  protected:

    ~ctype()
    { 
      // tab pointer is allways initialyzed by the constructor
      if ( /*tab &&*/ del )
        delete[] tab;
    }

    const mask* table() const __ntl_nothrow { return tab; }

    static const mask* classic_table() __ntl_nothrow
    {
      return 0;
    }

    ///\name 22.2.1.3.4 ctype<char> virtual functions [facet.ctype.char.virtuals]

    _NTL_LOC_VIRTUAL char do_toupper(char c) const
    { 
      return is(c, lower) ? c + 'A'-'a' : c;
    }

    _NTL_LOC_VIRTUAL const char* do_toupper(char* low, const char* high) const
    {
      while ( low != high )
        *low++ = do_toupper(*low);
      return high;
    }

    _NTL_LOC_VIRTUAL char do_tolower(char c) const
    { 
      return is(c, upper) ? c + 'A'-'a' : c;
    }

    _NTL_LOC_VIRTUAL const char* do_tolower(char* low, const char* high) const
    {
      while ( low != high )
        *low++ = do_tolower(*low);
      return high;
    }

    _NTL_LOC_VIRTUAL char do_widen(char c) const { return c; }

    _NTL_LOC_VIRTUAL const char* do_widen(const char* low, const char* high, char* to) const
    {
      while ( low != high )
        *to++ = *low++;
      return high;
    }

    _NTL_LOC_VIRTUAL char do_narrow(char c, char /*dfault*/) const { return c; }

    _NTL_LOC_VIRTUAL const char* do_narrow(const char * low,
                       const char * high,
                       char         /*dfault*/,
                       char *       to) const
    {
      while ( low != high )
        *to++ = *low++;
      return high;
    }

    ///\}

  private:

    ctype(const ctype&); // not defined
    void operator=(const ctype&); // not defined

    const mask * const  tab;
    const bool          del;

};// class ctype<char>

template<>
__forceinline
const ctype<char>& use_facet<ctype<char> >(const locale&)
{
  // static ctype<char> is constructed with default table which is not to be freed,
  // so the destructor call may be not queued up to the atexit function.
  // This is why it is implemented through placement new over a raw storage.
  static void * f[sizeof(ctype<char>)/sizeof(void*)];
  ctype<char> * const p = reinterpret_cast<ctype<char>*>(f);
  // The first class member is VTable ptr or the tab ptr member,
  // both are non-null after initialization is done.
  if ( f[0] ) new (p) ctype<char>;
  return *p;
}


/// 22.2.1.4 Class ctype_byname<char> [lib.locale.ctype.byname.special]
template <> class ctype_byname<char>
: public ctype<char>
{
  public:
    explicit ctype_byname(const char*, size_t refs = 0);
  protected:
    virtual ~ctype_byname(); // virtual
    virtual char do_toupper(char c) const;
    virtual const char* do_toupper(char* low, const char* high) const;
    virtual char do_tolower(char c) const;
    virtual const char* do_tolower(char* low, const char* high) const;
    virtual char do_widen(char c) const;
    virtual const char* do_widen(const char* low, const char* high, char* to) const;
    virtual char do_narrow(char c, char dfault) const;
    virtual const char* do_narrow(const char* low, const char* high, char dfault, char* to) const;
  private:
    ctype_byname(const ctype_byname&); // not defined
    void operator=(const ctype_byname&); // not defined
};

// 22.2.1.5 Class template codecvt [lib.locale.codecvt]

class codecvt_base { public: enum result { ok, partial, error, noconv }; };

template <class internT, class externT, class stateT>
class codecvt : public locale::facet, public codecvt_base
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    typedef internT intern_type;
    typedef externT extern_type;
    typedef stateT  state_type;

    explicit codecvt(size_t refs = 0);

    result out(      stateT &   state,
               const internT *  from,
               const internT *  from_end,
               const internT *& from_next,
                     externT *  to,
                     externT *  to_limit,
                     externT *& to_next) const;

    result unshift(stateT &   state,
                   externT *  to,
                   externT *  to_limit,
                   externT *& to_next) const;

    result in(      stateT &   state,
              const externT *  from,
              const externT *  from_end,
              const externT *& from_next,
                    internT *  to,
                    internT *  to_limit,
                    internT *& to_next) const;

    int encoding() const __ntl_nothrow;
    bool always_noconv() const __ntl_nothrow;

    int length(stateT &,
               const externT* from,
               const externT* end,
               size_t max) const;

    int max_length() const __ntl_nothrow;

    static locale::id id;

  ///////////////////////////////////////////////////////////////////////////
  protected:

    ~codecvt(); //virtual

    virtual result do_out(stateT& state, const internT* from, const internT* from_end,
      const internT*& from_next, externT* to, externT* to_limit, externT*& to_next) const;

    virtual result do_in(stateT& state, const externT* from, const externT* from_end,
      const externT*& from_next, internT* to, internT* to_limit, internT*& to_next) const;

    virtual result do_unshift(stateT& state, externT* to, externT* to_limit, externT*& to_next) const;

    virtual int do_encoding() const __ntl_nothrow;

    virtual bool do_always_noconv() const __ntl_nothrow;

    virtual int do_length(stateT&, const externT* from, const externT* end, size_t max) const;

    virtual int do_max_length() const __ntl_nothrow;

};// codecvt

/// 22.2.1.6 Class template codecvt_byname [lib.locale.codecvt.byname]
template <class internT, class externT, class stateT>
class codecvt_byname : public codecvt<internT, externT, stateT>
{
  public:
    explicit codecvt_byname(const char*, size_t refs = 0);

  protected:

    using codecvt<internT, externT, stateT>::result;

    ~codecvt_byname(); // virtual

    virtual result do_out(stateT& state,
                          const internT* from,
                          const internT* from_end,
                          const internT*& from_next,
                          externT* to,
                          externT* to_limit,
                          externT*& to_next) const;

    virtual result do_in(stateT& state, const externT* from, const externT* from_end,
      const externT*& from_next, internT* to, internT* to_limit, internT*& to_next) const;

    virtual int do_encoding() const __ntl_nothrow;

    virtual bool do_always_noconv() const __ntl_nothrow;

    virtual int do_length(stateT&, const externT* from, const externT* end, size_t max) const;

    virtual result do_unshift(stateT& state, externT* to, externT* to_limit, externT*& to_next) const;

    virtual int do_max_length() const __ntl_nothrow;

};

}//namespace std

#include "ios.hxx"

namespace std {

// 22.2.2 The numeric category [lib.category.numeric]

/// 22.2.2.1 Class template num_get [lib.locale.num.get]
template <class charT, class InputIterator = istreambuf_iterator<charT> >
class num_get : public locale::facet
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    typedef charT         char_type;
    typedef InputIterator iter_type;

    explicit num_get(size_t refs = 0);
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, bool& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, long& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, unsigned short& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, unsigned int& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, unsigned long& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, float& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, double& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, long double& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, void*& v) const;
    static locale::id id;

  ///////////////////////////////////////////////////////////////////////////
  protected:

    ~num_get(); //virtual

    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, bool& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, long& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, unsigned short& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, unsigned int& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, unsigned long& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, float& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, double& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, long double& v) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate& err, void*& v) const;
};

/// 22.2.2.2 Class template num_put [lib.locale.nm.put]
template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
class num_put : public locale::facet
{
  ///////////////////////////////////////////////////////////////////////////
  public:

    typedef charT           char_type;
    typedef OutputIterator  iter_type;

    explicit num_put(size_t refs = 0);

    iter_type put(iter_type s, ios_base& f, char_type fill, bool v) const
    {
      return do_put(s, f, fill, v);
    }

    iter_type put(iter_type s, ios_base& f, char_type fill, long v) const
    {
      return do_put(s, f, fill, v);
    }

    iter_type put(iter_type s, ios_base& f, char_type fill, unsigned long v) const
    {
      return do_put(s, f, fill, v);
    }

    iter_type put(iter_type s, ios_base& f, char_type fill, double v) const
    {
      return do_put(s, f, fill, v);
    }

    iter_type put(iter_type s, ios_base& f, char_type fill, long double v) const
    {
      return do_put(s, f, fill, v);
    }

    iter_type put(iter_type s, ios_base& f, char_type fill, const void* v) const
    {
      return do_put(s, f, fill, v);
    }

    static locale::id id;

  ///////////////////////////////////////////////////////////////////////////
  protected:

    ~num_put(); //virtual

    virtual iter_type
      do_put(iter_type out, ios_base& str, char_type fill, bool val) const
    {
      if ( (str.flags() & ios_base::boolalpha) == 0 )
        return do_put(out, str, fill, static_cast<int>(val));
      const numpunct<charT>& np = use_facet<numpunct<charT> >(loc);
      string_type s = val ? np.truename() : np.falsename();
    }

    virtual iter_type do_put(iter_type, ios_base&, char_type fill, long v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, unsigned long) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, double v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, long double v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, const void* v) const;

};

template <>
class num_put<char>;// : public locale::facet


// 22.2.3 The numeric punctuation facet [lib.facet.numpunct]

template <class charT> class numpunct;
template <class charT> class numpunct_byname;

// 22.2.4, collation:

template <class charT> class collate;
template <class charT> class collate_byname;

// 22.2.5, date and time:

class time_base;
template <class charT, class InputIterator> class time_get;
template <class charT, class InputIterator> class time_get_byname;
template <class charT, class OutputIterator> class time_put;
template <class charT, class OutputIterator> class time_put_byname;

// 22.2.6, money:

class money_base;
template <class charT, class InputIterator> class money_get;
template <class charT, class OutputIterator> class money_put;
template <class charT, bool Intl> class moneypunct;
template <class charT, bool Intl> class moneypunct_byname;

// 22.2.7, message retrieval:

class messages_base;
template <class charT> class messages;
template <class charT> class messages_byname;

}//namespace std

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif//#ifndef NTL__STLX_LOCALE