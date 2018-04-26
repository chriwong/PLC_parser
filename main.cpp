#include <iostream>
#include <stack>
#include <fstream>
#include <vector>

bool isLetter(const char &c);
bool isNumber(const char &c);
bool isValidSymbol(const char &c);
bool isIdentifierChar(const char &c);

int main() {

    std::vector<std::string> v;

    std::ifstream f("C:\\Users\\Christian\\UF Classes\\COP4020\\project\\project\\sample.txt");

    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
    }
    else {

        while (!f.fail()) {

            char c;
            f.get(c);

            std::cout << "got char " << c << std::endl;

            std::string s = "";
            s.push_back(c);

            std::cout << "string " << s << std::endl;

            if (isLetter(c)) {

                while (isLetter(f.peek()))
                    s.push_back(f.get());

            } else {
                v.push_back(s);
            }
        }

        for (int i = 0; i < v.size(); ++i) {
            std::cout << v.at(i) << std::endl;
        }
    }

    return 0;
}



bool isLetter(const char &c) {
    return (c>64 && c<91) || (c>96 && c<123);
}

bool isNumber(const char &c) {
    return (c>47 && c<58);
}

bool isValidSymbol(const char &c) {
    return (c>57 && c<63) || ();
}

bool isIdentifierChar(const char &c) {
    return isLetter(c) || isNumber(c) || c=='_';
}