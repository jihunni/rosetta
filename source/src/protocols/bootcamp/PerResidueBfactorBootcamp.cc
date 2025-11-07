// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file protocols/bootcamp/PerResidueBfactorBootcamp.cc
/// @brief Simple Metric class for pLDDT value extraction from AF2
/// @author Jihun (jeung@uw.edu)

// Unit headers
#include <protocols/bootcamp/PerResidueBfactorBootcamp.hh>
#include <protocols/bootcamp/PerResidueBfactorBootcampCreator.hh>

// Core headers
#include <core/simple_metrics/PerResidueRealMetric.hh>
#include <core/simple_metrics/util.hh>

#include <core/select/residue_selector/ResidueSelector.hh>
#include <core/select/residue_selector/util.hh>
#include <core/select/util.hh>

// Basic/Utility headers
#include <basic/Tracer.hh>
#include <basic/datacache/DataMap.hh>
#include <utility/tag/Tag.hh>
#include <utility/string_util.hh>
#include <utility/pointer/memory.hh>

// XSD Includes
#include <utility/tag/XMLSchemaGeneration.hh>
#include <basic/citation_manager/UnpublishedModuleInfo.hh>
#include <basic/citation_manager/CitationCollection.hh>

#ifdef    SERIALIZATION
// Utility serialization headers
#include <utility/serialization/serialization.hh>

// Cereal headers
#include <cereal/types/polymorphic.hpp>
#endif // SERIALIZATION

static basic::Tracer TR( "protocols.bootcamp.PerResidueBfactorBootcamp" );


namespace protocols {
namespace bootcamp {

using namespace core::select;
using namespace core::select::residue_selector;

	/////////////////////
	/// Constructors  ///
	/////////////////////

/// @brief Default constructor
PerResidueBfactorBootcamp::PerResidueBfactorBootcamp():
	core::simple_metrics::PerResidueRealMetric()
{}

////////////////////////////////////////////////////////////////////////////////
/// @brief Destructor (important for properly forward-declaring smart-pointer members)
PerResidueBfactorBootcamp::~PerResidueBfactorBootcamp(){}

core::simple_metrics::SimpleMetricOP
PerResidueBfactorBootcamp::clone() const {
	return utility::pointer::make_shared< PerResidueBfactorBootcamp >( *this );
}

std::string
PerResidueBfactorBootcamp::name() const {
	return name_static();
}

std::string
PerResidueBfactorBootcamp::name_static() {
	return "PerResidueBfactorBootcamp";

}
std::string
PerResidueBfactorBootcamp::metric() const {

	return "SHORT_NAME_FOR_SCOREFILE_HEADER_DEFAULT";
}

void
PerResidueBfactorBootcamp::parse_my_tag(
		utility::tag::TagCOP tag,
		basic::datacache::DataMap & datamap)
{

	SimpleMetric::parse_base_tag( tag );
	PerResidueRealMetric::parse_per_residue_tag( tag, datamap );


	if (tag->hasOption("atom_type ")){
		return;
	}
}

void
PerResidueBfactorBootcamp::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) {
	using namespace utility::tag;
	using namespace core::select::residue_selector;

	AttributeList attlist;
	attlist + XMLSchemaAttribute::attribute_w_default("atom_type", xs_string, "Atom type for pLDDT", "CA");
	attributes_for_parse_residue_selector( attlist, "residue_selector",
		"Selector specifying residues." );

	std::string description = "Simple Metric class for pLDDT value extraction from AF2";
	core::simple_metrics::xsd_per_residue_real_metric_type_definition_w_attributes(xsd, name_static(),
	description, attlist);
}

std::map< core::Size, core::Real >
PerResidueBfactorBootcamp::calculate(const core::pose::Pose & pose) const {
	utility::vector1< core::Size > selection = selection_positions(get_selector()->apply(pose));

	std::map< core::Size, core::Real > b_fact_map;
	for ( core::Size resi_index : selection ) {	
		if ( pose.residue_type(resi_index).has( atom_type_ ) ) {
			b_fact_map[resi_index] = pose.pdb_info()->bfactor(resi_index, pose.residue_type(resi_index).atom_index(atom_type_));
		}
	}

	return b_fact_map;
}

/// @brief This simple metric is unpublished.  It returns Jihun as its author.
void
PerResidueBfactorBootcamp::provide_citation_info( basic::citation_manager::CitationCollectionList & citations ) const {
	citations.add(
		utility::pointer::make_shared< basic::citation_manager::UnpublishedModuleInfo >(
		"PerResidueBfactorBootcamp", basic::citation_manager::CitedModuleType::SimpleMetric,
		"Jihun",
		"TODO: institution",
		"jeung@uw.edu",
		"Wrote the PerResidueBfactorBootcamp."
		)
	);
}

void
PerResidueBfactorBootcampCreator::provide_xml_schema( utility::tag::XMLSchemaDefinition & xsd ) const {
	PerResidueBfactorBootcamp::provide_xml_schema( xsd );
}

std::string
PerResidueBfactorBootcampCreator::keyname() const {
	return PerResidueBfactorBootcamp::name_static();
}

core::simple_metrics::SimpleMetricOP
PerResidueBfactorBootcampCreator::create_simple_metric() const {
	return utility::pointer::make_shared< PerResidueBfactorBootcamp >();
}

} //bootcamp
} //protocols


#ifdef    SERIALIZATION



template< class Archive >
void
protocols::bootcamp::PerResidueBfactorBootcamp::save( Archive & arc ) const {
	arc( cereal::base_class< core::simple_metrics::PerResidueRealMetric>( this ) );
	//arc( CEREAL_NVP( output_as_pdb_nums_ ) );

}

template< class Archive >
void
protocols::bootcamp::PerResidueBfactorBootcamp::load( Archive & arc ) {
	arc( cereal::base_class< core::simple_metrics::PerResidueRealMetric >( this ) );
	//arc( output_as_pdb_nums_ );


}

SAVE_AND_LOAD_SERIALIZABLE( protocols::bootcamp::PerResidueBfactorBootcamp );
CEREAL_REGISTER_TYPE( protocols::bootcamp::PerResidueBfactorBootcamp )

CEREAL_REGISTER_DYNAMIC_INIT( protocols_bootcamp_PerResidueBfactorBootcamp )
#endif // SERIALIZATION




