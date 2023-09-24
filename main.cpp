#include "WebServer.h"

using namespace std;

int main() {
	WebServer webServer("127.0.0.1", 3000);
	if (webServer.init() != 0) {
		return 0;
	}
	webServer.run();

	return 0;
}