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

#ifndef __pet_traits_hh__
#define __pet_traits_hh__

namespace pet {
	
  template <typename Object>
  struct Traits
  {
    typedef const Object& type;
    typedef const Object* type_opt;

    static type_opt none(void) { return 0; }
    static type_opt some(type x) { return &x; }
    static bool is_none(type_opt x) { return x == none(); }
    static type of_opt(type_opt x) { return *x; }
  };

  template <>
  struct Traits<bool>
  {
    typedef bool type;
    typedef bool type_opt;

    static type_opt none(void) { return false; }
    static type_opt some(type) { return true; }
    static bool is_none(type_opt x) { return x == false; }
    static type of_opt(type_opt x) { return x; }
  };

}

#endif // __pet_traits_hh__

