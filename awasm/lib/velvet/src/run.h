/*
Copyright 2007, 2008 Daniel Zerbino (zerbino@ebi.ac.uk)

    This file is part of Velvet.

    Velvet is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Velvet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Velvet; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
// Compilation
#include "velvet/src/globals.h"

// Utilities
#include "velvet/src/graphStats.h"
#include "velvet/src/utility.h"

// Datastructures
#include "velvet/src/kmer.h"
#include "velvet/src/readSet.h"
#include "velvet/src/tightString.h"
#include "velvet/src/roadMap.h"
#include "velvet/src/splayTable.h"
#include "velvet/src/graph.h"
#include "velvet/src/scaffold.h"
#include "velvet/src/binarySequences.h"

// PreGraph operations
#include "velvet/src/preGraph.h"
#include "velvet/src/preGraphConstruction.h"
#include "velvet/src/concatenatedPreGraph.h"

// Graph operations
#include "velvet/src/graph.h"
#include "velvet/src/graphReConstruction.h"
#include "velvet/src/concatenatedGraph.h"
#include "velvet/src/correctedGraph.h"
#include "velvet/src/locallyCorrectedGraph.h"

// Repeat resolution
#include "velvet/src/readCoherentGraph.h"
#include "velvet/src/shortReadPairs.h"
