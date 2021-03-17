#pragma once

#include "structs.h"
#include "Utils.h"

std::string findByKey2(std::string content, std::string key) {
	int cnt = 0;
	std::string fnd = "";
	for (int i = 0; i < content.length(); i++) {
		if (content.at(i) == key.at(cnt)) {
			cnt++;
			fnd += content.at(i);
		}
		else {
			cnt = 0;
			fnd = "";
		}
		if (cnt >= key.size()) {
			while (1) {
				int contentBase = i + 1 + 1;
				int e = contentBase;
				std::string tempStr = "";
				while (content.at(e) != ',') {
					if (e >= content.length()) {
						return "";
					}
					tempStr += content.at(e);
					e++;
				}
				return tempStr;
			}
			clog::log("Could not find key", "findByKey 1");
			return "";
		}
	}
	clog::log("Could not find key", "findByKey 2");
	return "";
}

void CheckNotifications() {
		std::string nCountS = Utils::GetStdoutFromCommand("curl localhost:5000/ncount");

		//std::cout << nCountS << std::endl;
		//int notification_count = std::stoi(nCountS);
		//std::cout << notification_count << std::endl;
		std::string res = findByKey2(nCountS, "count");
 		if (std::stoi(res) == 0) {
			clog::log("No new notifications.");
		}	
		else
		{
			std::string response = Utils::GetStdoutFromCommand("curl localhost:5000/notifications");
			for (int i = 0; i < std::stoi(res); i++) {

				notifications_centre_t::notification_t notif;
				notif.Title = findByKey2(response, std::to_string(i) + "Title");
				notif.ShortDescription = findByKey2(response, std::to_string(i) + "ShortDescription");
				notif.Description = findByKey2(response, std::to_string(i) + "Description");
				notif.Reason = findByKey2(response, std::to_string(i) + "Reason");
				notif.Issuer = findByKey2(response, std::to_string(i) + "Issuer");

				notifications->notifications.push_back(notif);
				clog::log("New notification stored!");
			}
			
		}
		//else {
		//}
}
