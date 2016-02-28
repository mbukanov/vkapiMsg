#include "vk.h"


int main(int argc, char ** argv) {

	if(argc <= 1)
		return 0;

	// change this on your login and password
	std::string login = "your_email";
	std::string password = "your_password";
	std::string app_id = "app_id";

	CVK * vk = new CVK(login, password, PERMISSION_OFFLINE | PERMISSION_MESSAGES, app_id);

	if(vk->Errno() != VKE_NORM) {
		printf("VKerror, Errno = %d\n", vk->Errno());
		return 0;
	}

	std::string user_id = "202692118"; // change this on user_id recipient

	vk->send_message(user_id, argv[1]);
	if(vk->Error() & (1 << VKE_SEND_MESSAGE)) {
		printf("VKerror: send message\n");
	}

	delete vk;

	return 0;
}