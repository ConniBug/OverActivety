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
	struct cachedData_t {
		cachedData_t(std::string name_t = "N/A",
			std::string wanIP_t = "N/A",
			std::string lanIP_t = "N/A",
			std::string hostname_t = "N/A",
			std::string uptime_t = "N/A",
			std::string cpuUsage_t = "N/A",
			std::string memoryUsage_t = "N/A",
			std::string ping_t = "N/A"
		) {
			name = name_t;
			wanIP = wanIP_t;
			lanIP = lanIP_t;
			hostname = hostname_t;
			uptime = uptime_t;
			cpuUsage = cpuUsage_t;
			memoryUsage = memoryUsage_t;
			ping = ping_t;
		}
		std::string name;
		IP_t wanIP;
		IP_t lanIP;
		std::string hostname;
		std::string uptime;
		std::string cpuUsage;
		std::string memoryUsage;
		std::string ping;
	};

	serverRecord_t(std::string name_2, IP_t ip_2) {
		name = name_2;
		ip = ip_2;
	}
	std::string name;
	IP_t ip;

	cachedData_t cachedData;
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



struct notifications_centre_t {
	struct notification_t {
		std::string Issuer;
		std::string Description;
		std::string Reason;
		std::string ShortDescription;
		std::string Title;

		int TimeToClose;

		bool displaying;
	};

	std::vector< notification_t > notifications;

	struct preferences_t {
		bool inGame;
	};

	preferences_t preferences;
};

namespace palate {
	const float r_1 = 0.92;
	const float g_1 = 0.34;
	const float b_1 = 0.56;
	
	const float r_2 = 0.23;
	const float g_2 = 0.29;
	const float b_2 = 0.48;

	uint64_t pal_2 = 0x3c4b7b;
	uint64_t pal_3 = 0xffe4f5;
	uint64_t pal_4 = 0xea9dc0;
	uint64_t pal_5 = 0xeb5890;

}

serverRecords_t*			servers			= new serverRecords_t();
notifications_centre_t*		notifications	= new notifications_centre_t();