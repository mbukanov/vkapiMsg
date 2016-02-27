#include "vk.h"


CVK::~CVK() 
{
	if(curl != NULL)
		curl_easy_cleanup(curl);
}

void CVK::cleanCookies() {
	unlink(COOKIE_FILE);
}

std::string CVK::parseHash(std::string buffer) 
{
	std::string hash = "";

	for(int i = buffer.length()-1; i > 0; i--)
	{
		if( buffer[i] == '&' &&
			buffer[i+1] == 'h' &&
			buffer[i+2] == 'a' && 
			buffer[i+3] == 's' &&
			buffer[i+4] == 'h' &&
			buffer[i+5] == '='
			)
		{
			i += 6;
			while(buffer[i] != '&')
			{
				hash += buffer[i];
				i++;
			}
			break;
		}
	}

	return hash;
}



std::string CVK::parseAccessToken(std::string buffer)
{
	std::string token = "";
	for(int i = 0; i < buffer.length(); i++)
	{
		if( buffer[i] == '#' &&
			buffer[i+1] == 'a' &&
			buffer[i+2] == 'c' &&
			buffer[i+3] == 'c' &&
			buffer[i+4] == 'e' &&
			buffer[i+5] == 's' &&
			buffer[i+6] == 's' &&
			buffer[i+7] == '_' &&
			buffer[i+8] == 't' &&
			buffer[i+9] == 'o' &&
			buffer[i+10] == 'k' &&
			buffer[i+11] == 'e' &&
			buffer[i+12] == 'n' &&
			buffer[i+13] == '='
			)
		{
			i += 14;
			while(buffer[i] != '&')
			{
				token += buffer[i];
				i++;
			}
			break;
		}
	}

	return token;
}

std::string CVK::parseDirectHash(std::string buffer)
{
	std::string hash = "";

	for(int i = 0; i < buffer.length()-10; i++)
	{
		if( buffer[i] == '&' &&
			buffer[i+1] == 'd' &&
			buffer[i+2] == 'i' && 
			buffer[i+3] == 'r' &&
			buffer[i+4] == 'e' &&
			buffer[i+5] == 'c' &&
			buffer[i+6] == 't' &&
			buffer[i+7] == '_' &&
			buffer[i+8] == 'h' &&
			buffer[i+9] == 'a' &&
			buffer[i+10] == 's' &&
			buffer[i+11] == 'h' &&
			buffer[i+12] == '='
			)
		{
			i += 13;
			while(buffer[i] != '&')
			{
				hash += buffer[i];
				i++;
			}
			break;
		}
	}
	return hash;
}



std::string CVK::parseLGH(std::string buffer)
{
	std::string lg_h = "";

	for(int i = 0; i < buffer.length()-10; i++)
	{
		if( buffer[i] == '=' &&
			buffer[i+1] == '"' &&
			buffer[i+2] == 'l' && 
			buffer[i+3] == 'g' &&
			buffer[i+4] == '_' &&
			buffer[i+5] == 'h' &&
			buffer[i+6] == '"'
			)
		{
			i += 15;
			while(buffer[i] != '"')
			{
				lg_h += buffer[i];
				i++;
			}
			break;
		}
	}

	return lg_h;
}



std::string CVK::parseIPH(std::string buffer)
{
	std::string ip_h = "";

	for(int i = 0; i < buffer.length()-10; i++)
	{
		if( buffer[i] == '=' &&
			buffer[i+1] == '"' &&
			buffer[i+2] == 'i' && 
			buffer[i+3] == 'p' &&
			buffer[i+4] == '_' &&
			buffer[i+5] == 'h' &&
			buffer[i+6] == '"'
			)
		{
			i += 15;
			while(buffer[i] != '"')
			{
				ip_h += buffer[i];
				i++;
			}
			break;
		}
	}

	return ip_h;
}


int CVK::writer(char * data, size_t size, size_t nmemb, std::string *buffer)
{
	int result = 0;
	if(buffer != NULL)
	{
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}


CVK::CVK(std::string _login, std::string _password) 
{
	VKError = VKE_NORM;
	VKErrno = VKE_NORM;
	cleanCookies();
	connectSettings.client_id = "5312450";
	connectSettings.current_access_token = "";
	connectSettings.email = _login;
	connectSettings.password = _password;

	connectSettings.scopes = "offline,messages";
	connectSettings.display = "mobile";
	connectSettings.settings = "69632";

	reconnect();
}

int CVK::curlSettings() {
	printf("Start curlSettings\n");
	int result = -1;

	cleanBuffers();

	if(curl != NULL) {
		curl_easy_cleanup(curl);
		curl = NULL;
	}
	
	curl = curl_easy_init();

	if(!curl)
		return VKE_REQUEST;

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./cookie.txt");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./cookie.txt");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Opera/9.80 (Windows NT 6.1; U; ru) Presto/2.2.15 Version/10.10");
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiestring.c_str());
    curl_easy_setopt(curl, CURLOPT_PORT, 443);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CVK::writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
}

void CVK::cleanBuffers() {
	buffer = "";
	errorBuffer = "";
}

