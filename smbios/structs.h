#pragma once

// Source: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.7.1.pdf

#ifndef SMBIOS_STRUCTS_H
#define SMBIOS_STRUCTS_H

#include <cstdint>
#include <vector>
#include <string>

namespace fi {
	namespace smbios {
		struct smbios_t {
			std::uint8_t	calling_method;
			std::uint8_t	major_version;
			std::uint8_t	minor_verion;
			std::uint8_t	dmi_revision;
			std::uint32_t	length;
		};

		struct table_header_t {
			std::uint8_t	type;
			std::uint8_t	length;
			std::uint16_t	handle;
		};

		struct smbios_uuid_t {
			std::uint32_t	time_low;
			std::uint16_t	time_mid;
			std::uint16_t	time_hi_and_version;
			std::uint8_t	clock_seq_hi_and_reversed;
			std::uint8_t	clock_seq_low;
			std::uint8_t	node[ 6 ];
		};

		struct system_information_t : public table_header_t {
			std::uint8_t	id_manufacturer;
			std::uint8_t	id_product_name;
			std::uint8_t	id_version;
			std::uint8_t	id_serial_number;
			smbios_uuid_t	uuid;
			std::uint8_t	wake_up_type;
			std::uint8_t	id_sku_number;
		};

		enum table_types : std::uint8_t {
			system_information = 1,
			end_of_table = 127 // Has a lenght of 4
		};

		namespace detail {
			typedef std::vector< std::string > table_string_container;

			struct smbios_table_entry_t {
				smbios_table_entry_t( ) { }

				smbios_table_entry_t( const std::vector< std::uint8_t >& _formatted_section, const table_string_container& _table_strings ) :
					formatted_section( _formatted_section ), strings( _table_strings ) { }

				table_string_container strings = { };
				std::vector< std::uint8_t > formatted_section = { };
			};
		} // namespace detail
	} // namespace smbios
} // namespace fi

#endif // SMBIOS_STRUCT_H