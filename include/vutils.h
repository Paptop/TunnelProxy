#ifndef VUTILS_H
#define VUTILS_H

#include <string>

typedef unsigned char uchar;
typedef unsigned int uint;

const int VLINELENGTH = 49;

std::string make_readable_format(const uchar* buffer, int size, bool bhex = true);
std::string make_line(int len = VLINELENGTH);
std::string make_sentence(const char* prefix, uint data, bool line = true);
std::string assemble_packet_info(const uchar* buffer, int size, int number);

#endif //VUTILS_H
