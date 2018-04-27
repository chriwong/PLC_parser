#include <iostream>
#include <stack>
#include <fstream>
#include <vector>

bool isLetter(const char &c);
bool isNumber(const char &c);
bool isSingleSymbol(const char &c);
bool isIdentifierChar(const char &c);

int main() {

    std::vector<std::string> v;

    std::ifstream f("C:\\Users\\Christian\\UF Classes\\COP4020\\project\\project\\sample.txt");

    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
    }
    else {

        while (!f.fail()) {

            std::string s = "";
            char c;
            f.get(c);

            if (c==' ' || c=='\n' || c=='\t' || c=='\v')
                continue;

            else if (isNumber(c)) {
                s.push_back(c);
                while (isNumber((char)f.peek()))
                    s.push_back((char)f.get());

                v.push_back(s);
            }
            else if (c == '\'') {
                s.push_back(c);

                if (f.peek() != '\'') {
                    s.push_back((char)f.get());

                    if (f.peek() == '\'') {
                        s.push_back((char)f.get());
                        v.push_back(s);
                    }
                    else
                        printf("Error Char token - expected ending quote but received %c", f.get());
                }
                else
                    printf("Error Char token - expected non-quote but received %c", f.get());
            }
            else if (c == '\"') {
                s.push_back(c);

                while (f.peek() != '\"')
                    s.push_back((char)f.get());

                if (f.peek() != '\"')
                    printf("Error String token - no closing quotes");
                else {
                    s.push_back((char)f.get());
                    v.push_back(s);
                }
            }
            else if (c == ':') {
                s.push_back(c);

                if (f.peek() == '=') {
                    s.push_back((char)f.get());

                    if (f.peek() == ':') {
                        s.push_back((char)f.get());
                    }
                }
                v.push_back(s);
            }
            else if (c == '<') {
                s.push_back(c);

                if (f.peek() == '=' || f.peek() == '>')
                    s.push_back((char)f.get());

                v.push_back(s);
            }
            else if (c == '>') {
                s.push_back(c);

                if (f.peek() == '=')
                    s.push_back((char)f.get());

                v.push_back(s);
            }
            else if (c == '.') {
                s.push_back(c);

                if (f.peek() == '.')
                    s.push_back((char)f.get());

                v.push_back(s);
            }
            else if (isSingleSymbol(c)) {
                s.push_back(c);
                v.push_back(s);
            }

            //identifiers


        }



        for (unsigned long i=0; i < v.size(); i++) {
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

bool isSingleSymbol(const char &c) {
    return (c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c==','||c=='{'||c=='}'||c==';');
}

bool isIdentifierChar(const char &c) {
    return isLetter(c) || isNumber(c) || c=='_';
}