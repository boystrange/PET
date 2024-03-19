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
// along with PET; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the PET World-Wide-Web page,
// http://www.cs.unibo.it/~lpadovan/PET/, or send a mail to
// <lpadovan@cs.unibo.it>


#pragma once

#include "pet_terms.hh"

namespace pet {

  template <typename A>
  struct AtomPath
  {
    AtomPath(const A& _a = A()) : a(_a) { }

    typedef typename A::in in;
    typedef typename A::out out;

    template <typename K>
    struct Compile { typedef ForkTerm<A, K, NullTerm<in> > RES; };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return make_fork_term(a, k, NullTerm<in>()); }

  private:
    A a;
  };

  template <typename P1, typename P2>
  struct OrPath
  {
    OrPath(const P1& _p1 = P1(), const P2& _p2 = P2()) : p1(_p1), p2(_p2) { }

    typedef typename P1::in in;
    typedef typename P1::out out;

    template <typename K>
    struct Compile {
      typedef typename P1::template Compile<K>::RES T1;
      typedef typename P2::template Compile<K>::RES T2;
      typedef ForkTerm<T1, IdTerm<typename T1::out>, T2> RES;
    };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return make_fork_term(p1.compile(k), IdTerm<typename Compile<K>::T1::out>(), p2.compile(k)); }

  private:
    P1 p1;
    P2 p2;
  };

  template <typename P1, typename P2>
  struct SeqPath
  {
    SeqPath(const P1& _p1 = P1(), const P2& _p2 = P2()) : p1(_p1), p2(_p2) { }

    typedef typename P1::in in;
    typedef typename P2::out out;

    template <typename K>
    struct Compile {
      typedef typename P2::template Compile<K>::RES T2;
      typedef typename P1::template Compile<T2>::RES RES;
    };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return p1.compile(p2.compile(k)); }

  private:
    P1 p1;
    P2 p2;
  };

  template <typename P>
  struct StarPath
  {
    StarPath(const P& _p = P()) : p(_p) { }

    typedef typename P::in in;
    typedef typename P::out out;

    template <typename K>
    struct Compile { typedef RefTerm<FixTerm<P, K> > RES; };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return typename Compile<K>::RES(FixTerm<P, K>::create(p, k)); }
  
  private:
    P p;
  };

  template <typename P>
  struct SharedPath 
  {
    SharedPath(const P& _p = P()) : p(_p) { }

    typedef typename P::in in;
    typedef typename P::out out;

    template <typename K>
    struct Compile { typedef RefTerm<BoxedTerm<typename P::template Compile<K>::RES> > RES; };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return typename Compile<K>::RES(BoxedTerm<typename P::template Compile<K>::RES>::create(p.compile(k))); }

  private:
    P p;
  };

  template <typename P>
  struct FilterPath
  {
    FilterPath(const P& _p) : p(_p) { }

    typedef typename P::in in;
    typedef typename P::in out;
    
    template <typename K>
    struct Compile
    { typedef typename AtomPath<FilterTerm<typename P::template Compile<IdTerm<in> >::RES> >::template Compile<K>::RES RES; };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return AtomPath<FilterTerm<typename P::template Compile<IdTerm<in> >::RES> >(FilterTerm<typename P::template Compile<IdTerm<in> >::RES>(p.compile(IdTerm<in>()))).compile(k); }
  
  private:
    P p;
  };

  template <typename P>
  struct NotPath
  {
    NotPath(const P& _p) : p(_p) { }

    typedef typename P::in in;
    typedef typename P::in out;

    template <typename K>
    struct Compile
    { typedef typename AtomPath<NotTerm<typename P::template Compile<IdTerm<in> >::RES> >::template Compile<K>::RES RES; };

    template <typename K>
    typename Compile<K>::RES
    compile(const K& k) const
    { return AtomPath<NotTerm<typename P::template Compile<IdTerm<in> >::RES> >(NotTerm<typename P::template Compile<IdTerm<in> >::RES>(p.compile(IdTerm<in>()))).compile(k); }

  private:
    P p;
  };

}

