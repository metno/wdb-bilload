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

#ifndef BILFILE_H_
#define BILFILE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iterator>
#include <vector>
#include <iosfwd>
#include "BilLoadConfiguration.h"

namespace bil
{

class BilFile
{
public:
	/** Constructor
	 * @param	file		The file name and path of the BIL file
	 */
	explicit BilFile( const boost::filesystem::path & file,
					  const wdb::BilLoadConfiguration::BilOptions & bilOptions );
	/// Destructor
	~BilFile();

	/** Return information about the BIL file
	 * @returns 	Formatted string containing information about the BIL file
	 */
	std::string information() const;
	/** Read the data values from the BIL file
	 * @param	out		Reference to double array containing the data values
	 */
	void readValues( std::vector<double> & out ) const;
	/** Read the head (parameter) information from file name
     * @returns			A std::string
	 */
	std::string getHead() const;
	/** Read date information from file name
     * @returns			A date as a std::string
	 */
	std::string getDate() const;

private:
	/// File name of the BIL file
	const boost::filesystem::path fileName_;
	/// File stream
	boost::filesystem::ifstream * bilFile_;
	/// X dimension of grid
	int nX_;
	/// Y dimension of grid
	int nY_;
	/// Number of Bit
	int nBits_;

};

}
#endif /*BILFILE_H_*/
