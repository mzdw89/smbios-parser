#pragma once

#ifndef SMBIOS_H
#define SMBIOS_H

#include "structs.h"

#include <Windows.h>

#include <unordered_map>
#include <functional>
#include <algorithm>

namespace fi {
	namespace smbios {
		class smbios_parser {
		public:
			void enum_tables(std::function< void(std::uint8_t table_type, detail::entry_handle entry_handle) > enum_fn) {
				for (auto table_entries : m_tables) { for (auto type_table : table_entries.second) { enum_fn(table_entries.first, type_table.handle); } }
			}

			double get_version() { return m_smbios_version; }

			template < typename T >
			detail::smbios_table< T > get_table_by_handle(detail::entry_handle handle) {
				for (auto& table_type : m_tables) {
					for (auto& table_entry : table_type.second) {
						if (table_entry.handle == handle)
							return detail::smbios_table< T >(table_entry);
					}
				}

				return { };
			}

		private:
			void parse_smbios()
			{
				auto size = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);

				if (!size)
					throw std::exception("smbios::parse_smbios: failed to get SMBIOS table size");

				void* buffer = malloc(size);

				if (!buffer)
					throw std::exception("smbios::parse_smbios: failed to allocate buffer");

				if (GetSystemFirmwareTable('RSMB', 0, buffer, size) != size)
					throw std::exception("smbios::parse_smbios: GetSystemFirmwareTable failed");

				auto smbios = reinterpret_cast<smbios_t*>(buffer);

				save_smbios_version(smbios);

				if (smbios->length <= sizeof(smbios_t))
					throw std::exception("smbios::parse_smbios: SMBIOS table has no entries");

				// First table
				detail::entry_handle handle = 0;
				auto table_header = reinterpret_cast<table_header_t*>(std::uintptr_t(buffer) + sizeof(smbios_t));
				do
				{
					std::uint8_t* string_table = reinterpret_cast<std::uint8_t*>(std::uintptr_t(table_header) + table_header->length);

					// Parse string table
					detail::table_string_container table_strings = { };
					auto string_table_size = parse_string_table(string_table, table_strings);

					// Add the table into our parsed tables
					// Some tables have multiple entries, which is why we use a container
					m_tables[table_header->type].emplace_back(table_header, handle, table_strings);

					// Go to next entry
					table_header = reinterpret_cast<table_header_t*>(string_table + string_table_size);

					// Increase the handle
					handle++;
				} while (table_header->type != table_types::end_of_table || table_header->length != 4);

				free(buffer);
			}

			void save_smbios_version(smbios_t* smbios)
			{
				char version_string[8] = { 0 };
				snprintf(version_string, std::size(version_string), "%i.%i", smbios->major_version, smbios->minor_verion);

				m_smbios_version = std::stod(version_string);
			}

			std::uint8_t parse_string_table(std::uint8_t* string_table, detail::table_string_container& out)
			{
				std::uint8_t size = 0;
				std::uint8_t string_count = 1;

				for (; ; size++, string_table++)
				{
					if (*string_table) 
						out[string_count].push_back(*string_table);
					else // String end is marked with a null byte
						string_count++; 

					// End of structure is marked by 2x null bytes (end-of-string + additional null terminator)
					if ((*string_table | *(string_table + 1)) == 0) break;
				}

				return size + 2; // Account for the 2 extra bytes
			}

			double m_smbios_version = 0;

			std::unordered_map< std::uint8_t, std::vector< detail::smbios_table_entry_t > > m_tables = { };
		};
	} // namespace smbios
} // namespace fi

#endif // SMBIOS_STRUCT_H