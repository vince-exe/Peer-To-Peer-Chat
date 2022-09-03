#pragma once

#include <iostream>
#include <string>
#include <algorithm>

namespace ChatUtilities {

	const int MIN_NICKNAME_LEN = 0;

	const int MAX_NICKANAME_LEN = 15;

	const int MIN_PORT = 4000;

	const int MAX_PORT = 20000;

	std::string takeNickName(int min, int max);

	void rmvEndl(std::string& string);

	int getPort(int min, int max);

	/* varable used to cache the server and client nicknames */
	static std::string clientNickName;

	static std::string serverNickName;

	const std::string disconnectMsg = "!disconnect";
}