#pragma once
#include <string>
#include <regex>
#include <iostream>
#include "logging.h"
#include "structs.h"

enum class loggingLevel {
	DEFAULT_t,
	WARNING_t,
	ERROR_t,
	DEBUGGING_t
};

struct IP_t {
	std::string getIpAddress() {
		return ip_address;
	}
	std::string setIpAddress(std::string new_ip) {
		std::regex regexp("^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$");
		if (std::regex_match(new_ip, regexp)) {
			ip_address = new_ip;
		}
	}
	std::string ip_address;
};

struct serverRecord_t {
	std::string name;
	IP_t ip;
};


namespace palate {
	const float pal_1_r = 0.92;
	const float pal_1_g = 0.34;
	const float pal_1_b = 0.56;
	uint64_t pal_2 = 0x3c4b7b;
	uint64_t pal_3 = 0xffe4f5;
	uint64_t pal_4 = 0xea9dc0;
	uint64_t pal_5 = 0xeb5890;

}