#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstring>
#include <iostream>
#include <vector>
#include "Color.hpp"

typedef struct {
    std::string token;
    std::string delim;
    std::string color;
    bool notNull;
}Token;

std::string keywords[] = {
				     "Asm",
				     "auto",
				     "bool",
				     "break",
				     "case",
				     "catch",
				     "char",
				     "class",
				     "const_cast",
				     "continue",
				     "default",
				     "delete",
				     "do",
				     "double",
				     "else",
				     "enum",
				     "dynamic_cast",
				     "extern",
				     "float",
				     "for",
				     "union",
				     "unsigned",
				     "using",
				     "friend",
				     "goto",
				     "if",
				     "inline",
				     "int",
				     "long",
				     "mutable",
				     "virtual",
				     "namespace",
				     "new",
				     "operator",
				     "private",
				     "protected",
				     "public",
				     "register",
				     "void",
				     "reinterpret_cast",
				     "return",
				     "short",
				     "signed",
				     "sizeof",
				     "static",
				     "static_cast",
				     "volatile",
				     "struct",
				     "switch",
				     "template",
				     "this",
				     "throw",
				     "try",
				     "typedef",
				     "typeid",
				     "unsigned",
				     "wchar_t",
				     "while",
				     "#include",
				     "#define",
				     "#ifndef",
				     "#endif",
				     "include",
				     "define",
				     "ifndef",
				     "endif",
				     "#"
};

bool isIn (std::string list[], std::string str) {
    for (int i = 0 ; i < 67; i++) {
	if (list[i] == str) {
	    return true;
	}
    }
    return false;
}

Token setColor(Token t) {
    if (isIn(keywords,t.token)){
	t.color = "#F92672";
	return t;
    }
    if (std::regex_match(t.token, std::regex("\".*\""))){
	t.color = "#E6DB74";
	return t;
    }
    if (t.token == "true" || t.token == "false") {
	t.color = "#AE81FF";
	return t;
    }
    if (t.delim == "(") {
	t.color = "#66D9EF";
	return t;
    }
    if (t.delim == "*") {
	t.color = "#66D9EF";
	return t;
    }
    if (std::regex_match(t.token, std::regex("<.*>"))) {
	t.color = "#E6DB74";
	return t;
    }
    t.color = "#FFFFFF";
    return t;
}

class Tokenizer {
private:
    std::string s,d;
    bool inquotes, sqstate, dqstate;
    int i;
public:
    Tokenizer(std::string str, std::string delims) : s(str),
						     d(delims),
						     i(0),
						     dqstate(false),
						     sqstate(false),
						     inquotes(false){}
    ~Tokenizer() {}
    Token Next() {
	std::string current_token = "";
	std::string current_delim = "";
	bool track = false;
	for (; i < s.length(); i++) {
	    if (inquotes) {
		if (s[i] == '"') {
		    if (!sqstate) {
			dqstate = !dqstate;
		    }
		}
		if (s[i] == '\'') {
		    if (!dqstate) {
			sqstate = !sqstate;
		    }
		}
	    }
	    if (!sqstate && !dqstate) {
		std::size_t found = d.find(s[i]);
		if (found != std::string::npos) {
		    current_delim = s[i];
		    i++;
		    track = true;
		    break;
		}
	    }
	    current_token += s[i];
	    if (i + 1 == s.length()) {
		track = true;
	    }
	}
	Token t;
	if ( track ) {
	    t.token = current_token;
	    t.delim = current_delim;
	    t.notNull = true;
	} else {
	    t.notNull = false;
	}
	return t;
    }
    void inQuotes(bool iq) {
	inquotes = iq;
    }
};

#endif
