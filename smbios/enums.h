#pragma once

// Source: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.7.0.pdf

#ifndef SMBIOS_ENUMS_H
#define SMBIOS_ENUMS_H

#include <cstdint>

namespace smbios {

	enum class Baseboard_FeatureFlags : uint8_t {
		hosting_board = 1 << 0,
		requires_daughter_or_aux_card = 1 << 1,
		removable = 1 << 2,
		replacable = 1 << 3,
		hot_swappable = 1 << 4
	};		
		
	enum class Baseboard_BoardTypes : uint8_t {
		unknown = 0x01,
		other = 0x02,
		server_blade = 0x03,
		connectivity_switch = 0x04,
		system_management_module = 0x05,
		processor_module = 0x06,
		io_module = 0x07,
		memory_module = 0x08,
		daughter_board = 0x09,
		motherboard = 0x0A,
		processor_memory_module = 0x0B,
		processor_io_module = 0x0C,
		interconnect_board = 0x0D
	};
		
	enum class TpmCharacteristics : uint64_t {
		// bits 0:1 = reserved
		charateristics_not_supported = 1 << 2,
		configurable_by_fw_update = 1 << 3,
		configurable_by_platform = 1 << 4,
		configurable_by_oem = 1 << 5
		// bits 6:63 = reserved
	};
} // namespace smbios

#endif // SMBIOS_ENUMS_H