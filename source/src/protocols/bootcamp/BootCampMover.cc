// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/bootcamp/BootCampMover.cc
/// @brief 2025_bootcamp_mover_subclass
/// @author Jihun (jeung@uw.edu)

// Unit headers
#include <protocols/bootcamp/BootCampMover.hh>
#include <protocols/bootcamp/BootCampMoverCreator.hh>

// Core headers
#include <core/pose/Pose.hh>

// Basic/Utility headers
#include <basic/Tracer.hh>
#include <utility/tag/Tag.hh>
#include <utility/pointer/memory.hh>

// XSD Includes
#include <utility/tag/XMLSchemaGeneration.hh>
#include <protocols/moves/mover_schemas.hh>

// Citation Manager
#include <utility/vector1.hh>
#include <basic/citation_manager/UnpublishedModuleInfo.hh>

// C++ headers
#include <iostream>

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


// static basic::Tracer TR( "protocols.bootcamp.BootCampMover" );

namespace protocols {
namespace bootcamp {
	/////////////////////
	/// Constructors  ///
	/////////////////////

/// @brief Default constructor
BootCampMover::BootCampMover():
	protocols::moves::Mover( BootCampMover::mover_name() )
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Destructor (important for properly forward-declaring smart-pointer members)
BootCampMover::~BootCampMover(){}

////////////////////////////////////////////////////////////////////////////////
	/// Mover Methods ///
	/////////////////////

/// @brief Apply the mover
void
BootCampMover::apply( core::pose::Pose& ){

}

core::scoring::ScoreFunctionOP 
BootCampMover::get_score_function() const { 
	return sfxn_; 
}

void 
BootCampMover::set_score_function( core::scoring::ScoreFunctionOP sfxn ) { 
	runtime_assert( sfxn != nullptr );
	sfxn_ = sfxn;
}

core::Size 
BootCampMover::get_num_iterations() const { 
	return num_iterations_; 
}

void 
BootCampMover::set_num_iterations( core::Size num_iterations ) { 
	num_iterations_ = num_iterations; 
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Show the contents of the Mover
void
BootCampMover::show(std::ostream & output) const
{
	protocols::moves::Mover::show(output);
}

////////////////////////////////////////////////////////////////////////////////
	/// Rosetta Scripts Support ///
	///////////////////////////////

/// @brief parse XML tag (to use this Mover in Rosetta Scripts)
void
BootCampMover::parse_score_function(
	utility::tag::TagCOP tag,
	basic::datacache::DataMap& data
) {
	core::scoring::ScoreFunctionOP new_score_function(
		core::scoring::parse_score_function(tag, data)
	);
	if ( new_score_function == nullptr ) return;
	BootCampMover::set_score_function( new_score_function );
}

void
BootCampMover::parse_my_tag(
	utility::tag::TagCOP const tag,
	basic::datacache::DataMap& datamap
) {
	core::Size nloop_;
	if ( tag->hasOption("nloop") ) {
		nloop_ = tag->getOption<core::Size>("nloop",1);
		runtime_assert( nloop_ > 0 );
	}
	else { nloop_ = 1; }

	BootCampMover::set_num_iterations( nloop_ );
	BootCampMover::parse_score_function( tag, datamap );
	// BootCampMover::parse_task_operations( tag, datamap );
}
void BootCampMover::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd )
{

	using namespace utility::tag;
	AttributeList attlist;

	//here you should write code to describe the XML Schema for the class.  If it has only attributes, simply fill the probided AttributeList.

	protocols::moves::xsd_type_definition_w_attributes( xsd, mover_name(), "2025_bootcamp_mover_subclass", attlist );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief required in the context of the parser/scripting scheme
protocols::moves::MoverOP
BootCampMover::fresh_instance() const
{
	
	return utility::pointer::make_shared< BootCampMover >();
}

/// @brief required in the context of the parser/scripting scheme
protocols::moves::MoverOP
BootCampMover::clone() const
{
	return utility::pointer::make_shared< BootCampMover >( *this );
}

std::string BootCampMover::get_name() const {
	return mover_name();
}

std::string BootCampMover::mover_name() {
	return "BootCampMover";
}

// void 
// BootCampMover::provide_citation_info(basic::citation_manager::CitationCollectionList & citations) override{
// 	using namespace basic::citation_manager;
// 	CitationCollectionOP citation(
// 		utility::pointer::make_shared< CitationCollection >(
// 		"SetupMetalsMover",
// 		CitedModuleType::Mover
// 		)
// 	);
// 	citation->add_citation( CitationManager::get_instance()->get_citation_by_doi("10.1073/pnas.2012800118") );
// 	citations.add( citation );
// 	citations.add( metal_selector_ );
// 	citations.add( contact_selector_ );
// }

/////////////// Creator ///////////////
protocols::moves::MoverOP
BootCampMoverCreator::create_mover() const
{
	return utility::pointer::make_shared< BootCampMover >();
}

std::string
BootCampMoverCreator::keyname() const
{
	return BootCampMover::mover_name();
}

void BootCampMoverCreator::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) const
{
	BootCampMover::provide_xml_schema( xsd );
}


} //bootcamp
} //protocols
