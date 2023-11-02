#pragma once

// Source: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.3.0.pdf

#ifndef SMBIOS_STRUCTS_H
#define SMBIOS_STRUCTS_H

#include "enums.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

namespace smbios {
	using byte = std::uint8_t;
	using word = std::uint16_t;
	using dword = std::uint32_t;
	using qword = std::uint64_t;

#pragma pack(push, 1)
	struct SMBios
	{
		byte calling_method;
		byte major_version;
		byte minor_verion;
		byte dmi_revision;
		dword length;
	};

	struct TableHeader
	{
		byte type;
		byte length;
		word handle;
	};
		
	struct BiosInformation : public TableHeader
	{
		byte id_vendor;
		byte id_bios_version;
		word bios_starting_address_segment;
		byte id_bios_release_date;
		byte bios_rom_size;
		qword bios_charateristics;
		word bios_characteristics_extensions;
		byte bios_major_release;
		byte bios_minor_release;
		byte embedded_controller_fw_major_release;
		byte embedded_controler_fw_minor_release;
		word extended_bios_rom_size;
	};

	struct SMBiosUUID
	{
		dword time_low;
		word time_mid;
		word time_hi_and_version;
		byte clock_seq_hi_and_reversed;
		byte clock_seq_low;
		byte node[6];
	};

	struct SystemInformation : public TableHeader
	{
		byte id_manufacturer;
		byte id_product_name;
		byte id_version;
		byte id_serial_number;
		SMBiosUUID uuid;
		byte wake_up_type;
		byte id_sku_number;
		byte id_family;
	};

	struct BaseboardInformation : public TableHeader
	{
		byte id_manufacturer;
		byte id_product;
		byte id_version;
		byte id_serial_number;
		byte id_asset_tag;
		Baseboard_FeatureFlags	feature_flags;
		byte id_location_in_chassis;
		word chassis_handle;
		Baseboard_BoardTypes board_type;
		byte num_object_handles;
		word object_handles[];
	};

	struct MemoryDevice : public TableHeader
	{
		word physical_memory_array_handle;
		word memory_error_information_handle;
		word total_width;
		word data_width;
		word size;
		byte form_factor;
		byte device_set;
		byte id_device_locator;
		byte id_bank_locator;
		byte memory_type;
		word type_detail;
		word speed;
		byte id_manufacturer;
		byte id_serial_number;
		byte id_asset_tag;
		byte id_part_number;
		byte attributes;
		dword extended_size;
		word configured_memory_speed;
		word minimum_voltage;
		word maximum_voltage;
		word configured_voltage;
		byte memory_technology;
		word memory_operating_mode_capability;
		byte id_firmware_version;
		word module_manufacturer_id;
		word module_product_id;
		word memory_subsystem;
		word memory_subsystem_controller_product_id;
		qword non_volatile_size;
		qword volatile_size;
		qword cache_size;
		qword logical_size;
		dword extended_speed;
		dword extended_configured_memory_speed;
	};
		
	struct TpmDevice : public TableHeader
	{
		byte vendor_id [4];
		byte major_spec_version;
		byte minor_spec_version;
		dword fw_version_1;
		dword fw_version_2;
		byte id_description;
		TpmCharacteristics tpm_characteristics;
		dword vendor_specific_information;
	};
#pragma pack(pop)

	// use old enum 
	enum TableTypes : byte
	{
		bios_information = 0,
		system_information = 1,
		baseboard_information = 2,
		memory_device = 17,
		tpm_device = 43,
		end_of_table = 127 // Has a lenght of 4
	};

	namespace detail
	{
		using entry_handle = word;
		using table_string_container = std::unordered_map<byte, std::string>;

		struct smbios_table_entry
		{
			smbios_table_entry() noexcept { }

			smbios_table_entry(
				TableHeader* const table_header, 
				entry_handle e_handle, 
				const table_string_container& _table_strings) 
				: strings(_table_strings), 
				handle(e_handle) 
			{ 
				formatted_section.assign(
					reinterpret_cast<byte*>(table_header), 
					reinterpret_cast<byte*>(table_header + table_header->length));
			}
			entry_handle handle = 0;
			table_string_container strings = { };
			std::vector<byte> formatted_section = { };
		};

		template <typename T> 
		class smbios_table
		{
		public:
			smbios_table() noexcept { }
			smbios_table(const smbios_table_entry& entry) : m_entry(entry) { }

			std::string operator[](int index)
			{
				return m_entry.strings[index];
			}

			const T* operator->() const noexcept
			{
				return reinterpret_cast<const T*>(m_entry.formatted_section.data());
			}

		private:
			smbios_table_entry m_entry = { };
		};

	} // namespace detail
} // namespace smbios

#endif // SMBIOS_STRUCT_H