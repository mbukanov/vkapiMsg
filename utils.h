#include <string.h>

std::string fillSpaces(std::string msg) {
	std::string result = "";
	for(int i = 0; i < msg.size(); i++ ) {
		if(msg[i] != ' ') 
			result += msg[i];
		else
			result += "%20";
	}

	return result;
}


std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string ret;
	int i = 0, j = 0;
	unsigned char char_array_3[3], char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}

	return ret;

}


void CutOutSymbols(std::string &text, char symbol) {
	std::string str_cut_start = "";
	std::string str_cut_end = "";
	std::string result = "";
	bool startCut = false;
	bool endCut = false;
	for(int i = 0; i < text.size(); i++) {
		if(text[i] == symbol && !startCut) {
			continue;
		} else if(text[i] != symbol && !startCut) {
			startCut = true;
		}

		if(startCut) {
			str_cut_start += text[i];
		}
	}


	for(int i = (int)str_cut_start.size()-1; i >= 0; i--) {
		if(str_cut_start[i] == symbol && !endCut) {
			continue;
		} else if(str_cut_start[i] != symbol && !endCut) {
			endCut = true;
		}

		if(endCut) {
			str_cut_end += str_cut_start[i];
		}
	}

	// reverse
	for(int i = (int)str_cut_end.size()-1; i >= 0; i--) {
		result += str_cut_end[i];
	}

}
