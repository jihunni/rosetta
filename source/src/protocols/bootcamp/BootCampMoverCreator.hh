// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/bootcamp/BootCampMoverCreator.hh
/// @brief 2025_bootcamp_mover_subclass
/// @author Jihun (jeung@uw.edu)

#ifndef INCLUDED_protocols_bootcamp_BootCampMoverCreator_HH
#define INCLUDED_protocols_bootcamp_BootCampMoverCreator_HH

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



namespace protocols {
namespace bootcamp {

class BootCampMoverCreator : public protocols::moves::MoverCreator {
public:
	protocols::moves::MoverOP create_mover() const override;
	std::string keyname() const override;
	void provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) const override;
};

} //bootcamp
} //protocols

#endif //INCLUDED_protocols_bootcamp_BootCampMoverCreator_HH
