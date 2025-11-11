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

// basic headers
#include <basic/options/option.hh>
#include <basic/options/keys/in.OptionKeys.gen.hh>
#include <basic/Tracer.hh>
#include <devel/init.hh>
#include <utility/pointer/owning_ptr.hh>

/// Project headers
#include <core/types.hh>
#include <core/import_pose/import_pose.hh>
#include <core/pose/Pose.fwd.hh>
#include <core/pose/Pose.hh>
#include <core/pose/variant_util.hh>
#include <core/scoring/ScoreFunctionFactory.hh>
#include <core/scoring/ScoreFunction.hh>
#include <core/kinematics/FoldTree.fwd.hh>
#include <core/kinematics/FoldTree.hh>
#include <core/kinematics/Edge.fwd.hh>
#include <core/kinematics/Edge.hh>

#include <protocols/moves/DsspMover.fwd.hh>
#include <protocols/moves/DsspMover.hh>

static basic::Tracer TR( "protocols.bootcamp.FoldTreeFromSS" );

FoldTreeFromSS::FoldTreeFromSS( std::string const & ss_string ) {
    // create FoldTree from secondary structure string
    std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > 
    edge_info = secondary_structure_string_to_edge_information(ssstring) ;
    ft_ = fold_tree_from_edge_info(edge_info);
}

utility::vector1< std::pair< core::Size, core::Size > > 
FoldTreeFromSS::identify_secondary_structure_spans( std::string const & ss_string )
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
std::string 
FoldTreeFromSS::dssp_str_from_pose(core::pose::Pose pose){
    std::string dssp_str;
    protocols::moves::DsspMover dssp_mover ;
    dssp_mover = protocols::moves::DsspMover();
    dssp_mover.apply(pose);
    return pose.secstruct();
}	

//create edge information from secondary structure string
std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size>> 
FoldTreeFromSS::secondary_structure_string_to_edge_information(std::string ss_string){
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
core::kinematics::FoldTree 
FoldTreeFromSS::fold_tree_from_edge_info(
    std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > edge_info
){
    core::kinematics::FoldTree fold_tree = core::kinematics::FoldTree();
    utility::vector1<core::Size> const & edge_start_residues = std::get<0>(edge_info);
    utility::vector1<core::Size> const & edge_end_residues = std::get<1>(edge_info);
    utility::vector1<core::Size> const & edge_types = std::get<2>(edge_info);

    TR( edge_start_residues.size() == edge_end_residues.size() );
    TR( edge_types.size() == edge_end_residues.size() );
    for ( core::Size ii = 1; ii <= edge_start_residues.size(); ++ii ) {
        fold_tree.add_edge( edge_start_residues[ ii ], edge_end_residues[ ii ], edge_types[ ii ] );
    }

    return fold_tree;
}

// pose to FoldTree
core::kinematics::FoldTree 
FoldTreeFromSS::pose_to_fold_tree( core::pose::Pose const & pose ){
    // get DSSP string from pose
    std::string dssp_str = dssp_str_from_pose(pose) ;
    std::tuple<utility::vector1< core::Size >, utility::vector1< core::Size>, utility::vector1< core::Size> > 
    edge_info = secondary_structure_string_to_edge_information(dssp_str) ;

    // create FoldTree from edge information
    core::kinematics::FoldTree fold_tree = fold_tree_from_edge_info(edge_info);

    return fold_tree ;
}	

int main( int argc, char ** argv) {
	std::cout << "Rosetta Bootcamp Lab4 / FoldTree" << std::endl;

    // import pose
	devel::init( argc, argv );
	utility::vector1< std::string > filenames = basic::options::option[ basic::options::OptionKeys::in::file::s ].value();
	if ( filenames.size() > 0 ) {
	std::cout << "You entered: " << filenames[ 1 ] << " as the PDB file to be read" << std::endl;
	} else {
		std::cout << "You didn’t provide a PDB file with the -in::file::s option" << std::endl;
		return 1;
	}
	
    // scoring
	core::pose::PoseOP pose = core::import_pose::pose_from_file( filenames[1] );
	core::scoring::ScoreFunctionOP sfxn = core::scoring::get_score_function() ; 
	core::Real score = sfxn->score( *pose );
	std::cout << "Score: " << score << std::endl ;

    // FoldTree
    core::kinematics::FoldTree  fold_tree = pose_to_fold_tree(*pose) ;

    // adding the cutpoint variants (virtual atom) to your Pose (upper and lower-cutpoint variants of each basic amino acid types)
    core::pose::correctly_add_cutpoint_variants(*pose);

    // add the chainbreak term in scorefunction
    sfxn->set_weight( core::scoring::chainbreak, 1.0 );

	return 0;
} 