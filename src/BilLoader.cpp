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

#include "BilLoader.h"
#include "BilFile.h"
//#include <GridGeometry.h>
#include <wdbException.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <wdbLogHandler.h>
#include <algorithm>
#include <functional>
#include <cmath>

using namespace std;
using namespace wdb;
using namespace wdb::load;
using namespace boost::posix_time;

namespace bil
{

BilLoader::BilLoader( LoaderDatabaseConnection & connection,
					  const BilLoadConfiguration::LoadingOptions & loadingOptions,
					  wdb::WdbLogHandler & logHandler )
	: connection_(connection),
	  loadingOptions_(loadingOptions),
	  logHandler_(logHandler)
{
	// NOOP
}

BilLoader::~BilLoader()
{
	// NOOP
}

namespace
{
	std::string toString(const boost::posix_time::ptime & time )
	{
		if ( time == boost::posix_time::ptime(neg_infin) )
			return "1900-01-01";
		else if ( time == boost::posix_time::ptime(pos_infin) )
			return "2100-01-01";
		// ...always convert to zulu time
		std::string ret = to_iso_extended_string(time) + "+00";
		return ret;
	}
}

void BilLoader::load(const bil::BilFile & file)
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.bilload.load" );
	log.infoStream() << "Preparing to load the file";
	try
	{
	    std::vector<double> data;
	    getValues( data, file );
	    const std::string dataProviderName = dataProvider( file );
	    const std::string placeName = place( file );
	    connection_.write ( & data[0],
							data.size(),
							dataProviderName,
							placeName,
							referenceTime ( file ),
							validTimeFrom( file ),
							validTimeTo( file ),
							valueParameter( file ),
							levelParameter( file ),
							levelFrom( file ),
							levelTo( file ),
							dataVersion( file ),
							confidenceCode( file ) );
	}
	catch ( wdb::ignore_value &e)
	{
		log.infoStream() << e.what();
	}
	catch ( std::exception & e )
	{
		log.errorStream() << e.what();
	}
}

void BilLoader::getValues(std::vector<double> & out, const BilFile & file)
{
	file.readValues( out );
	// TODO: Swap Top to Bottom?
	// TODO: Change units of measure
	/*
	std::vector<double> rawData;
	field.grid(rawData);

	out.reserve(rawData.size());

	double scale = std::pow(double(10), double(field.scaleFactor()));
	std::string unit;
	valueparameter(field, unit);
	float coeff = 1.0, term = 0.0;
	connection_.readUnit( unit, &coeff, &term );

    if ( ( coeff != 1.0 )&&( term != 0.0) ) {
    	for ( int i=0; i<rawData.size(); i++ ) {
   			out.push_back( convertValue(rawData[i], scale, coeff, term) );
    	}
    }
    else {
    	std::transform( rawData.begin(), rawData.end(), std::back_inserter(out),
					    std::bind2nd(scale_value(), scale) );
    }
	gridToLeftLowerHorizontal( out, field );
	*/

	// Screen orientation
    int nI = file.nX();
    int nJ = file.nY();
    for ( int j = 1; j <= nJ / 2; j ++ ) {
      for ( int i = 0; i < nI; i ++ ) {
	swap( out[((nJ - j) * nI) + i], out[((j - 1) * nI) + i] );
      }
    }


}

/*
 * The following code is duplicated in LoaderDatabaseConnection.
 * For dataProvider and place, the code is duplicated to ensure that
 * errors are thrown if these options are omitted.
 * In other cases, the information is required in case the filenames
 * do not contain the relevant "encoding".
 */

std::string BilLoader::dataProvider(const BilFile & file)
{
	std::string ret = loadingOptions_.dataProvider;
	if ( ret.empty() )
		throw std::invalid_argument("BIL files do not contain encoded data provider information. Data provider must be defined on the command line");
	return ret;
}

std::string BilLoader::place(const BilFile & file)
{
	std::string ret = loadingOptions_.placeName;
	if ( ret.empty() )
		throw std::invalid_argument("BIL files do not contain encoded place information. Place definition must be defined on the command line");
	return ret;
}

std::string BilLoader::referenceTime(const BilFile & file)
{
	std::string ret = loadingOptions_.referenceTime;
	if ( ret.empty() ) {
		// Default - 0600 UTC on the date given by the file
		ret = file.getDate() + " 06:00:00 UTC";
	}
	return ret;
}

std::string BilLoader::validTimeFrom(const BilFile & file)
{
	std::string ts = loadingOptions_.referenceTime;
	if ( ts.empty() ) {
		ts = file.getDate() + " 06:00:00";
	}
	ptime t( time_from_string( ts ) );
	// Default - 24 hours prior to the reference time
	ptime retTime = t - hours(24);
    string ret = to_iso_extended_string(retTime) + " UTC";
	return ret;
}

std::string BilLoader::validTimeTo(const BilFile & file)
{
	std::string ret = loadingOptions_.referenceTime;
	if ( ret.empty() ) {
		// Default - 0600 UTC on the date given by the file
		ret = file.getDate() + " 06:00:00 UTC";
	}
	return ret;
}

std::string BilLoader::valueParameter(const BilFile & file)
{
	std::string ret = loadingOptions_.valueParameter;
	if ( ret.empty() ) {
		std::string hdr = file.getHead();
		if ( hdr == "tm") {
			return "air temperature";
		}
		else if (hdr == "rr") {
			return "precipitation amount";
		}
		else
			throw std::invalid_argument("The parameter could not be decoded from the file name");
	}
	return ret;
}

std::string BilLoader::levelParameter(const BilFile & file)
{
	std::string ret = loadingOptions_.levelParameter;
	if ( ret.empty() ) {
		return "height above ground";
	}
	return ret;
}
float BilLoader::levelFrom(const BilFile & file)
{
	return 2.0;
}
float BilLoader::levelTo(const BilFile & file)
{
	return 2.0;
}

int BilLoader::dataVersion(const BilFile & file)
{
	return loadingOptions_.dataVersion;
}

int BilLoader::confidenceCode(const BilFile & file)
{
	return loadingOptions_.confidenceCode;
}

}