int CVK::connectStartPage() {
	printf("Start connectStartPage\n");

	int result = -1;

	cleanBuffers();

	std::string url = "https://oauth.vk.com/authorize?client_id=";
    url += connectSettings.client_id;
    url += "&scope="+connectSettings.scopes+"&redirect_uri=https://oauth.vk.com/blank.html&display=";
    url += connectSettings.display;
    url += "&v=5.37&response_type=token&revoke=1";

    printf("Send url = %s\n", url.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    result = curl_easy_perform(curl);

    if(result != CURLE_OK) {
    	result = VKE_REQUEST;
    	VKErrno = VKE_REQUEST;
    }

    connectSettings.current_ip_h = parseIPH(buffer);
    connectSettings.current_lg_h = parseLGH(buffer);
    connectSettings.current_hash = parseHash(buffer);

    return result;
}

int CVK::connectAuthorize() {
	printf("Start connectAuthorize\n");

	cleanBuffers();

	int result = -1;
	std::string url = "https://login.vk.com/?act=login&soft=1&utf8=1";

    std::string request = "_origin=https://oauth.vk.com";
    request += "&to=";
    request += "aHR0cHM6Ly9vYXV0aC52ay5jb20vYXV0aG9yaXplP2NsaWVudF9pZD01MzEyNDUwJnJlZGlyZWN0X3VyaT1odHRwcyUzQSUyRiUyRm9hdXRoLnZrLmNvbSUyRmJsYW5rLmh0bWwmcmVzcG9uc2VfdHlwZT10b2tlbiZzY29wZT02OTYzMiZ2PTUuMzcmc3RhdGU9JnJldm9rZT0xJmRpc3BsYXk9bW9iaWxl";
    request += "&email=" + connectSettings.email;//+"test_email";
    request += "&pass=" + connectSettings.password;//+"test_pass";
    request += "&ip_h=" + connectSettings.current_ip_h;
    request += "&lg_h=" + connectSettings.current_lg_h;

    printf("Send %s, with POST = %s\n", url.c_str(), request.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
    result = curl_easy_perform(curl);

    if(result != CURLE_OK) {
    	VKErrno = VKE_REQUEST;
    	result = VKE_REQUEST;
    	return VKE_REQUEST;
    }


    //connectSettings.current_ip_h = parseIPH(buffer);
    connectSettings.current_hash = parseHash(buffer);
    connectSettings.current_direct_hash = parseDirectHash(buffer);

    if( connectSettings.current_ip_h.empty() ) {
    	VKErrno = VKE_AUTH;
    	result = VKE_AUTH;
    } else {
    	result = 0;
    }

    printf("VKErrno = %d\n", VKErrno);

    return result;
}

int CVK::connectGrantAccess() {
	printf("Start connectGrantAccess\n");
	int result = -1;

	cleanBuffers();

    std::string url  = "https://login.vk.com/?act=grant_access&client_id="+connectSettings.client_id;
    url += "&settings=";
    url += connectSettings.settings;
    url += "&redirect_uri=https%3A%2F%2Foauth.vk.com%2Fblank.html";
    url += "&response_type=token";
    url += "&direct_hash=" + connectSettings.current_direct_hash;
    url += "&token_type=0&v=5.37";
    url += "&state=&display=";
    url += connectSettings.display;
    url += "&ip_h="+connectSettings.current_ip_h;
    url += "&hash="+connectSettings.current_hash;
    url += "&https=1";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    result = curl_easy_perform(curl);

    if(result != CURLE_OK) {
    	VKErrno = VKE_REQUEST;
    	result = VKE_REQUEST;
    }

    connectSettings.current_access_token = parseAccessToken(buffer);

    if(connectSettings.current_access_token.empty()) {
    	VKErrno = VKE_GRANT_ACCESS;
    	result = VKE_GRANT_ACCESS;
    } else {
    	result = 0;
    }

    return result;
}


void CVK::reconnect() 
{
	printf("Reconnect start\n");

	curlSettings();
	if(VKErrno != VKE_NORM) return;

	connectStartPage();
	if(VKErrno != VKE_NORM) return;

    connectAuthorize();
    if(VKErrno != VKE_NORM) return;

    connectGrantAccess();
    if(VKErrno != VKE_NORM) return;
}


std::string CVK::fillSpaces(std::string msg) {
	std::string result = "";
	for(int i = 0; i < msg.size(); i++ ) {
		if(msg[i] != ' ') 
			result += msg[i];
		else
			result += "%20";
	}

	return result;
}


int CVK::send_message(std::string user_id, std::string message) {
	printf("Start send_message\n");

	int result = -1;
	cleanBuffers();

	message = fillSpaces(message);

	std::string method = "messages.send";

	std::string url = "https://api.vk.com/method/";
	url += method;
	url += "?user_id=";
	url += user_id;
	url += "&message=";
	url += message;
	url += "&v=5.37&access_token=";
	url += connectSettings.current_access_token;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	result = curl_easy_perform(curl);

	if(result != CURLE_OK) {
		VKErrno = VKE_REQUEST;
		result = VKE_REQUEST;
	}

	std::size_t found = buffer.find("\"error\"");
	if(found != std::string::npos)
		VKError |= VKE_SEND_MESSAGE;

	return result;
}

int CVK::set_status(std::string text) {
	printf("Start set_status\n");
	int result = -1;
	cleanBuffers();

	text = fillSpaces(text);

    std::string method = "status.set";
    std::string url = "https://api.vk.com/method/";
    url += method;
    url += "?text=";
    url += text;
    url += "&access_token=";
    url += connectSettings.current_access_token;

    printf("setStatus, url = %s\n", url.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    result = curl_easy_perform(curl);

    return result;
}