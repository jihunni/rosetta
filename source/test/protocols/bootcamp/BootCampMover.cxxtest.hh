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

#include <test/core/init_util.hh>
#include <test/util/pose_funcs.hh>
#include <test/util/pose_funcs.hh>
#include <test/util/rosettascripts.hh>

/// Project headers
#include <basic/Tracer.hh>
#include <basic/datacache/DataMap.fwd.hh>
#include <basic/datacache/DataMap.hh>
#include <basic/citation_manager/UnpublishedModuleInfo.hh>

#include <utility/vector1.hh>
#include <utility/tag/XMLSchemaGeneration.fwd.hh>
#include <utility/tag/XMLSchemaGeneration.hh>
#include <utility/tag/Tag.fwd.hh>
#include <utility/tag/Tag.hh>
#include <utility/pointer/owning_ptr.hh>

#include <core/types.hh>
#include <core/scoring/ScoreFunction.fwd.hh>
#include <core/scoring/ScoreFunction.hh>
#include <core/scoring/xml_util.hh>

#include <protocols/moves/Mover.fwd.hh>
#include <protocols/moves/Mover.hh>
#include <protocols/moves/MoverFactory.fwd.hh>
#include <protocols/moves/MoverFactory.hh>
#include <protocols/moves/mover_schemas.hh>
#include <protocols/bootcamp/BootCampMover.hh>

// --------------- Test Class --------------- //
static basic::Tracer  TR("test.protocols.bootcamp.BootCampMover");

class BootCampMoverTest : public CxxTest::TestSuite {

public: 
	protocols::bootcamp::BootCampMoverOP bcm_creator;
	protocols::moves::MoverFactoryOP mover_factory_op;
	protocols::moves::MoverOP base_mover_op;
	protocols::moves::MoverOP bcm_op;


	// Shared initialization goes here.
	void setUp() {
		std::cout << "Setup starts." << std::endl;

		// register the mover creator with the mover factory

 
		// auto tag = std::make_shared<utility::tag::Tag>();
		// tag->setName(name);

		// basic::datacache::DataMap data_map;

		base_mover_op = (protocols::moves::MoverFactory::get_instance()) -> newMover(bcm_creator.keyname());
		// mover_factory_op->factory_register( BootCampMoverCreator() );
		// base_mover_op = mover_factory_op->newMover(BootCampMoverCreator::keyname());
		bcm_op = utility::pointer::dynamic_pointer_cast< protocols::bootcamp::BootCampMover > ( base_mover_op );

		// core_init();
		
		// bcm_op = mover_creator.create_mover();

		// using namespace protocols::jd2;

		// Set up a job outputter that writes a scorefile and no PDBs and no Silent Files.
		// PDBJobOutputterOP jobout( new PDBJobOutputter );


		// MoverFactory -> Mover
		// JobDis -> inst. Mover
		// Mover -> BCM


		// If the user chooses something else, then so be it, but by default score(_jd2) should only create a score
		// file and nothing else.
		// protocols::jd2::JobDistributor::get_instance()->go( mymover );

		// protocols::jd3::JobDistributorOP jd = protocols::jd3::JobDistributorFactory::create_job_distributor();
		


		// mover_factory = protocols::moves::MoverFactory();
		// mover_factory.factory_register(mover_creator)
		// bcm_op = mover_factory.newMover(BootCampMoverCreator::mover_name());
		
		// owning pointer (wrapping a raw pointer)
		// bcm_op = utility::pointer::dynamic_pointer_cast< BootCampMover > ( base_mover_op );
		// bcm_op->fresh_instance();
			// Dynamic cast : to downcast a pointer to a base class (e.g. Mover) to a pointer to a derived class (e.g. BootCampMover). 
			// The owning pointer’s operator() method, also called its functor, returns this raw pointer. 
			// base_mover_op : owning pointer 
			// bcm_op : raw pointer
		
		//base_mover_op->newMover(BootCampMoverCreator::mover_name());
		std::cout << "Setup completed." << std::endl;
	}

	// Shared finalization goes here.
	void tearDown() {
	};

	void test_mover_creation(){
			protocols::moves::MoverOP bcm_op;
		TS_ASSERT( bcm_op != nullptr );
	};

	void test_add_private_members_on_BootCampMover(){
		// bcm_op->set_num_iterations(5);
		// TS_ASSERT( bcm_op->get_num_iterations() == 5 );
		protocols::bootcamp::BootCampMover bcm_mover;
		bcm_mover.set_num_iterations(10);
		TS_ASSERT( bcm_mover.get_num_iterations() == 10 ); 

		// bcm_op->set_score_function( core::scoring::get_score_function() );
		// TS_ASSERT( bcm_op->get_score_function() != nullptr );
	};

	void test_tag_parser_on_BootCampMover(){
		protocols::bootcamp::BootCampMover bcm_mover;
		basic::datacache::DataMap data;
		utility::tag::TagCOP tag = tagptr_from_string("<MyTest name=test scorefxn=beta_nov16 nloop=1>\n" "</MyTest>");
		core::scoring::ScoreFunctionOP sfxn = core::scoring::ScoreFunctionOP( new core::scoring::ScoreFunction );
		data.add( "scorefxns" , "beta_nov16", sfxn );
		bcm_mover.parse_score_function(tag, data);

		TS_ASSERT_EQUALS(sfxn, bcm_mover.get_score_function() );
	};
};