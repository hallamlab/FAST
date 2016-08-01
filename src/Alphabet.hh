// Copyright 2008, 2009, 2010, 2012, 2013 Martin C. Frith

// This struct maps characters to codes (small integers) and back.

// We allow for both "proper" letters (e.g. ACGT for DNA), which get
// the lowest codes, and "improper" letters.  This is because real
// sequence data includes additional letters (e.g. ambiguous bases),
// so we have to handle them.  In addition, the space character
// represents a special delimiter.

#ifndef ALPHABET_HH
#define ALPHABET_HH

#include <string>
#include <iosfwd>

namespace cbrc{

typedef unsigned char uchar;

struct Alphabet{
  typedef unsigned long long countT;

  /* STATIC */
  static const char* dna;
  static const char* protein;

  static const unsigned capacity = 256;

  /* FIELDS */
  std::string letters;    // the "proper" letters, e.g. ACGT for DNA
  unsigned size;          // same as letters.size(): excludes delimiters

  uchar *encode;  // translate ASCII letters to codes (small integers)
  uchar *decode;  // translate codes to ASCII letters
  uchar *canonical;   // translate lowercase codes to uppercase codes
  uchar *complement;  // translate DNA codes to their complements

  /* METHODS */
  // does this alphabet start with the standard protein alphabet?
  bool isProtein() const{ return letters.find( protein ) == 0; }

  // make an Alphabet from a string containing the "proper" letters
  void fromString( const std::string& alphString );

  // add counts of "proper" letters to "counts" (counting lowercase too)
  void count( const uchar* beg, const uchar* end, countT* counts ) const;

  // translate (encode) a sequence of letters to numbers, in place
  void tr( uchar* beg, uchar* end ) const;

  // reverse-translate (decode) a sequence of numbers to letters
  // return the position after the last written position in dest
  char* rtCopy( const uchar* beg, const uchar* end, char* dest ) const;

  // reverse and complement a sequence of numbers, in place
  void rc( uchar* beg, uchar* end ) const;

  void init();
  void addLetters( const std::string& lettersToAdd, unsigned& code );
  void makeComplement();

  /* CON/DE STRUCTORS */ 
  Alphabet(){
    encode = new uchar[capacity];  // translate ASCII letters to codes (small integers)
    decode = new uchar[capacity];  // translate codes to ASCII letters
    canonical = new uchar[capacity];   // translate lowercase codes to uppercase codes
    complement = new uchar[capacity];  // translate DNA codes to their complements
  }

  ~Alphabet(){
    delete[] encode;
    delete[] decode;
    delete[] canonical;
    delete[]complement;
  }
};

std::ostream& operator<<( std::ostream& s, const Alphabet& a );
std::istream& operator>>( std::istream& s, Alphabet& a );

}  // end namespace
#endif
