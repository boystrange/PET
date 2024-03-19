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

#ifndef __pet_derived_paths_hh__
#define __pet_derived_paths_hh__

#include "pet_simple_paths.hh"

namespace pet {

  template <typename Object>
  struct EmptyPath : public AtomPath<NullTerm<Object> >
  { EmptyPath(void) : AtomPath<NullTerm<Object> >(NullTerm<Object>()) { } };

  template <typename Object>
  struct EpsilonPath : public AtomPath<IdTerm<Object> >
  { EpsilonPath(void) : AtomPath<IdTerm<Object> >(IdTerm<Object>()) { } };

  template <typename P>
  struct PlusPath : public SeqPath<P, StarPath<P> >
  { PlusPath(const P& _p = P()) : SeqPath<P, StarPath<P> >(_p, StarPath<P>(_p)) { } };

  template <typename P, int n, int m>
  struct RangePath : public SeqPath<P, RangePath<P, n - 1, m - 1> >
  { RangePath(const P& _p = P()) : SeqPath<P, RangePath<P, n - 1, m - 1> >(_p, RangePath<P, n - 1, m - 1>(_p)) { } };

  template <typename P, int m>
  struct RangePath<P, 0, m>
    : public OrPath<EpsilonPath<typename P::in>, SeqPath<P, RangePath<P, 0, m - 1> > >
  { RangePath(const P& _p = P()) : OrPath<EpsilonPath<typename P::in>, SeqPath<P, RangePath<P, 0, m - 1> > >(EpsilonPath<typename P::in>(), SeqPath<P, RangePath<P, 0, m - 1> >(_p, RangePath<P, 0, m - 1>(_p))) { } };

  template <typename P>
  struct RangePath<P, 0, 0> : public EpsilonPath<typename P::in>
  { RangePath(const P& = P()) : EpsilonPath<typename P::in>() { } };

  template <typename P>
  struct OptionPath : public RangePath<P, 0, 1>
  { OptionPath(const P& _p = P()) : RangePath<P, 0, 1>(_p) { } };

  template <typename P, int n>
  struct RepPath : public RangePath<P, n, n>
  { RepPath(const P& _p = P()) : RangePath<P, n, n>(_p) { } };

}

#endif // __pet_derived_paths_hh__
