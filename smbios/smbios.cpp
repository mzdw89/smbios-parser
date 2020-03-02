#include "smbios.h"

#include <Windows.h>

namespace fi {
	namespace smbios {
		std::unordered_map< std::uint8_t, std::vector< detail::smbios_table_entry_t > > tables = { };

		void parse_smbios( ) {
			auto size = GetSystemFirmwareTable( 'RSMB', 0, nullptr, 0 );

			if ( !size )
				throw std::exception( "smbios::parse_smbios: failed to get SMBIOS table size" );

			void* buffer = malloc( size );

			if ( !buffer )
				throw std::exception( "smbios::parse_smbios: failed to allocate buffer" );

			if ( GetSystemFirmwareTable( 'RSMB', 0, buffer, size ) != size )
				throw std::exception( "smbios::parse_smbios: GetSystemFirmwareTable failed" );

			auto smbios = reinterpret_cast< smbios_t* >( buffer );

			if ( smbios->length <= sizeof( smbios_t ) )
				throw std::exception( "smbios::parse_smbios: SMBIOS table has no entries" );

			// First table
			auto table_header = reinterpret_cast< table_header_t* >( std::uintptr_t( buffer ) + sizeof( smbios_t ) );
			do {
				std::uint8_t* string_table = reinterpret_cast< std::uint8_t* >( std::uintptr_t( table_header ) + table_header->length );

				// Parse string table
				detail::table_string_container table_strings = { };
				auto string_table_size = parse_string_table( string_table, table_strings );

				// Copy the formatted section
				std::vector< std::uint8_t > formatted_section( reinterpret_cast< std::uint8_t* >( table_header ), 
					reinterpret_cast< std::uint8_t* >( table_header + table_header->length ) );

				// Add the table into our parsed tables
				// Some tables have multiple entries, which is why we use a container
				tables[ table_header->type ].push_back( detail::smbios_table_entry_t( formatted_section, table_strings ) );

				// Go to next entry
				table_header = reinterpret_cast< table_header_t* >( string_table + string_table_size );
			} while ( table_header->type != table_types::end_of_table || table_header->length != 4 );

			free( buffer );
		}
		
		std::uint8_t parse_string_table( std::uint8_t* string_table, detail::table_string_container& out ) {
			std::uint8_t size = 0;
			std::string current_string = "";
		
			for ( ;; ) {
				if ( *string_table )
					current_string += *string_table;
				else if ( !current_string.empty( ) ) {
					// String end is marked with a null byte
					out.push_back( current_string );
					current_string.clear( );
				}

				// End of structure is marked by 2x null bytes (end-of-string + additional null terminator)
				if ( ( *string_table | *( string_table + 1 ) ) == 0 )
					break;

				size++;
				string_table++;
			}

			return size + 2; // Account for the 2 extra bytes
		}
	} // namespace smbios
} // namespace fi