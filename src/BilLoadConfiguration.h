/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


#ifndef BILLOADCONFIGURATION_H_
#define BILLOADCONFIGURATION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup bilLoad
 * @{
 */

/**
 * @file
 * This file contains the definition of the basic loading options
 *
 */

// PROJECT INCLUDES
#include <wdb/LoaderConfiguration.h>

// SYSTEM INCLUDES
#include <string>
#include <boost/program_options/positional_options.hpp>

// FORWARD REFERENCES
//

namespace wdb
{

/**
 * BilLoadConfiguration handles the options of the application.
 *
 * @see LoaderConfiguration
 */
class BilLoadConfiguration : public load::LoaderConfiguration
{
public:
	// LIFECYCLE
	/**
	 * Default Constructor
	 */
	explicit BilLoadConfiguration( const std::string & defaultDataProvider );
	/**
	 * Default Destructor
	 */
	virtual ~BilLoadConfiguration( );

	/** Container for bil file options
	 */
	struct BilOptions
	{
		char byteOrder;
		int nRows;
		int nCols;
		int nBits;
	};

	/**	Get the input options
	 * @return 	The input options
	 */
	const BilOptions & bilOptions() const { return bilOptions_; }

protected:
	using WdbConfiguration::parse_;
	/**
	 * Parse arguments from a given file
	 * @param	argc		Number of command line arguments
	 * @param	argv		The actual command line arguments
	 */
	virtual void parse_( int argc, char ** argv );

private:
	/// Input options
	BilOptions bilOptions_;
};

}		// namespace wdb

/**
 * @}
 *
 * @}
 */


#endif /*LOADERCONFIGURATION_H_*/
