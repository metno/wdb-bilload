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


/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup bilLoad
 * @{
 */
/**
 * @file
 * Implementation of BilLoadConfiguration class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "BilLoadConfiguration.h"
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;
using namespace wdb;

// Support Functions
namespace
{

/**
 * Define the BilLoad options
 * @param	out		Reference to option structure
 */
options_description
getBil( BilLoadConfiguration::BilOptions & out )
{
    options_description bilload("BIL Loading");
    bilload.add_options()
    ( "byteorder", value<char>( & out.byteOrder )->default_value( 'I' ), "Byte order of the BIL file. Currently, only intel byte order (I) is supported" )
	( "ncols", value<int>( & out.nCols )->default_value( 1195 ), "Number of columns in the BIL file" )
	( "nrows", value<int>( & out.nRows )->default_value( 1550 ), "Number of rows in the BIL file" )
	( "nbits", value<int>( & out.nBits )->default_value( 16 ), "Number of bits in a value (only 8, 16, and 32 bits allowed)" )
    ;

    return bilload;
}

} // namespace support functions


BilLoadConfiguration::BilLoadConfiguration(const std::string & defaultDataProvider)
	: LoaderConfiguration( defaultDataProvider )
{
	shownOptions().add( getBil( bilOptions_ ) );
	cmdOptions().add( getBil( bilOptions_ ) );
}

BilLoadConfiguration::~BilLoadConfiguration()
{
	// NOOP
}


void
BilLoadConfiguration::parse_( int argc, char ** argv )
{
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( command_line_parser( argc, argv ).
				options( opt ).
				positional( positionalOptions_ ).
				run(),
			givenOptions_ );
}


/**
 * @}
 *
 * @}
 */

