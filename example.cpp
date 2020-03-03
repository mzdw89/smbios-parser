#include "smbios/smbios.h"
#include <iostream>

namespace smbios = fi::smbios;

int main( ) {
	try {
		smbios::smbios_parser smbios;

		printf( "Your SMBIOS version is %.1f\n\n", smbios.get_version( ) );

		smbios.enum_tables( [ &smbios ]( std::uint8_t table_type, std::uint8_t* const formatted_section, smbios::detail::table_string_container& table_strings ) {
			auto version = smbios.get_version( );

			switch ( table_type ) {
				case smbios::table_types::system_information:
				{
					printf( "System Information:\n" );

					auto sys_info = reinterpret_cast< smbios::system_information_t* >( formatted_section );

					if ( version >= 2 ) {
						printf( "Manufacturer: %s\n", table_strings[ sys_info->id_manufacturer ].data( ) );
						printf( "Product Name: %s\n", table_strings[ sys_info->id_product_name ].data( ) );
						printf( "Version: %s\n", table_strings[ sys_info->id_version ].data( ) );
						printf( "Serial Number: %s\n", table_strings[ sys_info->id_serial_number ].data( ) );
					}
					
					if ( version >= 2.4 ) {
						printf( "SKU Number: %s\n", table_strings[ sys_info->id_sku_number ].data( ) );
						printf( "Family: %s\n", table_strings[ sys_info->id_family ].data( ) );
					}

					break;
				}
			}
		} );
	} catch ( const std::exception& e ) {
		std::cout << e.what( ) << std::endl;
		std::cin.get( );
		
		return 1;
	}

	return 0;
}