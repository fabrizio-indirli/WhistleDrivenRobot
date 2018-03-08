
#include <cstdio>
#include "miosix.h"
#include "button.h"

using namespace std;
using namespace miosix;

int main()
{
    configureButtonInterrupt();
    for(;;)
	{
		waitForButton();
        printf("Pulsante premuto\n");
	}
}
