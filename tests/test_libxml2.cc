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

#include <iostream>
#include <map>

#include <libxml/tree.h>
#include <sys/time.h>
#include <time.h>

#include "pet.hh"

namespace pet {

  template <>
  struct Traits<xmlNode>
  {
    typedef xmlNode* type;
    typedef xmlNode* type_opt;
  
    static type_opt none(void) { return NULL; }
    static type_opt some(type x) { return x; }
    static bool is_none(type_opt x) { return x == none(); }
    static type of_opt(type_opt x) { return x; }
  };

}

namespace xpe {

  xmlNode*
  parentNode(xmlNode* x)
  { return x->parent; };

  xmlNode*
  firstChild(xmlNode* x)
  { return x->children; };

  xmlNode*
  lastChild(xmlNode* x)
  { return x->last; };

  xmlNode*
  nextSibling(xmlNode* x)
  { return x->next; };

  xmlNode*
  previousSibling(xmlNode* x)
  { return x->prev; };

  struct Atom
  {
    typedef xmlNode in;
    typedef xmlNode out;
  };

  struct XMLString
  {
    XMLString(const char* _str) : buffer(_str ? XMLStringBuffer::create(BAD_CAST(_str)) : 0) { }
    XMLString(const xmlChar* _str) : buffer(_str ? XMLStringBuffer::create(_str) : 0) { }
    XMLString(const XMLString& _str) : buffer(_str.buffer) { if (buffer) buffer->ref(); }
    ~XMLString() { if (buffer) buffer->unref(); }

    const xmlChar* data(void) const { return buffer ? buffer->data() : 0; }
    operator const xmlChar*() const { return data(); }
    bool operator==(const XMLString& s) const { return xmlStrEqual(data(), s.data()); }
    bool operator==(const xmlChar* s) const { return xmlStrEqual(data(), s); }
    bool operator==(const char* s) const { return xmlStrEqual(data(), BAD_CAST(s)); }
    bool operator!=(const XMLString& s) const { return !(*this == s); }
    bool operator!=(const xmlChar* s) const { return !(*this == s); }
    bool operator!=(const char* s) const { return !(*this == s); }
    XMLString& operator=(const XMLString& s)
    {
      if (this == &s) return *this;
      if (s.buffer) s.buffer->ref();
      if (buffer) buffer->unref();
      buffer = s.buffer;
      return *this;
    }

  private:
    struct XMLStringBuffer {
      static XMLStringBuffer* create(const xmlChar* str)
      { return new XMLStringBuffer(str); }

      void ref(void) const { counter++; }
      void unref(void) const { if (--counter == 0) delete this; }

      const xmlChar* data(void) const { return str; }

    private:
      XMLStringBuffer(const xmlChar* _str) : str(xmlStrdup(_str)), counter(1) { }
      ~XMLStringBuffer() { xmlFree(str); }

      xmlChar* str;
      mutable unsigned counter;
    };

    XMLStringBuffer* buffer;
  };

  struct NameIs : public Atom
  {
    NameIs(const XMLString& _name) : name(_name) { }

    const XMLString& getName(void) const { return name; }
    xmlNode* walk(xmlNode* x) const { return (name == x->name) ? x : 0; }

  private:
    XMLString name;
  };

  struct NamespaceIs : public Atom
  {
    NamespaceIs(const XMLString& _ns) : ns(_ns) { }

    const XMLString& getNamespace(void) const { return ns; }
    xmlNode* walk(xmlNode* x) const
    { return (x->ns && ns == x->ns->href) ? x : 0; }

  private:
    XMLString ns;
  };

  template <int type>
  struct TypeIs : public Atom
  { xmlNode* walk(xmlNode* x) const { return (x->type == type) ? x : 0; } };

  struct HasProp : public Atom
  {
    HasProp(const XMLString& _prop) : prop(_prop) { }

    const XMLString& getProp(void) const { return prop; }
    xmlNode* walk(xmlNode* x) const { return xmlHasProp(x, prop) ? x : 0; }

  private:
    XMLString prop;
  };

}

