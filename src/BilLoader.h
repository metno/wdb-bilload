/*
 wdb

 Copyright (C) 2009 met.no

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

#ifndef FELTLOADER_H_
#define FELTLOADER_H_

//#include <wdb/WdbLevel.h>
#include "BilLoadConfiguration.h"
#include <wdb/LoaderDatabaseConnection.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>

namespace wdb
{
class WdbLogHandler;
}

namespace bil
{
class BilFile;

class BilLoader
{
public:
	/** Constructor
	 * @param	connection		Database connection
	 * @param	loadingOptions	The options used in loading files
	 * @param	logHandler		Reference to WDB logging
	 */
	BilLoader( wdb::database::LoaderDatabaseConnection & connection,
			   const  wdb::BilLoadConfiguration::LoadingOptions & loadingOptions,
			   wdb::WdbLogHandler & logHandler );
	/**
	 * Destructor
	 */
	~BilLoader();

	/** Load a BIL file
	 * @param	bilFile		Reference to the BIL file object to be loaded
	 */
	void load( const BilFile & bilFile );

private:
	/** Retrieve and process the values from the BIL file
	 * @param	out		Reference to double array containing the data values
	 * @param	file	The BIL file to extract data from
	 */
	void getValues(std::vector<double> & out, const BilFile & file);
	/** Retrieve the data provider information for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A dataprovider name string
	 */
	std::string dataProvider( const BilFile & file );
	/** Retrieve the place information for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A place name string
	 */
	std::string place( const BilFile & file );
	/** Retrieve the reference time for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A reference time string
	 */
	std::string referenceTime( const BilFile & file );
	/** Retrieve the beginning valid time information for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A valid time string
	 */
	std::string validTimeFrom( const BilFile & file );
	/** Retrieve the end valid time for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A valid time string
	 */
	std::string validTimeTo( const BilFile & file );
	/** Retrieve the value parameter for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A value parameter name
	 */
	std::string valueParameter( const BilFile & file );
	/** Retrieve the level parameter for the file
	 * @param	file	The BIL file to extract data from
	 * @returns			A level parameter name
	 */
	std::string levelParameter( const BilFile & file );
	/** Retrieve the lower level of the data
	 * @param	file	The BIL file to extract data from
	 * @returns			The level from
	 */
	float levelFrom( const BilFile & file );
	/** Retrieve the upper level of the data
	 * @param	file	The BIL file to extract data from
	 * @returns			The level to
	 */
	float levelTo( const BilFile & file );
	/** Retrieve the dataVersion the data
	 * @param	file	The BIL file to extract data from
	 * @returns			The data version
	 */
	int dataVersion( const BilFile & file );
	/** Retrieve the confidence code of the data
	 * @param	file	The BIL file to extract data from
	 * @returns			The confidence code
	 */
	int confidenceCode( const BilFile & file );

	/// Configuration Map
	//	std::map<felt::word, long int> felt2wdbDataProvider_;
	/// Reference to database connection
	wdb::database::LoaderDatabaseConnection & connection_;
	/// Reference to the loading options
	const wdb::LoaderConfiguration::LoadingOptions & loadingOptions_;
	/// Reference to the log handler
	wdb::WdbLogHandler & logHandler_;
};

}

#endif /*FELTLOADER_H_*/
