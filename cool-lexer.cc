/*
 *  The lexical analyzer for COOL.
 */

#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <strings.h>
#include <boost/algorithm/string.hpp>

/* Max size of string constants */
#define MAX_STR_CONST 1025

extern FILE *fin; /* we read from this file */

extern int curr_lineno;
extern int verbose_flag;

int yy_flex_debug;

extern YYSTYPE cool_yylval;

/* read file character by character */
char next;
bool nread;

// Returns the next character
// without moving the file pointer
char lookNext()
{
	if(!nread)
	{
		next = fgetc(fin);
		nread = true;
	}

	return next;
}

// Returns the next character
// and moves the file pointer ahead
char getNext()
{
	char cur = lookNext();
	if(cur != EOF)
		next = fgetc(fin);
	else
		nread = false;
	return cur;
}

/*
 * Class Word
 * The class containing the string to be
 * processed.
 */
class Word{
public:
	Word():theWord(){

	};

	Word(std::string &inputword): theWord(inputword){

	};

	virtual ~Word(){

	};

	Word &operator= (const Word &rhs){
		if(this != &rhs){
			this->theWord = rhs.theWord;
		}
		return *this;
	};

	void readChar(char nxt){
		theWord.append(1,nxt);
	};

	int parseWord(){
		return ERROR;
	}

private:
	std::string theWord;
};
// Returns the next token
// Modify this function to
// return the correct tokens
int cool_yylex()
{
	Word word;
	while(true) {
		char nxt = lookNext();
		switch(nxt)
		{
		case ('\n'):

		case ('\f'):

		case ('\r'):

		case ('\t'):

		case ('\v'):

		case ('-'):
			nxt = getNext();
			word.readChar(nxt);
			nxt = getNext();
			word.readChar(nxt);
			if(nxt == '-'){
				while(true){
					nxt = getNext();
					if(nxt != '\n' && nxt != EOF){
						word.readChar(nxt);
						continue;
					}
					return LE;//FIXME : May change return type;
				}
			}
			else{
				cool_yylval.error_msg = "Invalid Char: " + '-';
				return ERROR;
			}
			break;

		case (' '):
			return word.parseWord();

		case EOF:
			getNext();
			return 0;

			/* everything else */
		default:
			char em[2];
			em[0] = getNext();
			em[1] = '\0';
			cool_yylval.error_msg = em;
			return ERROR;
		}
	}
}
