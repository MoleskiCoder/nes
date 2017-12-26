#include "stdafx.h"

#include "Configuration.h"
#include "Computer.h"

int main(int, char*[])
{
	Configuration configuration;

#ifdef _DEBUG
	configuration.setDebugMode(true);
	configuration.setProfileMode(true);
#endif

	Computer computer(configuration);
	computer.initialise();

	computer.runLoop();

	return 0;
}