// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/bootcamp/BootCampMover.hh
/// @brief 2025_bootcamp_mover_subclass
/// @author Jihun (jeung@uw.edu)

#ifndef INCLUDED_protocols_bootcamp_BootCampMover_HH
#define INCLUDED_protocols_bootcamp_BootCampMover_HH

// Unit headers
#include <protocols/bootcamp/BootCampMover.fwd.hh>
#include <protocols/moves/Mover.fwd.hh>
#include <protocols/moves/Mover.hh>

// Protocol headers
#include <protocols/filters/Filter.fwd.hh>

// Core headers
#include <core/pose/Pose.fwd.hh>

// Basic/Utility headers
#include <basic/datacache/DataMap.fwd.hh>
//#include <utility/tag/XMLSchemaGeneration.fwd.hh> //transcluded from Mover

#include <basic/citation_manager/UnpublishedModuleInfo.fwd.hh>

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


namespace protocols {
namespace bootcamp {

///@brief 2025_bootcamp_mover_subclass
class BootCampMover : public protocols::moves::Mover {

public:

	/////////////////////
	/// Constructors  ///
	/////////////////////

	/// @brief Default constructor
	BootCampMover();

	/// @brief Destructor (important for properly forward-declaring smart-pointer members)
	~BootCampMover() override;


public:

	/////////////////////
	/// Mover Methods ///
	/////////////////////


	/// @brief Apply the mover
	void
	apply( core::pose::Pose & pose ) override;

	void
	show( std::ostream & output = std::cout ) const override;


public:

	///////////////////////////////
	/// Rosetta Scripts Support ///
	///////////////////////////////

	/// @brief parse XML tag (to use this Mover in Rosetta Scripts)
	void
	parse_score_function(
		utility::tag::TagCOP tag,
		basic::datacache::DataMap & data );
	
	void
	parse_my_tag(
		utility::tag::TagCOP tag,
		basic::datacache::DataMap & data ) override;

	//BootCampMover & operator=( BootCampMover const & src );

	/// @brief required in the context of the parser/scripting scheme
	protocols::moves::MoverOP
	fresh_instance() const override;

	/// @brief required in the context of the parser/scripting scheme
	protocols::moves::MoverOP
	clone() const override;

	std::string
	get_name() const override;

	static
	std::string
	mover_name();

	static
	void
	provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd );

public: //Function overrides needed for the citation manager:

	/// @brief This mover is unpublished.  It returns Jihun as its author.
	// void provide_citation_info(basic::citation_manager::CitationCollectionList & citations) const override;

private: // methods

	core::scoring::ScoreFunctionOP 
	get_score_function() const;

	void 
	set_score_function( core::scoring::ScoreFunctionOP sfxn ) ;

	core::Size 
	get_num_iterations() const ;

	void 
	set_num_iterations( core::Size num_iterations ) ;

private: // data
	core::scoring::ScoreFunctionOP sfxn_;
	core::Size num_iterations_;
};

std::ostream &
operator<<( std::ostream & os, BootCampMover const & mover );

} //bootcamp
} //protocols

#endif //protocols_bootcamp_BootCampMover_HH
