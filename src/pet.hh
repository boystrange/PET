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

#ifndef __pet_hh__
#define __pet_hh__

#include "pet_terms.hh"
#include "pet_simple_paths.hh"
#include "pet_derived_paths.hh"

namespace pet {

  template <typename P>
  struct Path {
    Path(const P& _p) : path(_p) { }
    P path;
    
    typename Traits<typename P::out>::type_opt
    walk(typename Traits<typename P::in>::type x) const
    { return path.compile(IdTerm<typename P::out>()).walk(x); }

    typename Traits<typename P::out>::type_opt
    operator()(typename Traits<typename P::in>::type x) const
    { return walk(x); }

    template <typename P1>
    Path<SeqPath<P, FilterPath<P1> > >
    operator[](const Path<P1>& p)
    { return Path<SeqPath<P, FilterPath<P1> > >(SeqPath<P, FilterPath<P1> >(path, FilterPath<P1>(p.path))); }

  };

  template <typename Object>
  Path<EmptyPath<Object> >
  empty(void)
  { return EmptyPath<Object>(); }

  template <typename Object>
  Path<EpsilonPath<Object> >
  epsilon(void)
  { return EpsilonPath<Object>(); }

  template <typename A>
  Path<AtomPath<A> >
  atom(const A& a)
  { return AtomPath<A>(a); }

  template <typename ObjectIn, typename ObjectOut>
  Path<AtomPath<FunTerm<ObjectIn, ObjectOut> > >
  fun(typename Traits<ObjectOut>::type_opt (*f)(typename Traits<ObjectIn>::type))
  { return AtomPath<FunTerm<ObjectIn, ObjectOut> >(FunTerm<ObjectIn, ObjectOut>(f)); }

  template <typename Object,typename Compare, typename Alloc>
  Path<AtomPath<SinkTerm<Object, Compare, Alloc> > >
  sink(Sink<Object, Compare, Alloc>& sink)
  { return AtomPath<SinkTerm<Object, Compare, Alloc> >(SinkTerm<Object, Compare, Alloc>(sink)); }

  template <typename Object>
  Path<AtomPath<FunTerm<Object, Object> > >
  fun(typename Traits<Object>::type_opt (*f)(typename Traits<Object>::type))
  { return AtomPath<FunTerm<Object, Object> >(FunTerm<Object, Object>(f)); }

  template <typename Object>
  Path<AtomPath<PredTerm<Object> > >
  when(bool (*p)(typename Traits<Object>::type))
  { return AtomPath<PredTerm<Object> >(PredTerm<Object>(p)); }

  template<typename P1, typename P2>
  Path<OrPath<P1, P2> >
  alt(const Path<P1>& p1, const Path<P2>& p2)
  { return OrPath<P1, P2>(p1.path, p2.path); }

  template <typename P1, typename P2>
  Path<SeqPath<P1, P2> >
  seq(const Path<P1>& p1, const Path<P2>& p2)
  { return SeqPath<P1, P2>(p1.path, p2.path); }

  template <typename P>
  Path<StarPath<P> >
  star(const Path<P>& p)
  { return StarPath<P>(p.path); }

  template <typename P>
  Path<PlusPath<P> >
  plus(const Path<P>& p)
  { return PlusPath<P>(p.path); }

  template <typename P>
  Path<OptionPath<P> >
  option(const Path<P>& p)
  { return OptionPath<P>(p.path); }

  template <typename P, int n>
  Path<RepPath<P, n> >
  rep(const Path<P>& p)
  { return RepPath<P, n>(p.path); }

  template <typename P, int n, int m>
  Path<RangePath<P, n, m> >
  range(const Path<P>& p)
  { return RangePath<P, n, m>(p); }

  template <typename P>
  Path<SharedPath<P> >
  share(const Path<P>& p)
  { return SharedPath<P>(p.path); }

  template <typename P>
  Path<FilterPath<P> >
  filter(const Path<P>& p)
  { return FilterPath<P>(p.path); }

  template <typename P>
  Path<NotPath<P> >
  notp(const Path<P>& p)
  { return NotPath<P>(p.path); }
}

template <typename P>
pet::Path<pet::NotPath<P> >
operator!(const pet::Path<P>& p)
{ return pet::notp(p); }

template <typename P1, typename P2>
pet::Path<pet::OrPath<P1, P2> >
operator|(const pet::Path<P1>& p1, const pet::Path<P2>& p2)
{ return pet::alt(p1, p2); }

template <typename P1, typename P2>
pet::Path<pet::SeqPath<P1, P2> >
operator>>(const pet::Path<P1>& p1, const pet::Path<P2>& p2)
{ return pet::seq(p1, p2); }

template <typename P>
pet::Path<pet::StarPath<P> >
operator*(const pet::Path<P>& p)
{ return pet::star(p); }

template <typename P>
pet::Path<pet::PlusPath<P> >
operator+(const pet::Path<P>& p)
{ return pet::plus(p); }

#endif // __pet_hh__
