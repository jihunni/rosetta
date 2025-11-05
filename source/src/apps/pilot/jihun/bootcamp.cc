// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington UW TechTransfer, email: license@u.washington.edu.

#include <iostream>
#include <basic/Tracer.hh>
#include <basic/options/option.hh>
#include <basic/options/keys/in.OptionKeys.gen.hh>
#include <devel/init.hh>
#include <utility/pointer/owning_ptr.hh>
#include <core/pose/Pose.hh>
#include <core/import_pose/import_pose.hh>
#include <core/scoring/ScoreFunctionFactory.hh>
#include <core/scoring/ScoreFunction.hh>
#include <core/scoring/Energies.hh>
#include <numeric/random/random.fwd.hh>
#include <protocols/moves/MonteCarlo.fwd.hh>
#include <protocols/moves/MonteCarlo.hh>
#include <core/pack/pack_rotamers.hh>
#include <core/pack/task/TaskFactory.hh>
#include <core/pack/task/PackerTask.hh>
#include <core/kinematics/MoveMap.hh>
#include <core/optimization/MinimizerOptions.hh>
#include <core/optimization/AtomTreeMinimizer.hh>


int main( int argc, char ** argv) {
	std::cout << "Hello World!" << std::endl;

	devel::init( argc, argv );
	utility::vector1< std::string > filenames = basic::options::option[ basic::options::OptionKeys::in::file::s ].value();
	if ( filenames.size() > 0 ) {
	std::cout << "You entered: " << filenames[ 1 ] << " as the PDB file to be read" << std::endl;
	} else {
		std::cout << "You didn’t provide a PDB file with the -in::file::s option" << std::endl;
		return 1;
	}
	
	core::pose::PoseOP pose = core::import_pose::pose_from_file( filenames[1] );
	//core::pose::PoseOP pose = core::import_pose::import_pose::pose_from_file( pdb )
	core::scoring::ScoreFunctionOP sfxn = core::scoring::get_score_function() ; 
	core::Real score = sfxn->score( *pose );
	std::cout << "Score: " << score << std::endl ;

	
	core::Size randres = static_cast< core::Size > ( numeric::random::uniform() * pose->total_residue() + 1 );
	core::Real pert1 = numeric::random::uniform() ;
	core::Real pert2 = numeric::random::uniform() ;
	core::Real orig_phi = pose->phi( randres );
	core::Real orig_psi = pose->psi( randres );
	pose->set_phi( randres, orig_phi + pert1 );
	pose->set_psi( randres, orig_psi + pert2 );
	core::Real temperature = 273+25;

	protocols::moves::MonteCarlo monteCarlo = protocols::moves::MonteCarlo(*pose, *sfxn, temperature) ;
	//protocols::moves::MonteCarloOP monteCarlo = protocols::moves::MonteCarlo(*pose, *sfxn, temperature) ;
	
	int max_iter = 20;
	bool boltzmann_bool = false ;
	int acceptance_rate_report_total_counter = 0 ;
	int acceptance_rate_report_accept_counter = 0 ;
	const int acceptance_rate_report_freq = 5 ;
	core::Real pose_energy_sum = 0 ;
	//utility::vector1<acceptance_rate_report_freq> pose_energy_list 
	core::Real acceptance_rate = -1 ;
	core::pose::PoseOP pose_prev = pose;
	for (int i=0 ; i <= max_iter ; i++){
		std::cout << "current iteration: " << i << std::endl;
		// random perturbation
		core::Size randres = static_cast< core::Size > ( numeric::random::uniform() * pose->total_residue() + 1 );
        	core::Real pert1 = numeric::random::uniform() ;
        	core::Real pert2 = numeric::random::uniform() ;
        	core::Real orig_phi = pose->phi( randres );
        	core::Real orig_psi = pose->psi( randres );
        	pose->set_phi( randres, orig_phi + pert1 );
        	pose->set_psi( randres, orig_psi + pert2 );
		
		score = sfxn->score( *pose );
		boltzmann_bool = monteCarlo.boltzmann(*pose, score, "unk", 1, 0) ;
		if (boltzmann_bool == false) { 
			// reject : back to original init_pose
			pose = pose_prev ;
		}
		else {
			// accept
			acceptance_rate_report_accept_counter ++ ;
		}
		acceptance_rate_report_total_counter ++ ;
		pose_energy_sum += (pose->energies()).total_energy();

		core::pack::task::PackerTaskOP repack_task = core::pack::task::TaskFactory::create_packer_task( *pose );
		//repack_task->restrict_to_repacking();
		(*repack_task).restrict_to_repacking();
		core::pack::pack_rotamers( *pose, *sfxn, repack_task );

		// minimization
		core::kinematics::MoveMap mm;
		mm.set_bb( true );
		mm.set_chi( true );
		
		core::optimization::MinimizerOptions min_opts( "lbfgs_armijo_atol", 0.01, true );
		core::optimization::AtomTreeMinimizer atm;
		atm.run( *pose, mm, *sfxn, min_opts );

		std::cout<<  i % acceptance_rate_report_freq << std::endl;
		if ( i % acceptance_rate_report_freq  == 0) {
			acceptance_rate = static_cast<core::Real>(acceptance_rate_report_accept_counter) / static_cast<core::Real>(acceptance_rate_report_total_counter);
			std::cout << "[acceptance_rate] : " << acceptance_rate  << std::endl ;;
			std::cout << "Pose average energy : " << pose_energy_sum / acceptance_rate_report_total_counter << std::endl;
			pose_energy_sum = 0 ; 
		}
	}
	return 0;
} 



