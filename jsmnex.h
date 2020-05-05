#ifndef JSMNEX_H
#define JSMNEX_H

#define JSMN_HEADER
#define JSMN_PARENT_LINKS
#include "jsmn.h"

#ifndef TOKENSIZE
//Default token array size is 128
#define TOKENSIZE 128
#endif

typedef struct{
  jsmn_parser p;
  char * jsonStr;
  int jsonStrLen;
  int tokenNum;
  jsmntok_t token[TOKENSIZE];
  int tokenSize;
}jsmnexInfo;


/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
enum jsmnextype_t{
  JSMNEX_UNDEFINED = 0,
  JSMNEX_OBJECT = 1,
  JSMNEX_ARRAY = 2,
  JSMNEX_STRING = 3,
  JSMNEX_PRIMITIVE = 4,
  JSMNEX_NUM = 5,
  JSMNEX_TRUE = 6,
  JSMNEX_FALSE = 7,
  JSMNEX_NULL = 8,
} ;


enum jsmnexerr{
  JSMN_ERROR_NOT_FOUND = -11,
  JSMN_ERROR_KEY_WORD_TOO_LONG = -12,
  JSMN_ERROR_FORMAT_ERROR = -13,
  JSMN_ERROR_NOT_FLOAT = -14,
  JSMN_ERROR_STR_SHIFT_ERROR = -15,
  JSMN_ERROR_STR_LENGTH_MISMATCH = -16,
  JSMN_ERROR_TYPE_MISMATCH = -17,
};

enum jsmnexSearchMode{

  JSMN_FIND_KEY = 10,
  JSMN_FIND_INDEX = 11
};

/**
 * Run Find Token by variable argument
 */
extern int jsmnexInit(jsmnexInfo * jExInfo, char * jsStr);
extern int jsmnexFindToken(jsmnexInfo * jExInfo, const char * format, ...);
extern float jsmnexReadFloat(jsmnexInfo * jExInfo, int tokenInd, int * type);
extern float jsmnexStr2f(char * str);
extern int jsmnexWriteStr(jsmnexInfo * jExInfo, int ind, char * wStr, int wStrLen);
extern int jsmnexWriteInt(jsmnexInfo * jExInfo, int ind, int val);
extern int jsmnexWriteFloat(jsmnexInfo * jExInfo, int ind, float val);
extern int jsmnexWriteTrue(jsmnexInfo * jExInfo, int ind);
extern int jsmnexWriteFalse(jsmnexInfo * jExInfo, int ind);
extern int jsmnexWriteNull(jsmnexInfo * jExInfo, int ind);

#endif /* JSMNEX_H */
