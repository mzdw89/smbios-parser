#include "smbios/smbios.h"
#include <iostream>

namespace smbios = fi::smbios;

int main( ) {
	try {
		smbios::parse_smbios( );

		// Prints all the strings in the SMBIOS table
		for ( auto& table_array : smbios::tables ) {
			auto table_type = table_array.first;
			auto table_entries = table_array.second;

			for ( int i = 0; i < table_entries.size( ); i++ ) {
				for ( auto& s : table_entries[ i ].strings )
					printf( "%i.%i: %s\n", table_type, i, s.data( ) );
			}
		}

	} catch ( const std::exception& e ) {
		std::cout << e.what( ) << std::endl;
		std::cin.get( );

		return 1;
	}

	return 0;
}