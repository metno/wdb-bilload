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

#include "BilFile.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;
using namespace boost::filesystem;
using namespace wdb;

namespace bil
{

BilFile::BilFile( const path & file,
				  const BilLoadConfiguration::BilOptions & bilOptions )
	: fileName_(file)
{
	if ( ! exists(file) )
		throw runtime_error("Cannot find file " + file.native_file_string() );
	if ( is_directory(file) )
		throw runtime_error(file.native_directory_string() + " is a directory, not a file");
	string fileName = file.native_file_string();
	bilFile_ = new boost::filesystem::ifstream( file );
	nX_ = bilOptions.nCols;
	nY_ = bilOptions.nRows;
	nBits_ = bilOptions.nBits;
}

BilFile::~BilFile()
{
	delete bilFile_;
}

std::string BilFile::information() const
{
	/*
	std::ostringstream cont;
	cont << "File type\t\t" << block1_[0] << "\n";
	cont << "Time a\t\t\t" << lastUpdateTime() << "\n";
	cont << "Time b\t\t\t" << referenceTime() << "\n";
	cont << "M\t\t\t" << block1_[7] << "\n";
	cont << "N\t\t\t" << block1_[8] << "\n";
	cont << "K\t\t\t" << block1_[9] << "\n";
	cont << "L\t\t\t" << block1_[10] << "\n";
	cont << "MX\t\t\t" << block1_[11] << "\n";
	cont << "Last word last block\t" << block1_[12] << "\n";
	cont << "storage type\t\t" << block1_[13] << "\n";
	cont << "Update status\t\t" << block1_[14] << "\n";
	cont << "Arch time a\t\t" << firstTime() << "\n";
	cont << "Arch time b\t\t" << lastTime() << "\n";
	cont << "Termins\t\t\t" << block1_[25] << "\n";
	cont << "Indexes/term\t\t" << block1_[26] << "\n";
	cont << "Producer\t\t" << block1_[27] << "\n";
	cont << "Time unit\t\t" << block1_[28] << "\n";
	cont << "Time resolution\t\t" << block1_[29] << "\n";

	return cont.str();
	*/
	return "Not implemented yet!";
}

void BilFile::readValues( std::vector<double> & out ) const
{
	// Dimensions
	int noOfInts = nX_ * nY_;
	out.reserve( noOfInts );
	std::vector<double> rawData;
	//rawData.reserve( noOfInts );
	switch (nBits_) {
	case 8:
		unsigned char buf8[1];
		while ( bilFile_->read(  reinterpret_cast<char *>( buf8 ), 1 ) ) {
			double value;
			value =  static_cast<double>( buf8[0] );
			rawData.push_back( value );
		}
		break;
	case 16:
		unsigned char buf16[2];
		while ( bilFile_->read( reinterpret_cast<char *>( buf16 ), 2 ) ) {
			double value;
			value =  buf16[0] | ( buf16[1] << 8 );
			rawData.push_back( value );
		}
		break;
	case 32:
		unsigned char buf32[4];
		while ( bilFile_->read( reinterpret_cast<char *>( buf32 ), 4 ) ) {
			double value;
			value =  buf32[0] + ( buf32[1] << 8 ) + ( buf32[2] << 16 ) + ( buf32[3] << 24 );
			rawData.push_back( value );
		}
		break;
	default:
        throw std::invalid_argument("The specified number of bit per value is not supported");
	}
	/// Scale data
	for ( unsigned int i=0; i<rawData.size(); i++ ) {
		//if ((rawData[i] != 10000)&&((rawData[i] < 2716)||(rawData[i] > 2943)))
		//	cout << rawData[i] << ", ";
	  if ( rawData[i] != 10000.0 )
	    out.push_back( rawData[i] / 10.0 );
	  else
	    out.push_back( rawData[i] );
	}
}

std::string BilFile::getHead() const
{
	string fileName = fileName_.string();
	size_t endPath = fileName.find_last_of( '/' );
	size_t startExt = fileName.rfind( '.' );
	std::string rootFile = fileName.substr( endPath + 1, (startExt - endPath) - 1 );
	std::string head = rootFile.substr( 0, rootFile.find('_') );
	return head;
}

std::string BilFile::getDate() const
{
	string fileName = fileName_.string();
	size_t endPath = fileName.find_last_of( '/' );
	size_t startExt = fileName.rfind( '.' );
	std::string rootFile = fileName.substr( endPath + 1, (startExt - endPath) - 1 );
	std::string date = rootFile.substr( rootFile.find('_') + 1 );
	date[ date.find( '_' ) ] = '-';
	date[ date.find( '_' ) ] = '-';
	return date;
}

  int BilFile::nX() const
  {
    return nX_;
  }

  int BilFile::nY() const
  {
    return nY_;
  }

}
