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

	// --------------- Test Cases --------------- //
	utility::vector1< std::pair< core::Size, core::Size > > 
	identify_secondary_structure_spans( std::string const & ss_string )
	{
  	utility::vector1< std::pair< core::Size, core::Size > > ss_boundaries;
  	core::Size strand_start = -1;
	for ( core::Size ii = 0; ii < ss_string.size(); ++ii ) {
    		if ( ss_string[ ii ] == 'E' || ss_string[ ii ] == 'H'  ) {
      			if ( int( strand_start ) == -1 ) {
        			strand_start = ii;
    			} else if ( ss_string[ii] != ss_string[strand_start] ) {
   			ss_boundaries.push_back( std::make_pair( strand_start+1, ii ) );
        		strand_start = ii;
      			}
    		} else {
				if ( int( strand_start ) != -1 ) {
       				ss_boundaries.push_back( std::make_pair( strand_start+1, ii ) );
        			strand_start = -1;
     			}
    		}
  	}
 	 if ( int( strand_start ) != -1 ) {
    	// last residue was part of a ss-eleemnt
    	ss_boundaries.push_back( std::make_pair( strand_start+1, ss_string.size() ));
 	 }
  	for ( core::Size ii = 1; ii <= ss_boundaries.size(); ++ii ) {
    		std::cout << "SS Element " << ii << " from residue "
      		<< ss_boundaries[ ii ].first << " to "
      		<< ss_boundaries[ ii ].second << std::endl;
  	}
  	return ss_boundaries;
	}

	// to create dssp from pose
	std::string dssp_str_from_pose(core::pose::Pose pose){
		std::string dssp_str;
		protocols::moves::DsspMover dssp_mover ;
		dssp_mover = protocols::moves::DsspMover();
		dssp_mover.apply(pose);
		return pose.secstruct();
	}	

	//create edge information from secondary structure string
	std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size>> 
	secondary_structure_string_to_edge_information(std::string ss_string){
		utility::vector1< std::pair< core::Size, core::Size > > ss_span ; 
		utility::vector1<core::Size> edge_start_residues ;
		utility::vector1<core::Size> edge_end_residues ;
		utility::vector1<core::Size> edge_types ;

		ss_span = identify_secondary_structure_spans(ss_string);

		// add terminal peptide edges
		core::Size previous_end_idx = 0 ;
		core::Size first_ss_cut_point ; 
		core::Size ss_cut_point ;
		core::Size jump_edge_iter = 1 ;
		for ( core::Size ii = 1; ii <= ss_span.size(); ++ii ) {
			core::Size start_idx = ss_span[ii].first;
			core::Size end_idx = ss_span[ii].second;

			// extend to termini if first or last ss element
			if ( ii == 1 ) {
				start_idx = 1 ;
				first_ss_cut_point = (ss_span[1].first + ss_span[1].second) / 2 ;
			}

			if ( ii == ss_span.size() ) {
				end_idx = ss_string.size() ;
			}
			
			// to add loop between ss elements as peptide edges
			if ( ii > 1 && ii <= ss_span.size() && previous_end_idx + 1 < start_idx) {
				core::Size loop_start_idx = previous_end_idx + 1 ;
				core::Size loop_end_idx = start_idx - 1 ;
				ss_cut_point = (loop_start_idx + loop_end_idx) / 2 ;

				// add jump edge
				edge_start_residues.push_back( first_ss_cut_point );
				edge_end_residues.push_back( ss_cut_point );
				edge_types.push_back( jump_edge_iter );
				jump_edge_iter ++ ; 

				// add peptide edges
				edge_start_residues.push_back( ss_cut_point );
				edge_end_residues.push_back( loop_start_idx );
				edge_types.push_back( core::kinematics::Edge::PEPTIDE );

				edge_start_residues.push_back( ss_cut_point );
				edge_end_residues.push_back( loop_end_idx );
				edge_types.push_back( core::kinematics::Edge::PEPTIDE );

			}

			// secondary structure
			ss_cut_point = (ss_span[ii].first + ss_span[ii].second) / 2 ;
			if ( ii > 1 ) {
				edge_start_residues.push_back( first_ss_cut_point );
				edge_end_residues.push_back( ss_cut_point );
				edge_types.push_back( jump_edge_iter );
				jump_edge_iter ++;
			}
			
			// add peptide edges
			edge_start_residues.push_back( ss_cut_point );
			edge_end_residues.push_back( start_idx );
			edge_types.push_back( core::kinematics::Edge::PEPTIDE );

			edge_start_residues.push_back( ss_cut_point );
			edge_end_residues.push_back( end_idx );
			edge_types.push_back( core::kinematics::Edge::PEPTIDE );

			previous_end_idx = end_idx ;
		}

		return std::make_tuple(
			edge_start_residues, 
			edge_end_residues, 
			edge_types);
	}


	// to create a FoldTree from edge information
	core::kinematics::FoldTree fold_tree_from_edge_info(
		std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > edge_info
	){
		static basic::Tracer  TR("test.protocols.bootcamp.FoldTreeFromSSTests fold_tree_from_edge_info");
		core::kinematics::FoldTree fold_tree = core::kinematics::FoldTree();
		utility::vector1<core::Size> const & edge_start_residues = std::get<0>(edge_info);
		utility::vector1<core::Size> const & edge_end_residues = std::get<1>(edge_info);
		utility::vector1<core::Size> const & edge_types = std::get<2>(edge_info);

		TS_ASSERT( edge_start_residues.size() == edge_end_residues.size() );
		TS_ASSERT( edge_types.size() == edge_end_residues.size() );
		for ( core::Size ii = 1; ii <= edge_start_residues.size(); ++ii ) {
			fold_tree.add_edge( edge_start_residues[ ii ], edge_end_residues[ ii ], edge_types[ ii ] );
		}
		std::cout << "edges added " << std::endl;

		return fold_tree;
	}

	// pose to FoldTree
	core::kinematics::FoldTree pose_to_fold_tree( core::pose::Pose const & pose ){
		// get DSSP string from pose
		std::string dssp_str = dssp_str_from_pose(pose) ;
		std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > 
 edge_info = secondary_structure_string_to_edge_information(dssp_str) ;

		// create FoldTree from edge information
		core::kinematics::FoldTree fold_tree = fold_tree_from_edge_info(edge_info);

		return fold_tree ;
	}	

	void test_secondary_structure_spans(){
		static basic::Tracer  TR("test.protocols.bootcamp.FoldTreeFromSSTests test_secondary_structure_spans");
		std::cout << "test_secondary_structure_spans" << std::endl;
		std::string test_str = "HHHHHHH   HHHHHHHHHHHH      HHHHHHHHHHHHEEEEEEEEEEHHHHHHH EEEEHHH ";
		utility::vector1< std::pair< core::Size, core::Size > > SS_vector ;

		utility::vector1<int> answer_vector_first = utility::vector1<int> {1,11,29,41,51,59,63};
		utility::vector1<int> answer_vector_second = utility::vector1<int> {7,22,40,50,57,62,65};

		SS_vector = identify_secondary_structure_spans(test_str) ;
        	for ( core::Size ii = 1; ii <= SS_vector.size(); ++ii ) {
				TS_ASSERT( static_cast<int>(SS_vector[ ii ].first) == static_cast<int>(answer_vector_first[ ii ]) );
				TS_ASSERT( static_cast<int>(SS_vector[ ii ].second) == static_cast<int>(answer_vector_second[ ii ]) );
        }
	}

	void test_get_residue_edge(){
		std::cout << "[func] test_get_residue_edge" << std::endl;
		std::string dssp_string = "   EEEEEEE    EEEEEEE         EEEEEEEEE    EEEEEEEEEE   HHHHHH         EEEEEEEEE         EEEEE     ";
		std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > 
 edge_info = secondary_structure_string_to_edge_information(dssp_string) ;
		std::cout << "[func] test_get_residue_edge / edge_info " << std::endl;

		core::kinematics::FoldTree fold_tree = fold_tree_from_edge_info(edge_info);
		std::cout << fold_tree.to_string() << std::endl;

		std::cout << "[func] test_get_residue_edge / fold_tree " << std::endl;

		for ( core::Size resi = 1; resi <= dssp_string.size(); ++resi ) {
			if (resi == fold_tree.root()) continue;
			core::kinematics::Edge const & edge = fold_tree.get_residue_edge(resi);
			std::cout << "residue " << resi << " is connected by edge: "
				<< edge.start() << " to " << edge.stop()
				<< " of type " << edge.label() << std::endl;
		}
		TS_ASSERT( fold_tree.check_fold_tree() );
	}

	void test_from_pose_to_fold_tree(){
		std::cout << "[func] test_from_pose_to_fold_tree" << std::endl;
		core::pose::Pose pose = create_test_in_pdb_pose();
		// core::pose::Pose pose = test::util::create_test_in_pdb_pose();
		core::kinematics::FoldTree fold_tree = pose_to_fold_tree(pose);
		TS_ASSERT( fold_tree.check_fold_tree() );
	}
};