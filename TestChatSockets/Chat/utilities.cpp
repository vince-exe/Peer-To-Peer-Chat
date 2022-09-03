#include "utilities.h"

std::string ChatUtilities::takeNickName(int min, int max) {
    std::string nickname;
    do {
        std::cout << "\nNickname: ";
        std::getline(std::cin, nickname);

    } while (nickname.length() <= min || nickname.length() >= max);

    return nickname;
}

void ChatUtilities::rmvEndl(std::string& string) {
    string.erase(std::remove(string.begin(), string.end(), '\n'), string.end());
}
