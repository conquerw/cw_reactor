#include <stdio.h>
#include <unistd.h>
#include <httpserver.h>

int main()
{
	httpserver_init();
	
	while(1)
		sleep(10);
}
