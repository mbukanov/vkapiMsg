#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define COOKIE_FILE "cookie.txt"

#define PERMISSION_OFFLINE 0b1
#define PERMISSION_STRING_OFFLINE "offline"

#define PERMISSION_MESSAGES 0b10
#define PERMISSION_STRING_MESSAGES "messages"

#define PIPE_PATH "/tmp/VKpipe"
#define PIPE_SIZE 256

#define VKE_NORM 0
#define VKE_REQUEST -1
#define VKE_AUTH  -2
#define VKE_GRANT_ACCESS  -3

#define VKE_SEND_MESSAGE 0b1


static std::string cookiestring;
static std::string buffer;
static std::string errorBuffer;


struct ConnectSettings {
	std::string email;
	std::string password;
	std::string client_id;
	std::string scopes;
	std::string display;
	std::string settings;
	std::string current_ip_h;
	std::string current_lg_h;
	std::string current_hash;
	std::string current_direct_hash;
	std::string current_access_token;
};

class CVK {
public:

	CVK(std::string _login, std::string _password, int permissions, std::string app_id);
	~CVK();
	
	int curlSettings();
	int connectStartPage();
	int connectAuthorize();
	int connectGrantAccess();

	std::string parseHash(std::string buffer);
	std::string parseAccessToken(std::string buffer);
	std::string parseDirectHash(std::string buffer);
	std::string parseLGH(std::string buffer);
	std::string parseIPH(std::string buffer);

	static int writer(char * data, size_t size, size_t nmemb, std::string *buffer);

	void reconnect();

	int send_message(std::string user_id, std::string message);
	int set_status(std::string text);


	std::string createPermissions(int permissions);

	void cleanBuffers();

	void cleanCookies();

	int Error() { return VKError; }
	int Errno() { return VKErrno; }

	int createPipe();
	int closePipe();
	void clearPipeBuffer();
	int get_pipe_fd();
	char * getPipeBuffer();

private:
	CURL * curl;
	
	int VKErrno;
	int VKError;

	int pipe_fd;
	char pipe_buffer[PIPE_SIZE];

	std::string permissions;

	ConnectSettings connectSettings;

};