#include <iostream>
#include <stack>
#include <fstream>
#include <vector>

bool isLetter(const char &c);
bool isNumber(const char &c);
bool isSingleSymbol(const char &c);
bool isIdentifierChar(const char &c);

struct Node {
    std::string token;
    Node *left, *right;

    Node(std::string s, Node *l, Node *r) : token(s), left(l), right(r) {}

    int childCount();
};
int Node::childCount() {
    int n = 0;
    Node *p = left;
    while (p) {
        p = p->right;
        n++;
    }
    return n;
}


struct AST {
    Node* root;

    AST() {root=nullptr;}

    void printTree(int, Node*);

};
void AST::printTree(int depth, Node *n) {
    if (n) {
        int x = n->childCount();
        for (int i = 0; i < depth; ++i)
            std::cout << ". ";
        std::cout << n->token << "(" << x << ")" << std::endl;
        printTree(depth+1, n->left);
        printTree(depth+1, n->right);
    }
}


struct Parser {
    std::vector<std::string> v;
    unsigned long vi;
    std::stack<Node*> stack;
    AST *tree;

    Parser() {vi = 0; tree = nullptr;}

    void tokenize(const char*);
    void parse();
    void build_tree(std::string, int);
    bool read_token(std::string);
};



void Parser::tokenize(const char* filepath) {

    std::ifstream f(filepath);

    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
    }
    else {
        while (!f.fail()) {

            std::string s = "";
            char c;
            f.get(c);

            // Skip whitespace
            if (c==' ' || c=='\n' || c=='\t' || c=='\v')
                continue;

                // Numbers are any consecutive digits
            else if (isNumber(c)) {
                s.push_back(c);
                while (isNumber((char)f.peek()))
                    s.push_back((char)f.get());

                v.push_back(s);
            }

                // Chars are single quote -> char -> single quote
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

                // Strings are double quote -> any number of any other characters -> double quote
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

                // Colon
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

                // Less than
            else if (c == '<') {
                s.push_back(c);

                if (f.peek() == '=' || f.peek() == '>')
                    s.push_back((char)f.get());

                v.push_back(s);
            }

                // Greater than
            else if (c == '>') {
                s.push_back(c);

                if (f.peek() == '=')
                    s.push_back((char)f.get());

                v.push_back(s);
            }

                // Dot
            else if (c == '.') {
                s.push_back(c);

                if (f.peek() == '.')
                    s.push_back((char)f.get());

                v.push_back(s);
            }

                // Single Symbols + - * / = , ; ()
            else if (isSingleSymbol(c)) {
                s.push_back(c);
                v.push_back(s);
            }

                // Comments
            else if (c == '{') {
                while ((char)f.peek() != '}')
                    f.get();
            }
            else if (c == '#') {
                while ((char)f.peek() != '\n')
                    f.get();
            }

                // Identifiers
            else if (isLetter(c) || c=='_') {
                s.push_back(c);

                while (isIdentifierChar((char)f.peek()))
                    s.push_back((char)f.get());

                v.push_back(s);
            }
        }
    }
}

void Parser::parse() {

}

void Parser::build_tree(std::string s, int n) {
    Node *p = nullptr;
    for (int i=0; i<n; i++) {
        Node *t = stack.top();
        t->right = p;
        p = t;
    }
    stack.push(p);
}

bool Parser::read_token(std::string s) {
    if (v.at(vi) != s) {
        return false;
    }
    else {
        vi++;
        return true;
    }
    // return v.at(vi++) == s
}


int main(int argc, char const* argv[]) {
    Parser parser;

    if (argc<2){}
    else {
        std::string flagSwitch = argv[1];

        if (flagSwitch == "-ast") {

            parser.tokenize(argv[2]);

//            for (unsigned long i=0; i<parser.v.size(); i++) {
//                std::cout << parser.v.at(i) << std::endl;
//            }
            //...

        }
        else {
            printf("Error, switch not recognized");
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

// + - * / = , ; () {}
bool isSingleSymbol(const char &c) {
    return (c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c==','||c==';'||c=='=');
}

bool isIdentifierChar(const char &c) {
    return isLetter(c) || isNumber(c) || c=='_';
}