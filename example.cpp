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
					
					printf("\n");
					break;					
				}
				case smbios::table_types::tpm_device:
				{
					printf( "[TPM Device]\n" );

					auto tpm_info = smbios.get_table_by_handle< smbios::tpm_device_t >( entry_handle );
					printf( "Vendor ID: %c%c%c%c\n", tpm_info->vendor_id[0], tpm_info->vendor_id[1], tpm_info->vendor_id[2], tpm_info->vendor_id[3] );
					printf( "TPM Version: %i.%i\n", tpm_info->major_spec_version, tpm_info->minor_spec_version );
					if ( version >= 2 ) {
						printf( "Description: %s\n", tpm_info[ tpm_info->id_description ].data( ) );
					}
					printf("\n");
					break;
				}
				case smbios::table_types::memory_device:
				{
					auto mem_device = smbios.get_table_by_handle< smbios::memory_device_t >( entry_handle );
					
					if (mem_device->size > 0) { // skip empty banks
						printf( "[Memory Device]\n" );

						printf( "Size: %d\n", mem_device->size );
						if ( version >= 2.1 ) {
							printf( "Device Locator: %s\n", mem_device[ mem_device->id_device_locator ].data( ) );
							printf( "Bank Locator: %s\n", mem_device[ mem_device->id_bank_locator ].data( ) );
						}
						if ( version >= 2.3 ) {
							printf( "Manufacturer: %s\n", mem_device[ mem_device->id_manufacturer ].data( ) );
							printf( "Serial Number: %s\n", mem_device[ mem_device->id_serial_number ].data( ) );
							printf( "Asset Tag: %s\n", mem_device[ mem_device->id_asset_tag ].data( ) );
							printf( "Part Number: %s\n", mem_device[ mem_device->id_part_number ].data( ) );
						}
						if ( version >= 3.2 ) {
							printf( "Firmware Version: %s\n", mem_device[ mem_device->id_firmware_version ].data( ) );
						}
						printf("\n");
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