// Copyright 2008, 2009, 2010, 2013 Martin C. Frith

// This class holds a suffix array.  The suffix array is just a list
// of numbers indicating positions in a text, sorted according to the
// alphabetical order of the text suffixes starting at these
// positions.  A query sequence can then be matched incrementally to
// the suffix array using binary search.

// A "subset suffix array" means that, when comparing two suffixes, we
// consider subsets of letters to be equivalent.  For example, we
// might consider purines to be equivalent to each other, and
// pyrimidines to be equivalent to each other.  The subsets may vary
// from position to position as we compare two suffixes.

// There is always a special subset, called DELIMITER, which doesn't
// match anything.

// For faster matching, we use "buckets", which store the start and
// end in the suffix array of all size-k prefixes of the suffixes.
// They store this information for all values of k from 1 to, say, 12.

#ifndef SUBSET_SUFFIX_ARRAY_HH
#define SUBSET_SUFFIX_ARRAY_HH

#include "CyclicSubsetSeed.hh"
#include "VectorOrMmap.hh"

namespace cbrc{

  class SubsetSuffixArray{
    public:
      //typedef std::size_t indexT;
      typedef unsigned indexT;

      CyclicSubsetSeed& getSeed() { return seed; }
      const CyclicSubsetSeed& getSeed() const { return seed; }

      // Add every step-th text position in the range [beg,end).
      // Positions starting with delimiters aren't added.
      // The positions aren't sorted.
      void addPositions( const uchar* text, indexT beg, indexT end, indexT step );

      // Sort the suffix array (but don't make the buckets).
      void sortIndex( const uchar* text, indexT maxUnsortedInterval );

      // Make the buckets.  If bucketDepth+1 == 0, then a default
      // bucketDepth is used.  The default is: the maximum possible
      // bucketDepth such that the number of bucket entries is at most 1/4
      // the number of suffix array entries.
      void makeBuckets( const uchar* text, indexT bucketDepth );

      // Clear the positions, so we can add new positions from scratch.
      void clearPositions();

      void fromFiles( const std::string& baseName,
          bool isMaskLowercase, const uchar letterCode[] );

      void closeFiles();

      void toFiles( const std::string& baseName,
          bool isAppendPrj, indexT textLength ) const;

      // Find the smallest match to the text, starting at the given
      // position in the query, such that there are at most maxHits
      // matches, or the match-depth is at least minDepth.  Return the
      // range of matching indices via beg and end.
      void match( const indexT*& beg, const indexT*& end,
          const uchar* queryPtr, const uchar* text,
          indexT maxHits, indexT minDepth ) const;

      // Count matches of all sizes, starting at the given position in the
      // query.  Don't try this for large self-comparisons!
      void countMatches( std::vector<unsigned long long>& counts,
          const uchar* queryPtr, const uchar* text ) const;

    //private:
public:
      CyclicSubsetSeed seed;
      VectorOrMmap<indexT> index;  // sorted indices
      VectorOrMmap<indexT> buckets;
      std::vector<indexT> bucketSteps;  // step size for each k-mer

      static void equalRange( const indexT*& beg, const indexT*& end,
          const uchar* textBase,
          const uchar* subsetMap, uchar symbol );
      static const indexT* lowerBound( const indexT* beg, const indexT* end,
          const uchar* textBase,
          const uchar* subsetMap, uchar subset );
      static const indexT* upperBound( const indexT* beg, const indexT* end,
          const uchar* textBase,
          const uchar* subsetMap, uchar subset );

      // Return the maximum prefix size covered by the buckets.
      indexT maxBucketPrefix() const { return bucketSteps.size() - 1; }

      indexT defaultBucketDepth();

      void makeBucketSteps( indexT bucketDepth );
  };

}  // end namespace
#endif
