#pragma once

#ifndef SMBIOS_H
#define SMBIOS_H

#include "structs.h"

#include <unordered_map>

namespace fi {
	namespace smbios {
		extern std::unordered_map< std::uint8_t, std::vector< detail::smbios_table_entry_t > > tables;

		void parse_smbios( );

		std::uint8_t parse_string_table( std::uint8_t* string_table, detail::table_string_container& out );
	} // namespace smbios
} // namespace fi

#endif // SMBIOS_STRUCT_H