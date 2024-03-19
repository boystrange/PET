// Copyright (C) 2004, Luca Padovani <lpadovan@cs.unibo.it>.
//
// This file is part of PET, a C++ library for Path Expression
// Templates
// 
// PET is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// PET is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the PET World-Wide-Web page,
// http://www.cs.unibo.it/~lpadovan/PET/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __pet_terms_hh__
#define __pet_terms_hh__

#include <set>

#include "pet_traits.hh"

namespace pet {

  template <typename Object>
  struct TrueTerm
  {
    typedef Object in;
    typedef bool out;

    bool
    walk(typename Traits<in>::type) const
    { return true; }
  };

  template <typename Object>
  struct IdTerm
  {
    typedef Object in;
    typedef Object out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { return Traits<out>::some(x); }
  };

  template <typename Object>
  struct NullTerm
  {
    typedef Object in;
    typedef Object out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type) const
    { return Traits<out>::none(); }
  };

  template <typename Object,
	    typename Compare = std::less<typename Traits<Object>::type>, 
	    typename Alloc = std::allocator<typename Traits<Object>::type> >
  struct Sink
  {
    typedef std::set<typename Traits<Object>::type, Compare, Alloc> Set;

    bool has(typename Traits<Object>::type x) const { return sink.find(x) != sink.end(); }
    void add(typename Traits<Object>::type x) { sink.insert(x); }
    const Set& getSink(void) const { return sink; }

  private:
    Set sink;
  };

  template <typename Object,
	    typename Compare = std::less<typename Traits<Object>::type>, 
	    typename Alloc = std::allocator<typename Traits<Object>::type> >
  struct SinkTerm
  {
    SinkTerm(Sink<Object, Compare, Alloc>& _sink) : sink(_sink) { }

    typedef Object in;
    typedef Object out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { 
      if (!sink.has(x))
	{
	  sink.add(x);
	  return Traits<out>::some(x);
	}
      else
	return Traits<out>::none();
    }

  private:
    Sink<Object, Compare, Alloc>& sink;
  };

  template <typename ObjectIn, typename ObjectOut>
  struct FunTerm
  {
    typedef ObjectIn in;
    typedef ObjectOut out;

    FunTerm(typename Traits<out>::type_opt (*_f)(typename Traits<in>::type)) : f(_f) { }

    typename Traits<out>::type_opt walk(typename Traits<in>::type x) const { return f(x); }
    
  private:
    typename Traits<out>::type_opt (*f)(typename Traits<in>::type);
  };

  template <typename Object>
  struct PredTerm
  {
    typedef Object in;
    typedef Object out;

    PredTerm(bool (*_p)(typename Traits<in>::type)) : p(_p) { }

    typename Traits<out>::type_opt walk(typename Traits<in>::type x) const
    { return p(x) ? Traits<out>::some(x) : Traits<out>::none(); }

  private:
    bool (*p)(typename Traits<in>::type);
  };
  
  template <typename K1, typename K2, typename K3>
  struct ForkTerm
  {
    ForkTerm(const K1& _k1, const K2& _k2, const K3& _k3) : k1(_k1), k2(_k2), k3(_k3) { }

    typedef typename K1::in in;
    typedef typename K2::out out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    {
      typename Traits<typename K1::out>::type_opt y = k1.walk(x);
      if (!Traits<typename K1::out>::is_none(y))
	return k2.walk(Traits<typename K1::out>::of_opt(y));
      else
	return k3.walk(x);
    }

  private:
    const K1 k1;
    const K2 k2;
    const K3 k3;
  };

  template <typename Object, typename K2, typename K3>
  struct ForkTerm<IdTerm<Object>, K2, K3>
  {
    ForkTerm(const IdTerm<Object>&, const K2& _k2, const K3&) : k2(_k2) { }

    typedef Object in;
    typedef typename K2::out out;
    
    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { return k2.walk(x); }

  private:
    const K2 k2;
  };

  template <typename K1, typename K2, typename K3>
  ForkTerm<K1, K2, K3>
  make_fork_term(const K1& k1, const K2& k2, const K3& k3)
  { return ForkTerm<K1, K2, K3>(k1, k2, k3); }

  template <typename K, typename In = typename K::in, typename Out = typename K::out>
  struct WeakRefTerm
  {
    WeakRefTerm(const K& _k) : k(_k) { }

    typedef In in;
    typedef Out out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { return k.walk(x); }

  private:
    const K& k;
  };

  template <typename K>
  struct BoxedTerm
  {
    static BoxedTerm* create(const K& k) { return new BoxedTerm(k); }

    typedef typename K::in in;
    typedef typename K::out out;

    void ref(void) const { this->counter++; }
    void unref(void) const { if (--this->counter == 0) delete this; }

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { return k.walk(x); }

  protected:
    BoxedTerm(const K& _k) : k(_k), counter(0) { }

  private:
    BoxedTerm(const BoxedTerm&); // this object must not be copied
    BoxedTerm& operator=(const BoxedTerm&); // this object must not be assigned

    const K k;
    mutable unsigned counter;
  };

  // by passing WeakRef explicit in and out types we apparently break a
  // circularity that prevents the compiler from inferring those types
  // automatically
  template <typename P, typename K>
  struct FixTerm
    : public BoxedTerm<ForkTerm<K, IdTerm<typename K::in>,
				typename P::template Compile<WeakRefTerm<FixTerm<P, K>, typename P::in, typename P::out > >::RES> >
  {
    static FixTerm* create(const P& p, const K& k) { return new FixTerm(p, k); }

    typedef typename K::in in;
    typedef typename K::in out;

  private:
    FixTerm(const P& p, const K& k)
      : BoxedTerm<ForkTerm<K, IdTerm<typename K::in>,
			   typename P::template Compile<WeakRefTerm<FixTerm> >::RES> >(make_fork_term(k, IdTerm<typename K::in>(),
												      p.compile(WeakRefTerm<FixTerm>(*this))))
    { }

    FixTerm(const FixTerm&); // this object must not be copied
    FixTerm& operator=(const FixTerm&); // this object must not be assigned
  };

