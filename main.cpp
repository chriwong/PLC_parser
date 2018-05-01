#include <iostream>
#include <stack>
#include <fstream>
#include <vector>

bool isLetter(const char &);
bool isNumber(const char &);
bool isSingleSymbol(const char &);
bool isIdentifierChar(const char &);
bool isValidIdentifier(std::string);
bool isValidChar(std::string);
bool isValidString(std::string);

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


struct Parser {
    std::vector<std::string> v;
    unsigned long vi;
    std::stack<Node*> stack;
    Node *root;

    Parser() {vi = 0; root=nullptr;}

    void tokenize(const char*);
    void build_tree(std::string, int);
    bool read_token(std::string);
    void Tiny();
    void Consts();
    void Const();
    void ConstValue();
    void Types();
    void Type();
    void LitList();
    void SubProgs();
    void Fcn();
    void Params();
    void Dclns();
    void Dcln();
    void Body();
    void Statement();
    void OutExp();
    void StringNode();
    void Caseclauses();
    void Caseclause();
    void CaseExpression();
    void OtherwiseClause();
    void Assignment();
    void ForStat();
    void ForExp();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void Name();
    void parse();
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

void Parser::build_tree(std::string s, int n) {
    Node *p = nullptr;
    for (int i=0; i<n; i++) {
        Node *t = stack.top();
        stack.pop();
        t->right = p;
        p = t;
    }
    stack.push(new Node(s, p, nullptr));
}

/* Has to have a special, generic case for identifiers... */
bool Parser::read_token(std::string s) {
    if (s == "<identifier>") {
        vi++;
        return true;
    }
    else {
        if (v.at(vi) != s) {
            std::cout << "Error reading " << s << std::endl;
            return false;
        } else {
            vi++;
            std::cout << "Correctly read " << s << std::endl;
            std::cout << " vi : " << vi << std::endl;
            return true;
        }
    }
    // return v.at(vi++) == s
}

void Parser::Tiny() {
    if (v.at(vi) == "program") {
        read_token("program");
        Name();
        read_token(":");
        Consts();
        Types();
        Dclns();
        SubProgs();
        Body();
        Name();
        read_token(".");
        build_tree("program", 7);
    }
    else {
        std::cout << "Error in Tiny()" << std::endl;
    }
}

void Parser::Consts() {
    if (v.at(vi) == "const") {
        int n = 1;
        read_token("const");
        Const();
        while (v.at(vi) == ",") {
            read_token(",");
            Const();
            n++;
        }
        read_token(";");
        build_tree("consts", n);
    }
    else {
        build_tree("consts", 0);
    }
}

void Parser::Const() {
    Name();
    read_token("=");
    ConstValue();
    build_tree("const", 2);
}

void Parser::ConstValue() {}

/*
 * I don't know how to code the regex + operator (one or more occurences)
 * It should call Type() and read a semicolon at least one time, but the continuity case
 *  should stop when
 *      1) Next token after the semicolon is 'var' (which doesn't happen
 *          all the time, so that won't work (though for this project it might) )
 *      2) Next token is not an identifier (meaning it is one of the keywords)
 *          I'm probably going to do this, but it's going to be super tedious typing every single keyword
*/
void Parser::Types() {
    if (v.at(vi) == "type") {
        int n = 0;
        read_token("type");
        do {
            Type();
            read_token(";");
            n++;
        } while (v.at(vi) != "var"); //TODO change to while(v.at(vi).isNotKeyword)
        build_tree("types", n);
    }
    else {
        build_tree("types", 0);
    }
}

void Parser::Type() {
    Name();
    read_token("=");
    LitList();
    build_tree("type", 2);
}

void Parser::LitList() {
    int n = 1;
    read_token("(");
    Name();
    while (v.at(vi) == ",") {
        read_token(",");
        Name();
        n++;
    }
    read_token(")");
    build_tree("lit", n);
}

void Parser::SubProgs() {
    int n = 0;
    while (v.at(vi) == "function") {
        Fcn();
        n++;
    }
    build_tree("subprogs", n);
}

void Parser::Fcn() {
    if (v.at(vi) == "function") {
        read_token("function");
        Name();
        read_token("(");
        Params();
        read_token(")");
        read_token(":");
        Name();
        read_token(";");
        Consts();
        Types();
        Dclns();
        Body();
        Name();
        read_token(";");
    }
    else {
        std::cout << "Error in Fcn()" << std::endl;
    }
}

void Parser::Params() {
    int n = 1;
    Dcln();
    while (v.at(vi) == ";") {
        Dcln();
        n++;
    }
    build_tree("params", n);
}

void Parser::Dclns() {
    if (v.at(vi) == "var") {
        int n = 0;
        read_token("var");
        do {
            Dcln();
            read_token(";");
            n++;
        } while (v.at(vi) != "function" || v.at(vi) != "begin");
        build_tree("dclns", n);
    }
    else {
        build_tree("dclns", 0);
    }
}

void Parser::Dcln() {
    int n = 1;
    Name();
    while (v.at(vi) == ",") {
        read_token(",");
        Name();
        n++;
    }
    read_token(":");
    Name();
    build_tree("var", 1+n);
}

void Parser::Body() {
    read_token("begin");
    int n = 1;
    Statement();
    while (v.at(vi) == ";") {
        read_token(";");
        Statement();
        n++;
    }
    read_token("end");
    build_tree("block", n);
}

void Parser::Statement() {
    if (v.at(vi) == "output") {
        read_token("output");
        read_token("(");
        int n = 1;
        OutExp();
        while (v.at(vi) == ",") {
            read_token(",");
            OutExp();
            n++;
        }
        read_token(")");
        build_tree("output", n);
    }
    else if (v.at(vi) == "if") {
        read_token("if");
        Expression();
        read_token("then");
        Statement();
        if (v.at(vi) == "else") {
            read_token("else");
            Statement();
            build_tree("if", 3);
        }
        else
            build_tree("if", 2);
    }
    else if (v.at(vi) == "while") {
        read_token("while");
        Expression();
        read_token("do");
        Statement();
        build_tree("while", 2);
    }
    else if (v.at(vi) == "repeat") {
        read_token("token");
        int n = 1;
        Statement();
        while (v.at(vi) == ";") {
            read_token(";");
            Statement();
            n++;
        }
        read_token("until");
        Expression();
        build_tree("repeat", 1+n);
    }
    else if (v.at(vi) == "for") {
        read_token("for");
        read_token("(");
        ForStat();
        read_token(";");
        ForExp();
        read_token(";");
        ForStat();
        read_token(")");
        Statement();
        build_tree("for", 4);
    }
    else if (v.at(vi) == "loop") {
        read_token("loop");
        int n = 1;
        Statement();
        while (v.at(vi) == ";") {
            read_token(";");
            Statement();
            n++;
        }
        read_token("pool");
        build_tree("loop", n);
    }
    else if (v.at(vi) == "case") {
        read_token("case");
        Expression();
        read_token("of");
        Caseclauses();
        OtherwiseClause();
        read_token("end");
        build_tree("case", 3);
    }
    else if (v.at(vi) == "read") {
        read_token("read");
        read_token("(");
        int n = 1;
        Name();
        while (v.at(vi) == ",") {
            read_token(",");
            Name();
            n++;
        }
        read_token(")");
        build_tree("read", n);
    }
    else if (v.at(vi) == "exit") {
        read_token("exit");
        build_tree("exit", 0);
    }
    else if (v.at(vi) == "return") {
        read_token("return");
        Expression();
        build_tree("return", 1);
    }
}

void Parser::OutExp() {}

void Parser::StringNode() {}

void Parser::Caseclauses() {}

void Parser::Caseclause() {}

void Parser::CaseExpression() {}

void Parser::OtherwiseClause() {}

void Parser::Assignment() {}

void Parser::ForStat() {}

void Parser::ForExp() {}

void Parser::Expression() {}

void Parser::Term() {}

void Parser::Factor() {}

void Parser::Primary() {}

void Parser::Name() {
    if (isValidIdentifier(v.at(vi))) {
        build_tree(v.at(vi), 0);
        read_token("<identifier>");
        build_tree("<identifier>", 1);
    }
    else {
        std::cout << "Error in Name()" << std::endl;
    }
}

void Parser::parse() {
    Tiny();
    root = stack.top();

}






int main(int argc, char const* argv[]) {
    Parser parser;

    if (argc>1) {
        std::string flagSwitch = argv[1];

        if (flagSwitch == "-ast") {

            parser.tokenize(argv[2]);

            for (unsigned long i=0; i<parser.v.size(); i++)
                std::cout << parser.v.at(i) << std::endl;

//            parser.parse();
//
//            printTree(0, parser.root);

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

// + - * / = , ; ()
bool isSingleSymbol(const char &c) {
    return (c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c==','||c==';'||c=='=');
}

bool isIdentifierChar(const char &c) {
    return isLetter(c) || isNumber(c) || c=='_';
}

bool isValidIdentifier(std::string s) {
    if (isLetter(s.at(0)) || s.at(0) == '_') {
        for (unsigned long i=1; i<s.length(); i++) {
            if (!isIdentifierChar(s.at(i)))
                return false;
        }
        return true;
    }
    else
        return false;
}

bool isValidChar(std::string s) {
    if (s.length()>2) {

        if (s.at(0) == '\'') {

            if (s.at(1) != '\'') {

                if (s.at(2) == '\'')
                    return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

bool isValidString(std::string s) {
    if (s.length() > 2) {

        if (s.at(0) == '\"') {

            for (int i=0; i<s.length()-1; i++) {
                if (s.at(i) == '\"')
                    return false;
            }

            if (s.at(s.length()-1) == '\"')
                return true;
        }
        else return false;
    }
    else return false;
}

void printTree(int depth, Node *n) {
    if (n) {
        int x = n->childCount();
        for (int i = 0; i < depth; ++i)
            std::cout << ". ";
        std::cout << n->token << "(" << x << ")" << std::endl;
        printTree(depth+1, n->left);
        printTree(depth+1, n->right);
    }
}