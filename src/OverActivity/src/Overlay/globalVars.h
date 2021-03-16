#pragma once
#define SIGS hazedumper::signatures
#include <Windows.h>

//struct config_t {
//
//	struct bhop_t {
//		bool enabled = false;
//	};
//	struct esp_t {
//		bool enabled = false;
//	};
//
//	bhop_t* bhop = new bhop_t();
//	esp_t* esp = new esp_t();
//};
//config_t* config = new config_t();

struct vars_t {

	DWORD pID;
	HANDLE procHandle;
	DWORD clientModule;
	DWORD engineModule;
	DWORD vstdlibModule;
	DWORD clientState;

	float frameTime = 0.f;
};

vars_t* gVars = new vars_t();