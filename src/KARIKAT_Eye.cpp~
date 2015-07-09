//============================================================================
// Name        : KARIKAT.cpp
// Author      : Wonsub Choi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "kai/kai.h"
#include "MPEGCoder.h"
#include "ModeManager.h"

#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>


int wasKeyboardHit()
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	// don't echo and don't wait for ENTER
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

	// make it non-blocking (so we can check without waiting)
	if (0 != fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK))
	{
		return 0;
	}

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	if (0 != fcntl(STDIN_FILENO, F_SETFL, oldf))
	{
		return 0;
	}

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}




int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	printf("size of unsigned long : %d\n", sizeof(unsigned long));

	kaiInitialize();
	MPEGLib::InitailizeLibrary();

	ModeManager modemanager;

	modemanager.init();

	while(true)
	{
		modemanager.process();
		usleep(30000);
	}

	printf("end..\n ");

	return 0;
}
