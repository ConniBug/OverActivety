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
	IP_t() {
		setIpAddress("N/A");
	}

	IP_t(std::string ip) {
		setIpAddress(ip);
	}

	std::string getIpAddress() {
		return ip_address;
	}
	void setIpAddress(std::string new_ip) {
		std::regex regexp("^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$");
		if (std::regex_match(new_ip, regexp)) {
			ip_address = new_ip;
		}
	}
	std::string ip_address;
};

struct serverRecord_t {
	serverRecord_t(std::string name_2, IP_t ip_2) {
		name = name_2;
		ip = ip_2;
	}
	std::string name;
	IP_t ip;
};

struct serverRecords_t {
	std::vector<serverRecord_t> servers;

	serverRecords_t() {
		std::vector<serverRecord_t> tmp;
		serverRecord_t tmp3 = serverRecord_t("test", IP_t("127.0.0.1"));
		tmp.push_back(tmp3);
		servers = tmp;
	}


	void addNew(serverRecord_t serv) {
		servers.push_back(serv);
	}
	void addNew(std::string ip, std::string name) {
		serverRecord_t tmp = serverRecord_t(name, IP_t(ip));

 		servers.push_back(tmp);
	}
};

serverRecords_t* servers = new serverRecords_t();

namespace palate {
	const float pal_1_r = 0.92;
	const float pal_1_g = 0.34;
	const float pal_1_b = 0.56;
	uint64_t pal_2 = 0x3c4b7b;
	uint64_t pal_3 = 0xffe4f5;
	uint64_t pal_4 = 0xea9dc0;
	uint64_t pal_5 = 0xeb5890;

}