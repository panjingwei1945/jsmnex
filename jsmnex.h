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
};

enum jsmnexSearchMode{

  JSMN_FIND_KEY = 10,
  JSMN_FIND_INDEX = 11
};

/**
 * Run Find Token by variable argument
 */
//JSMN_API int jsmnFindToken(char * str, jsmntok_t * t, int tNum, const char * format, ...);
JSMN_API void jsmnexInit(jsmnexInfo * jExInfo);
JSMN_API int jsmnexParse(jsmnexInfo * jExInfo);
JSMN_API int jsmnFindToken(jsmnexInfo * jExInfo, const char * format, ...);
JSMN_API float jsmnexReadFloat(jsmnexInfo * jExInfo, int tokenInd, int * type);
JSMN_API float jsmnexStr2f(char * str);
JSMN_API int jsmnexWriteStr(jsmnexInfo * jExInfo, int ind, char * wStr, int wStrLen);
JSMN_API int jsmnexWriteInt(jsmnexInfo * jExInfo, int ind, int val);
JSMN_API int jsmnexWriteFloat(jsmnexInfo * jExInfo, int ind, float val);
JSMN_API int jsmnexWriteTrue(jsmnexInfo * jExInfo, int ind);
JSMN_API int jsmnexWriteFalse(jsmnexInfo * jExInfo, int ind);
JSMN_API int jsmnexWriteNull(jsmnexInfo * jExInfo, int ind);

#endif /* JSMNEX_H */
