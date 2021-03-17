#define _CRT_SECURE_NO_WARNINGS

#define PROTOCAL "http://"
#define PORT ":5000"
#define PATH "/info"

#include "src/Overlay/DirectOverlay.h"
#include "src/Overlay/Overlay.h"
#include <iostream>  
#include <stdlib.h> 
#include <chrono>
#include "logging.h"
#include "Utils.h"
#include "notificationHandler.h"

int windowLength = 1920;
int windowWidth = 1920;
int windowHeight = 1080;
#define FONT_SIZE 15

Vector groupBox(std::string Title, float thickness, int width, int height, int x, int y, float r = 1, float g = 0, float b = 0) {
	const int textOffset = 18;
	DrawString(Title, FONT_SIZE, Vector(x + 2, y, 0), Colour(r, g, b, 1));
	DrawBox(x, y + textOffset, x + width, y + height, 2, r, g, b, 1, false);

	return Vector(0, 0, 0);
}

std::string findByKey(std::string content, std::string key) {
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
			//std::cout << "FOUND" << std::endl;
			//std::cout << fnd << std::endl;
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
				//std::cout << "DONE KEY IS" << std::endl;
				//std::cout << tempStr << std::endl;
				return tempStr;
			}
			clog::log("Could not find key", "findByKey 1");
			return "";
		}
	}
	clog::log("Could not find key", "findByKey 2");
	return "";
}

auto lastCheck = std::chrono::system_clock::now().time_since_epoch();

void updateCache(std::string ip, serverRecord_t::cachedData_t* cachedData) {
	auto info = Utils::GetStdoutFromCommand("curl http://" + ip + ":5000/info");

	cachedData->name = findByKey(info, "name");;
	cachedData->wanIP = IP_t(findByKey(info, "wanIP"));
	cachedData->lanIP = IP_t(findByKey(info, "lanIP"));
	cachedData->hostname = findByKey(info, "hostname");
	cachedData->uptime = findByKey(info, "uptime");;
	cachedData->cpuUsage = findByKey(info, "cpuUsage");;
	cachedData->memoryUsage = findByKey(info, "memoryUsage");
	cachedData->ping = Utils::getServerPing("google.com");
}

void updateServersCache() {
	for (int i = 0; i <= servers->servers.size(); i++) {
		serverRecord_t tmp = servers->servers.at(i);
		if (tmp.cachedData.name == "N/A") {
			// Cache hasnt been initiated

			updateCache(tmp.ip.getIpAddress(), &tmp.cachedData);
		}

	}
}

void serverSegmant(int x, int y) {
	int draw_x, draw_y;
	// 0, 0
	//DrawBox(x + 10, y + 10, 
	//	    x + 600, y + 300
	//	, 2, 1, 0, 1, 1, false);
	DrawRoundedBox(
		x + 10, y + 10,		// pos 1
		x + 600, y + 300,	// pos 2 
		10, 10, 
		2, 
		palate::r_1, palate::g_1, palate::b_1, 1,
		false);

	draw_x = x + 13;
	draw_y = y + 13;

	groupBox("Conni", 1, 500, 200, draw_x + 10, draw_y, palate::r_1, palate::g_1, palate::b_1);
	draw_x = draw_x + 13;
	draw_y = draw_x + 13;

	static float maxWidth = 0;
	groupBox("Physical Servers", 1, 9 * maxWidth, 100, draw_x + 10, draw_y, palate::r_1, palate::g_1, palate::b_1);
	draw_x += 16;
	draw_y += 21;


	
	static std::string info = "";

	static std::string serverName = "";
	static std::string serverUptime = "";
	static std::string serverWanIP = "";
	static std::string serverLanIP = "";
	static std::string serverPing = "";

	if (serverName.length() > maxWidth) maxWidth = serverName.length();
	if (serverUptime.length() > maxWidth) maxWidth = serverUptime.length();
	if (serverWanIP.length() > maxWidth) maxWidth = serverWanIP.length();
	if (serverLanIP.length() > maxWidth) maxWidth = serverLanIP.length();
	if (serverPing.length() > maxWidth) maxWidth = serverPing.length();

	if (info == "" ||
		(int)lastCheck.count() + 10'000'000 < (int)std::chrono::system_clock::now().time_since_epoch().count()
		) {
		clog::log("Checking Servers", "serverSegmant", loggingLevel::DEBUGGING_t);
		lastCheck = std::chrono::system_clock::now().time_since_epoch();
		
		
		//ShellExecute(0, L"open", L"cmd.exe", L"server_1.bat > out.txt", 0, SW_HIDE);
		//info = readFileAsString("./out.txt");
		info = Utils::GetStdoutFromCommand("curl http://spookiebois.club:5000/info");

		serverName		=	findByKey(info, "name");;
		serverUptime	=	findByKey(info, "uptime");;
		serverWanIP		=	findByKey(info, "wanIP");;
		serverLanIP		=	findByKey(info, "lanIP");
		serverPing		=	Utils::getServerPing("google.com");
	}

	if (serverName != "") {
		//groupBox(serverName + " - Online!", 1, 8 * maxWidth, 50, draw_x, draw_y, 1, 0, 0);
		DrawString(serverName + " - Online!",		FONT_SIZE, draw_x, draw_y,					1, 0, 0,	1);

		draw_x += 12;
		draw_y += 21;

		DrawString("WanIP: "	+ serverWanIP,		FONT_SIZE, draw_x, draw_y,					1, 0, 0,	1);
		DrawString("LanIP: "	+ serverLanIP,		FONT_SIZE, draw_x, draw_y + FONT_SIZE * 1,	1, 0, 0,	1);
		DrawString("Uptime: "	+ serverUptime,		FONT_SIZE, draw_x, draw_y + FONT_SIZE * 2,	1, 0, 0,	1);
		DrawString("Ping: "		+ serverPing,		FONT_SIZE, draw_x, draw_y + FONT_SIZE * 3,	1, 0, 0,	1);
		//DrawString("Uptime: 12 Days", FONT_SIZE, draw_x, draw_y + FONT_SIZE, 1, 0, 0, 1);


	}
	else {
		groupBox("Offline!", 1, 75, 50, draw_x, draw_y, 1, 0, 0);
		draw_x += 12;
		draw_y += 21;
	}


	//DrawBox(draw_x, draw_y, 100, 100, 2, 0, 1, 0, 1, false);
}


