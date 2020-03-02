# smbios-parser
A small C++ library which helps you read the SMBIOS table.

# Example
```c++
smbios::parse_smbios();

smbios::system_information_t* sys_info = reinterpret_cast< smbios::system_information_t* >
( smbios::tables[ smbios::table_types::system_information ][ 0 ].formatted_section.data( ) );
```


# Notes
Not all table structures are implemented. If you'd like to add a structure, check the
official specification of SMBIOS.

# License
This project is licensed under the MIT License.