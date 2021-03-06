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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "BilLoadConfiguration.h"
#include "BilFile.h"
#include "BilLoader.h"
//#include "test/FakeDatabaseConnection.h"
//#include "FeltLoadConfiguration.h"
#include <wdb/LoaderDatabaseConnection.h>
#include <wdbLogHandler.h>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace bil;

// Support Functions
namespace
{

/**
 * Write the program version to stream
 * @param	out		Stream to write to
 */
void version( ostream & out )
{
    out << PACKAGE_STRING << endl;
}

/**
 * Write help information to stram
 * @param	options		Description of the program options
 * @param	out			Stream to write to
 */
void help( const boost::program_options::options_description & options, ostream & out )
{
	version( out );
	out << '\n';
    out << "Usage: "PACKAGE_NAME" [OPTIONS] FILES...\n\n";
    out << options << endl;
}
} // namespace


int main(int argc, char ** argv)
{
	wdb::BilLoadConfiguration conf("wdb_bil");
	try
    {
    	conf.parse( argc, argv );
    	if ( conf.general().help )
    	{
    		help( conf.shownOptions(), cout );
    		return 0;
    	}
    	if ( conf.general().version )
    	{
    		version( cout );
    		return 0;
    	}
    }
    catch( exception & e ) {
        cerr << e.what() << endl;
        help( conf.shownOptions(), clog );
        return 1;
    }

	wdb::WdbLogHandler logHandler( conf.logging().loglevel, conf.logging().logfile );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.bilload.main" );
    log.debug( "Starting bilLoad" );

    const std::vector<std::string> & file = conf.input().file;
	std::vector<boost::filesystem::path> files;
	copy( file.begin(), file.end(), back_inserter(files) );
	try
	{
		if ( conf.output().list )
		{
			log.fatalStream() << "Data listing not implemented for BIL file loading";
			return 1;
		}
		else
		{

		    log.debugStream() << "Attempting to connect to database " << conf.pqDatabaseConnection();
		   	wdb::load::LoaderDatabaseConnection connection( conf );
		    log.debugStream() << "...connected";

		    BilLoader loader( connection, conf.loading(), logHandler );
			for ( std::vector<boost::filesystem::path>::const_iterator it = files.begin(); it != files.end(); ++ it )
			{
				try
				{
					loader.load( bil::BilFile(*it, conf.bilOptions() ) );
				}
				catch ( std::exception & e )
				{
					log.errorStream() << "Unable to load file " << it->native_file_string();
					log.errorStream() << "Reason: " << e.what();
				}
			}
		}
	}
	catch ( std::exception & e )
	{
		log.fatalStream() << e.what();
		return 1;
	}
}