namespace regions {
	namespace top {
		void left() {
			int topLeft_x = 0;
			int topLeft_y = 0;
			serverSegmant(topLeft_x, topLeft_y);
		}
	}
}

static bool lastDisplaying = false;
static bool displaying = false;

static bool anim_notif = false;
static float currentWidth = 0;
static float targetWidth = 0;
static bool anim_slideIn = true;

const float widthPerLetter = (FONT_SIZE / 2);

int maxStrLength(std::string str) {
	std::string lastYes = "";
	for (int i = 0; i <= str.length(); i++) {
		if (i * widthPerLetter < currentWidth) lastYes = str.substr(0, i);
		else 
			return lastYes.length();
	}

}

bool DrawNotification(float x, float y, notifications_centre_t::notification_t notif) {
	std::string textToDisplay = notif.Title.substr(0, maxStrLength(notif.Title));

	bool done = false;
	if (!anim_notif) {
		anim_notif = true;
		if (displaying) {
			targetWidth = x + 200;
		}
		else 
		{
			targetWidth = 20 + ((FONT_SIZE / 2) * notif.Title.length());
		}
	}
	else {
		anim_notif = true;
		float tmp = targetWidth / 12;
		if (anim_slideIn) {
			currentWidth += tmp;

		}
		else {
			currentWidth -= tmp;

		}
		targetWidth -= tmp;
		if (tmp <= 0.0001) {
			if (anim_notif) {
				if (anim_slideIn) {
					anim_notif = false;
					anim_slideIn = false;
				}
				else {
					anim_slideIn = true;
					currentWidth = 0.f;
					done = true;
				}
			}
			else 
			{

			}
			targetWidth = 0;
		}
	}

	if (displaying) {
		DrawBox(x, y, x + currentWidth, y + 20, 2, palate::r_2, palate::g_2, palate::b_2, 1, true);

		DrawString(textToDisplay, FONT_SIZE, x + 4, y + 4,		     palate::r_1, palate::g_1, palate::b_1, 1);
		DrawString(textToDisplay, FONT_SIZE, x + 4, y + 4 + FONT_SIZE, palate::r_1, palate::g_1, palate::b_1, 1);
	}
	else {
		DrawBox(x, y, x + currentWidth, y + 20, 2, palate::r_2, palate::g_2, palate::b_2, 1, true);

		DrawString(textToDisplay, FONT_SIZE, x + 4, y + 4, palate::r_1, palate::g_1, palate::b_1, 1);
	}

	return done;
}


void drawLoop(int width, int height) {	
	bool clearToDoHeavyWork = true;

	if (lastDisplaying != displaying) {
		lastDisplaying = displaying;
		Sleep(1000);
	}
	else {
		bool t = GetAsyncKeyState(VK_CONTROL);
		bool tt = GetAsyncKeyState(VK_SHIFT);
		if(t &&  tt) {
			displaying = !displaying;
			clearToDoHeavyWork = false;
		}
	}


	static int currentNotif = 0;
	if (notifications->notifications.size() - currentNotif > 0) {
		clearToDoHeavyWork = false;


		notifications_centre_t::notification_t tmpNotif = notifications->notifications.at(currentNotif);
		if (DrawNotification(0.f, 90.f, tmpNotif)) {
			currentNotif++;
			anim_notif = false;
			currentWidth = 0;
			targetWidth = 0;
			anim_slideIn = true;
		}

	}

	if (clearToDoHeavyWork) CheckNotifications();

	if (!displaying) return;
		
	//DrawLine(0, 0, 100, 100, 5, 1, 1, 0, .8);
	//DrawBox(100, 100, 100, 100, 5, 0, 1, 0, 1, 0);
	//DrawCircle(50, 50, 20, 1, 1, 0, 0, .25, 1);
	//DrawEllipse(500, 100, 50, 20, 5, 1, 0, 0, 1, 0);

	// Dim Background
	DrawBox(0, 0, 1920, 1080, 1, 0.13, 0.18, 0.36, 0.9, true);

	//regions::top::left();
}

auto window = FindWindow(NULL, L"untitled - notepad");

void main()
{
	servers->addNew("127.0.0.1", "Connis Main Server");

	DirectOverlaySetOption(D2DOV_DRAW_FPS | D2DOV_FONT_IMPACT);
	DirectOverlaySetup(drawLoop, window);

	getchar();
}
