#include "smbios/smbios.h"
#include <iostream>

namespace smbios = fi::smbios;

int main( ) {
	try {
		smbios::smbios_parser smbios;

		printf( "Your SMBIOS version is %.1f\n\n", smbios.get_version( ) );

		smbios.enum_tables( [ &smbios ]( std::uint8_t table_type, smbios::detail::entry_handle entry_handle ) {
			auto version = smbios.get_version( );

			switch ( table_type ) {
				case smbios::table_types::bios_information:
				{
					printf( "[BIOS Information]\n" );

					// Get the struct
					auto bios_info = smbios.get_table_by_handle< smbios::bios_information_t >( entry_handle );

					// Check the version
					if ( version >= 2 ) {
						// Access the strings like you would in an array
						// Members are being accessed as if sys_info were a pointer
						printf( "Vendor: %s\n", bios_info[ bios_info->id_vendor ].data( ) );
						printf( "BIOS Version: %s\n", bios_info[ bios_info->id_bios_version ].data( ) );
						printf( "BIOS Release Date: %s\n", bios_info[ bios_info->id_bios_release_date ].data( ) );
					}

					printf("\n");
					break;
				}
				case smbios::table_types::system_information:
				{
					printf( "[System Information]\n" );

					// Get the struct
					auto sys_info = smbios.get_table_by_handle< smbios::system_information_t >( entry_handle );

					// Check the version
					if ( version >= 2 ) {
						// Access the strings like you would in an array
						// Members are being accessed as if sys_info were a pointer
						printf( "Manufacturer: %s\n", sys_info[ sys_info->id_manufacturer ].data( ) );
						printf( "Product Name: %s\n", sys_info[ sys_info->id_product_name ].data( ) );
						printf( "Version: %s\n", sys_info[ sys_info->id_version ].data( ) );
						printf( "Serial Number: %s\n", sys_info[ sys_info->id_serial_number ].data( ) );
					}
					
					if ( version >= 2.4 ) {
						printf( "SKU Number: %s\n", sys_info[ sys_info->id_sku_number ].data( ) );
						printf( "Family: %s\n", sys_info[ sys_info->id_family ].data( ) );
					}

					printf("\n");
					break;
				}
				case smbios::table_types::baseboard_information:
				{
					printf( "[Baseboard Information]\n" );

					// Get the struct
					auto board_info = smbios.get_table_by_handle< smbios::baseboard_information_t >( entry_handle );

					// Check the version
					if ( version >= 2 ) {
						printf( "Manufacturer: %s\n", board_info[ board_info->id_manufacturer ].data( ) );
						printf( "Product: %s\n", board_info[ board_info->id_product ].data( ) );
						printf( "Version: %s\n", board_info[ board_info->id_version ].data( ) );
						printf( "Serial Number: %s\n", board_info[ board_info->id_serial_number ].data( ) );
						printf( "Asset Tag: %s\n", board_info[ board_info->id_asset_tag ].data( ) );
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