// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/bootcamp/PerResidueBfactorBootcamp.fwd.hh
/// @brief Simple Metric class for pLDDT value extraction from AF2
/// @author Jihun (jeung@uw.edu)

#ifndef INCLUDED_protocols_bootcamp_PerResidueBfactorBootcamp_fwd_hh
#define INCLUDED_protocols_bootcamp_PerResidueBfactorBootcamp_fwd_hh

// Utility headers
#include <utility/pointer/owning_ptr.hh>


// Forward
namespace protocols {
namespace bootcamp {

class PerResidueBfactorBootcamp;

using PerResidueBfactorBootcampOP = utility::pointer::shared_ptr< PerResidueBfactorBootcamp >;
using PerResidueBfactorBootcampCOP = utility::pointer::shared_ptr< PerResidueBfactorBootcamp const >;

} //bootcamp
} //protocols

#endif //INCLUDED_protocols_bootcamp_PerResidueBfactorBootcamp_fwd_hh