int
main(int argc, char* argv[])
{
  if (argc != 3) {
	  std::cerr << "usage: test <URL> <test#>" << std::endl;
	  return -1;
  }

  if (xmlDocPtr doc = xmlReadFile(argv[1], NULL, 0))
    {
      pet::Sink<xmlNode> sink;

      struct timeval start;
      struct timeval end;

      *pet::fun<xmlNode>(xpe::firstChild)((xmlNode*) doc);
      gettimeofday(&start, NULL);
#if 1
      for (int i = 0; i < 20; i++) {
        switch (argv[2][0]) {
	  case '1':
            (*((pet::fun<xmlNode>(xpe::firstChild) >> *(pet::fun<xmlNode>(xpe::nextSibling))))
             >> pet::sink(sink) >> pet::empty<xmlNode>())((xmlNode*) doc);
	    break;
	  case '2':
            (*((pet::fun<xmlNode>(xpe::firstChild) >> *(pet::fun<xmlNode>(xpe::nextSibling))))
	     >> pet::atom(xpe::TypeIs<XML_ELEMENT_NODE>())
	     >> pet::atom(xpe::NamespaceIs("http://www.w3.org/1998/Math/MathML"))
	     >> pet::atom(xpe::NameIs("mrow"))
	     >> pet::atom(xpe::HasProp("xref"))
             >> pet::sink(sink) >> pet::empty<xmlNode>())((xmlNode*) doc);
	    break;
	  case '3':
            (*((pet::fun<xmlNode>(xpe::firstChild) >> *(pet::fun<xmlNode>(xpe::nextSibling))))
	     >> pet::atom(xpe::TypeIs<XML_TEXT_NODE>())
	     >> pet::fun<xmlNode>(xpe::parentNode)
	     >> pet::atom(xpe::TypeIs<XML_ELEMENT_NODE>())
	     >> pet::atom(xpe::NamespaceIs("http://www.w3.org/1998/Math/MathML"))
	     >> pet::atom(xpe::NameIs("mrow"))
	     >> pet::atom(xpe::HasProp("xref"))
             >> pet::sink(sink) >> pet::empty<xmlNode>())((xmlNode*) doc);
	    break;
	  case '4':
            ((*((pet::fun<xmlNode>(xpe::firstChild) >> *(pet::fun<xmlNode>(xpe::nextSibling))))
	      >> pet::atom(xpe::TypeIs<XML_TEXT_NODE>()))
	     [pet::fun<xmlNode>(xpe::parentNode)
	      >> pet::atom(xpe::TypeIs<XML_ELEMENT_NODE>())
	      >> pet::atom(xpe::NamespaceIs("http://www.w3.org/1998/Math/MathML"))
	      >> pet::atom(xpe::NameIs("mrow"))
	      >> pet::atom(xpe::HasProp("xref"))]
              >> pet::sink(sink) >> pet::empty<xmlNode>())((xmlNode*) doc);
	    break;
	  case '5':
            ((*((pet::fun<xmlNode>(xpe::firstChild) >> *(pet::fun<xmlNode>(xpe::nextSibling))))
	      >> pet::atom(xpe::TypeIs<XML_ELEMENT_NODE>())
	      >> pet::atom(xpe::NamespaceIs("http://www.w3.org/1998/Math/MathML"))
	      >> pet::atom(xpe::NameIs("mrow"))
	      >> pet::atom(xpe::HasProp("xref")))
	     [pet::fun<xmlNode>(xpe::firstChild)
	      >> pet::fun<xmlNode>(xpe::nextSibling)
	      >> pet::fun<xmlNode>(xpe::nextSibling)
	      >> pet::fun<xmlNode>(xpe::nextSibling)
	      >> pet::fun<xmlNode>(xpe::nextSibling)
	      >> pet::fun<xmlNode>(xpe::nextSibling)]
             >> pet::sink(sink) >> pet::empty<xmlNode>())((xmlNode*) doc);
	    break;
	}
      }
#endif
      gettimeofday(&end, NULL);

      long t0 = start.tv_sec * 1000000 + start.tv_usec;
      long t1 = end.tv_sec * 1000000 + end.tv_usec;

      std::cout
	      << "counter has found " 
	      << sink.getSink().size() << " nodes in "
	      << (t1 - t0) / 1000.0 << " msec" << std::endl;

      xmlFreeDoc(doc);
    }
  else
    {
      std::cerr << "Failed to parse " << argv[1] << std::endl;
    }

  return 0;
}
