// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   test/protocols/match/ProteinSCSampler.cxxtest.hh
/// @brief
/// @author Jihun Jeung (jeung@uw.edu)

// C++ headers
#include <iostream>

// Test headers
#include <cxxtest/TestSuite.h>

#include <test/util/pose_funcs.hh>
#include <test/core/init_util.hh>
#include <test/util/pose_funcs.hh>


/// Project headers
#include <basic/Tracer.hh>
#include <core/types.hh>
#include <core/kinematics/FoldTree.fwd.hh>
#include <core/kinematics/FoldTree.hh>
#include <core/kinematics/Edge.fwd.hh>
#include <core/kinematics/Edge.hh>
#include <protocols/moves/DsspMover.fwd.hh>
#include <protocols/moves/DsspMover.hh>



namespace protocols {
namespace bootcamp  {


class FoldTreeFromSSTests : public CxxTest::TestSuite {
public:
	void setUp();
	void tearDown();

	utility::vector1< std::pair< core::Size, core::Size > > 
	identify_secondary_structure_spans( std::string const & ss_string );

	std::string 
    dssp_str_from_pose(core::pose::Pose pose);

	std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size>> 
	secondary_structure_string_to_edge_information(std::string ss_string);

	core::kinematics::FoldTree 
    fold_tree_from_edge_info(std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > edge_info);

	void 
    test_secondary_structure_spans();
	
    void 
    test_get_residue_edge();
	
    void 
    test_from_pose_to_fold_tree();

}
}