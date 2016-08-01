// Copyright 2008, 2009, 2010, 2011, 2013, 2014 Martin C. Frith

// This struct holds the command line arguments for lastdb.

#ifndef LASTDB_ARGUMENTS_HH
#define LASTDB_ARGUMENTS_HH

#include <cstddef>  // size_t
#include "SequenceFormat.hh"

#include <string>
#include <vector>

namespace cbrc{

struct LastdbArguments{
  //typedef std::size_t indexT;
  typedef unsigned indexT;

  // set the parameters to their default values:
  LastdbArguments();

  // set parameters from a list of arguments:
  void fromArgs( int argc, char** argv );

  // options:
  bool isProtein;
  bool isCaseSensitive;
  std::vector< std::string > seedPatterns;
  std::size_t volumeSize;  // type?
  bool unlimited;
  indexT indexStep;
  std::vector< std::string > subsetSeedFiles;
  std::string userAlphabet;
  indexT minSeedLimit;
  indexT bucketDepth;
  bool isCountsOnly;
  int verbosity;
  bool version;
  sequenceFormat::Enum inputFormat;

  // positional arguments:
  std::string lastdbName;
  int inputStart;  // index in argv of first input filename
};

}  // end namespace
#endif
