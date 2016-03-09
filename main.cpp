#include "vk.h"


int main(int argc, char ** argv) {

	// change this on your login and password 
	std::string login = "your_email";
	std::string password = "your_password";
	std::string app_id = "5312450";

	CVK * vk = new CVK(login, password, PERMISSION_OFFLINE | PERMISSION_MESSAGES, app_id);

	if(vk->Errno() != VKE_NORM) {
		printf("VKerror, Errno = %d\n", vk->Errno());
		return 0;
	}

	std::string user_id = "202692118"; // change this on user_id recipient

	vk->createPipe();
	vk->clearPipeBuffer();

	while(true) {
		while(read(vk->get_pipe_fd(), vk->getPipeBuffer(), PIPE_SIZE) > 0) {
			vk->send_message(user_id, vk->getPipeBuffer());
			if(vk->Error() & (1 << VKE_SEND_MESSAGE)) {
				printf("VKerror: send message\n");
			}
			vk->clearPipeBuffer();
		}
	}


	delete vk;

	return 0;
}