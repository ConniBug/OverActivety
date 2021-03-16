#define _CRT_SECURE_NO_WARNINGS

#include "src/Overlay/DirectOverlay.h"
#include "src/Overlay/Overlay.h"
#include <iostream>  
#include <stdlib.h> 
#include <chrono>
#include "logging.h"

int windowLength = 1920;
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

#define PROTOCAL "http://"
#define PORT ":5000"
#define PATH "/info"

auto lastCheck = std::chrono::system_clock::now().time_since_epoch();

void updateServersCache() {
	for (int i = 0; i <= servers->servers.size(); i++) {
		serverRecord_t tmp = servers->servers.at(i);


	}
}

#include "Utils.h"
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
		palate::pal_1_r, palate::pal_1_g, palate::pal_1_b, 1,
		false);

	draw_x = x + 13;
	draw_y = y + 13;

	groupBox("Conni", 1, 500, 200, draw_x + 10, draw_y, palate::pal_1_r, palate::pal_1_g, palate::pal_1_b);
	draw_x = draw_x + 13;
	draw_y = draw_x + 13;

	static float maxWidth = 0;
	groupBox("Physical Servers", 1, 9 * maxWidth, 100, draw_x + 10, draw_y, palate::pal_1_r, palate::pal_1_g, palate::pal_1_b);
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
		info = Utils::GetStdoutFromCommand("curl http://localhost:5000/info");

		serverName		=	findByKey(info, "name");;
		serverUptime	=	findByKey(info, "uptime");;
		serverWanIP		=	findByKey(info, "wanIP");;
		serverLanIP		=	findByKey(info, "lanIP");;
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
void drawLoop(int width, int height) {
	if (lastDisplaying != displaying) {
		lastDisplaying = displaying;
		Sleep(1000);
	}else if (GetAsyncKeyState(VK_CONTROL)) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			displaying = !displaying;
		}
	}
	if (displaying) return;
		
	//DrawLine(0, 0, 100, 100, 5, 1, 1, 0, .8);
	//DrawBox(100, 100, 100, 100, 5, 0, 1, 0, 1, 0);
	//DrawCircle(50, 50, 20, 1, 1, 0, 0, .25, 1);
	//DrawEllipse(500, 100, 50, 20, 5, 1, 0, 0, 1, 0);

	// Dim Background
	DrawBox(0, 0, 1920, 1080, 1, 0.13, 0.18, 0.36, 0.8, true);

	regions::top::left();
}

auto window = FindWindow(NULL, L"untitled - notepad");

void main()
{

	servers->addNew("127.0.0.1", "Connis Main Server");

	DirectOverlaySetOption(D2DOV_DRAW_FPS | D2DOV_FONT_IMPACT);
	DirectOverlaySetup(drawLoop, window);
	getchar();
}
