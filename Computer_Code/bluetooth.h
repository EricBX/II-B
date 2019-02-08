#ifndef bluetooth_h
#define bluetooth_h

#include"function.h"

HANDLE com = CreateFile(L"COM5", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);//´®¿ÚÑ¡Ôñ


void sendCommand(char c) {
	char data[2];
	data[0] = c;
	data[1] = '\0';
	DWORD addr = 0;
	if (!WriteFile(com, data, 1, &addr, 0))
		cout << "Bluetooth error" << endl;
}

#endif