#if 0
  // by passing WeakRef explicit in and out types we apparently break a
  // circularity that prevents the compiler from inferring those types
  // automatically
  template <typename P, typename K>
  struct FixTerm : public ForkTerm<K, IdTerm<typename K::in>,
				   typename P::template Compile<WeakRefTerm<FixTerm<P, K>, typename P::in, typename P::out > >::RES>
  {
    static FixTerm* create(const P& p, const K& k) { return new FixTerm(p, k); }

    typedef typename K::in in;
    typedef typename K::in out;

    void ref(void) const { this->counter++; }
    void unref(void) const { if (--this->counter == 0) delete this; }
    
  private:
    FixTerm(const P& p, const K& k)
      : ForkTerm<K, IdTerm<typename K::in>,
		 typename P::template Compile<WeakRefTerm<FixTerm> >::RES>(k, IdTerm<typename K::in>(),
									   p.compile(WeakRefTerm<FixTerm>(*this)))
      , counter(0) { }

    FixTerm(const FixTerm&); // this object must not be copied
    FixTerm& operator=(const FixTerm&); // this object must not be assigned

    mutable unsigned counter;
  };
#endif

  template <typename K>
  struct RefTerm
  {
    RefTerm(const K* _k) : k(_k) { k->ref(); }
    RefTerm(const RefTerm& r) : k(r.k) { k->ref(); }
    ~RefTerm() { k->unref(); }

    typedef typename K::in in;
    typedef typename K::out out;

    typename Traits<out>::type_opt
    walk(typename Traits<in>::type x) const
    { return k->walk(x); }

  private:
    RefTerm& operator=(const RefTerm&); // should not happen
    const K* k;
  };

  template <typename K>
  struct FilterTerm
  {
    FilterTerm(const K& _k) : k(_k) { }

    typedef typename K::in in;
    typedef typename K::in out;

    typename Traits<in>::type_opt
    walk(typename Traits<in>::type x) const
    { return Traits<out>::is_none(k.walk(x)) ? Traits<out>::none() : Traits<out>::some(x); }

  private:
    const K k;
  };

  template <typename K>
  struct NotTerm
  {
    NotTerm(const K& _k) : k(_k) { }

    typedef typename K::in in;
    typedef typename K::in out;

    typename Traits<in>::type_opt
    walk(typename Traits<in>::type x) const
    { return Traits<out>::is_none(k.walk(x)) ? Traits<out>::some(x) : Traits<out>::none(); }

  private:
    const K k;
  };

#if 0
  template <typename In, typename Out>
  struct AbstractTerm
  {
    typedef In in;
    typedef Out out;

    typename Traits<out>::type_out
    walk(typename Traits<in>::type x) const
    { return walkAux(x); }

  protected:
    virtual typename Traits<out>::type_out
    walkAux(typename Traits<in>::type) const = 0;
  };
#endif

}

#endif // __pet_terms_hh__
