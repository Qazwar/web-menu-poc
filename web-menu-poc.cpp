// web-menu-poc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "settingServer.h"

#include <Windows.h>
int main()
{
    std::cout << "Hello World!\n";
	CreateThread(NULL, NULL, SettingServer::serverThread, NULL, NULL, NULL);

	while (true) {
		Sleep(1);
	}
}
