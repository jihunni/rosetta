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
#include <protocols/bootcamp/fold_tree_from_ss.hh>

// --------------- Test Class --------------- //
static basic::Tracer  TR("test.protocols.bootcamp.FoldTreeFromSSTests");

class FoldTreeFromSSTests : public CxxTest::TestSuite {

public:


	// --------------- Fixtures --------------- //

	// Define a test fixture (some initial state that several tests share)
	// In CxxTest, setUp()/tearDown() are executed around each test case. If you need a fixture on the test
	// suite level, i.e. something that gets constructed once before all the tests in the test suite are run,
	// suites have to be dynamically created. See CxxTest sample directory for example.

	// Shared initialization goes here.
	void setUp() {
		core_init();
	}

	// Shared finalization goes here.
	void tearDown() {
	}

	void test_secondary_structure_spans(){
		static basic::Tracer  TR("test.protocols.bootcamp.FoldTreeFromSSTests test_secondary_structure_spans");
		std::string test_str = "HHHHHHH   HHHHHHHHHHHH      HHHHHHHHHHHHEEEEEEEEEEHHHHHHH EEEEHHH ";
		utility::vector1< std::pair< core::Size, core::Size > > SS_vector ;

		utility::vector1<int> answer_vector_first = utility::vector1<int> {1,11,29,41,51,59,63};
		utility::vector1<int> answer_vector_second = utility::vector1<int> {7,22,40,50,57,62,65};

		SS_vector = protocols::bootcamp::identify_secondary_structure_spans(test_str) ;
        	for ( core::Size ii = 1; ii <= SS_vector.size(); ++ii ) {
				TS_ASSERT( static_cast<int>(SS_vector[ ii ].first) == static_cast<int>(answer_vector_first[ ii ]) );
				TS_ASSERT( static_cast<int>(SS_vector[ ii ].second) == static_cast<int>(answer_vector_second[ ii ]) );
        }
	};

	void test_get_residue_edge(){
		std::string dssp_string = "   EEEEEEE    EEEEEEE         EEEEEEEEE    EEEEEEEEEE   HHHHHH         EEEEEEEEE         EEEEE     ";
		std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > 
 		edge_info = protocols::bootcamp::secondary_structure_string_to_edge_information(dssp_string) ;
		core::kinematics::FoldTree fold_tree = protocols::bootcamp::fold_tree_from_edge_info(edge_info);

		// for ( core::Size resi = 1; resi <= dssp_string.size(); ++resi ) {
		// 	if (resi == fold_tree.root()) continue;
		// 	core::kinematics::Edge const & edge = fold_tree.get_residue_edge(resi);
		// 	std::cout << "residue " << resi << " is connected by edge: "
		// 		<< edge.start() << " to " << edge.stop()
		// 		<< " of type " << edge.label() << std::endl;
		// }
		TS_ASSERT( fold_tree.check_fold_tree() );
	};

	void test_from_pose_to_fold_tree(){
		core::pose::Pose pose = create_test_in_pdb_pose();
		// core::pose::Pose pose = test::util::create_test_in_pdb_pose();
		pose = protocols::bootcamp::pose_to_fold_tree(pose);
		TS_ASSERT( pose.fold_tree().check_fold_tree() );
	};
};