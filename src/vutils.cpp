#include "vutils.h"

#include <iostream>
#include <sstream>
#include <cstring>

#include <iomanip>

std::string make_readable_format(const uchar* buffer, int size, bool bhex)
{
	std::ostringstream ss;
	const int column_size = 8;
	const int line_size = column_size * 2;
	const std::string& line = make_line(); 

	ss << "|";
		
	for(int i = 0; i < size; ++i)
	{
		const bool is_last_byte = (i + 1) % line_size == 0;
		const bool is_the_middle = !is_last_byte && (i + 1) % column_size == 0;

		if(bhex)
		{
			ss << std::hex;
			ss << std::setfill('0');
		}

		ss << std::setw(2);

		if(bhex)
		{
			ss << (uint)buffer[i];
		}
		else if(isalnum(buffer[i]))
		{
			ss << (char)buffer[i];
		}
		else
		{
			ss << ".";
		}

		if(!is_last_byte)
		{
			ss << " ";
		}
		else
		{
			ss << "|" << std::endl << "|";
		}

		if(is_the_middle)
		{
			ss << "  ";
		}
	}

	ss << std::endl;
	ss << line;

	return std::move(ss.str());
}

std::string make_line(int len)
{
	std::ostringstream ss;
	
	ss << "*";
	for(int i = 0; i < len; ++i)
	{
		ss << "-";
	}
	ss << "*\n";

	return std::move(ss.str());
}

std::string make_sentence(const char* prefix, uint data, bool bTopline)
{
	std::ostringstream ss;
	const int offset = 4;
	const std::string& line = make_line();

	if(bTopline)
	{
		ss << line;
	}

	const std::string& data_str = std::to_string(data);
	std::string prefix_str = std::string(prefix);
	prefix_str.append(data_str);

	ss << "|";
	ss << std::setw(VLINELENGTH / 2 + offset + 1) << prefix_str;
	ss << std::setw(VLINELENGTH / 2 - offset / 2)  << "|\n";
	ss << line;

	return std::move(ss.str());
}


std::string assemble_packet_info(const uchar* buffer, int size, int number)
{
	std::ostringstream ss;
	const std::string& str_hex = make_readable_format(buffer, size);
	const std::string& str_char = make_readable_format(buffer, size, false);
	const std::string& str_size = make_sentence("SIZE: ", size, false);
	const std::string& packet_num = make_sentence("RECEIVED PACKET: ", number);
	ss << packet_num << str_size << str_hex << str_char << std::endl << std::endl;

	return std::move(ss.str());
}
