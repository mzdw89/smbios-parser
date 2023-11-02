#include "smbios/smbios.hpp"
#include <iostream>

void PrintBiosInfo(const double& version, smbios::detail::smbios_table<smbios::BiosInformation>& biosInfo)
{
	printf("[BIOS Information]\n");

	// Check the version
	if (version >= 2) {
		// Access the strings like you would in an array
		// Members are being accessed as if sys_info were a pointer
		printf("Vendor: %s\n", biosInfo[biosInfo->id_vendor].c_str());
		printf("BIOS Version: %s\n", biosInfo[biosInfo->id_bios_version].c_str());
		printf("BIOS Release Date: %s\n", biosInfo[biosInfo->id_bios_release_date].c_str());
	}

	printf("\n");
}


void PrintSystemInfo(const double& version, smbios::detail::smbios_table<smbios::SystemInformation>& sysInfo)
{
	printf("[System Information]\n");

	// Check the version
	if (version>= 2) {
		// Access the strings like you would in an array
		// Members are being accessed as if sys_info were a pointer
		printf("Manufacturer: %s\n", sysInfo[sysInfo->id_manufacturer].c_str());
		printf("Product Name: %s\n", sysInfo[sysInfo->id_product_name].c_str());
		printf("Version: %s\n", sysInfo[sysInfo->id_version].c_str());
		printf("Serial Number: %s\n", sysInfo[sysInfo->id_serial_number].c_str());
	}

	if (version>= 2.4) {
		printf("SKU Number: %s\n", sysInfo[sysInfo->id_sku_number].c_str());
		printf("Family: %s\n", sysInfo[sysInfo->id_family].c_str());
	}

	printf("\n");
}

void PrintBaseboardInfo(const double& version, smbios::detail::smbios_table<smbios::BaseboardInformation>& boardInfo)
{
	printf("[Baseboard Information]\n");

	// Check the version
	if (version>= 2) {
		printf("Manufacturer: %s\n", boardInfo[boardInfo->id_manufacturer].c_str());
		printf("Product: %s\n", boardInfo[boardInfo->id_product].c_str());
		printf("Version: %s\n", boardInfo[boardInfo->id_version].c_str());
		printf("Serial Number: %s\n", boardInfo[boardInfo->id_serial_number].c_str());
		printf("Asset Tag: %s\n", boardInfo[boardInfo->id_asset_tag].c_str());
	}

	printf("\n");
}

void PrintTpmInfo(const double& version, smbios::detail::smbios_table<smbios::TpmDevice>& tpmInfo)
{
	printf("[TPM Device]\n");
	printf("Vendor ID: %c%c%c%c\n", tpmInfo->vendor_id[0], tpmInfo->vendor_id[1], tpmInfo->vendor_id[2], tpmInfo->vendor_id[3]);
	printf("TPM Version: %i.%i\n", tpmInfo->major_spec_version, tpmInfo->minor_spec_version);
	if (version >= 2)
	{
		printf("Description: %s\n", tpmInfo[tpmInfo->id_description].c_str());
	}

	printf("\n");
}

void PrintMemDevices(const double& version, smbios::detail::smbios_table<smbios::MemoryDevice>& memDevice)
{
	printf("[Memory Device]\n");

	printf("Size: %d\n", memDevice->size);
	if (version >= 2.1)
	{
		printf("Device Locator: %s\n", memDevice[memDevice->id_device_locator].c_str());
		printf("Bank Locator: %s\n", memDevice[memDevice->id_bank_locator].c_str());
	}
	if (version >= 2.3)
	{
		printf("Manufacturer: %s\n", memDevice[memDevice->id_manufacturer].c_str());
		printf("Serial Number: %s\n", memDevice[memDevice->id_serial_number].c_str());
		printf("Asset Tag: %s\n", memDevice[memDevice->id_asset_tag].c_str());
		printf("Part Number: %s\n", memDevice[memDevice->id_part_number].c_str());
	}
	if (version >= 3.2)
	{
		printf("Firmware Version: %s\n", memDevice[memDevice->id_firmware_version].c_str());
	}

	printf("\n");
}

int main( )
{
	try
	{
		smbios::SMBiosParser smbios;
		smbios.Parse();

		printf("Your SMBIOS version is %.1f\n\n", smbios.GetVersion());

		smbios.EnumTables(
			[&smbios](std::uint8_t tableType, smbios::detail::entry_handle entryHandle)
		{
			const auto version = smbios.GetVersion();

			switch(tableType)
			{
				case (int)smbios::TableTypes::bios_information:
				{
					auto biosInfo = smbios.get_table_by_handle<smbios::BiosInformation>(entryHandle);
					PrintBiosInfo(version, biosInfo);
					break;
				}
				case (int)smbios::TableTypes::system_information:
				{
					auto sysInfo = smbios.get_table_by_handle<smbios::SystemInformation>(entryHandle);
					PrintSystemInfo(version, sysInfo);
					break;
				}
				case (int)smbios::TableTypes::baseboard_information:
				{
					auto boardInfo = smbios.get_table_by_handle<smbios::BaseboardInformation>(entryHandle);
					PrintBaseboardInfo(version, boardInfo);
					break;					
				}
				case (int)smbios::TableTypes::tpm_device:
				{
					auto tpmInfo = smbios.get_table_by_handle<smbios::TpmDevice>(entryHandle);
					PrintTpmInfo(version, tpmInfo);
					break;
				}
				case (int)smbios::TableTypes::memory_device:
				{
					auto memDevice = smbios.get_table_by_handle<smbios::MemoryDevice>(entryHandle);
					if (memDevice->size> 0) // skip empty banks
					{ 
						PrintMemDevices(version, memDevice);
					}					
					break;
				}
				default:
					break;
			}
		} );
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		
		return 1;
	}

	return 0;
}

