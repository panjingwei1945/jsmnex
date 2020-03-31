#define JSMN_PARENT_LINKS
#include "jsmn.h"
#include "jsmnex.h"
#include "stdarg.h"
#include "printf.h"

struct {
  int parent;
  int mode;
  char * key;
  int keyLen;
  int index;
  int findFlag;
}jsmnexSInfo;

void jsmnexInit(jsmnexInfo * jExInfo)
{
  jsmn_init(&(jExInfo->p));
  jExInfo->tokenSize = TOKENSIZE;
  jExInfo->jsonStrLen = strlen(jExInfo->jsonStr);
}

int jsmnexParse(jsmnexInfo * jExInfo)
{
  jExInfo->tokenNum = jsmn_parse(&(jExInfo->p), jExInfo->jsonStr, jExInfo->jsonStrLen, jExInfo->token, jExInfo->tokenSize);
  return jExInfo->tokenNum;
}
/**
  * @brief  use variable arguments to get token index.
  * @param  str : string pointer contain the json data.
  * @param  t : token array pointer contain the parsed information.
  * @param  tNum : Number of t array.
  * @param  argsNum : Number of variable arguments.
  * @param  ... : variable arguments. only accept string pointer.
  * @retval DRESULT: > 0 indicate the index of t that contain the information of the value looking for. < 0 means something error.
  */
//int jsmnFindToken(char * str, jsmntok_t * t, int tNum, const char * format, ...)
int jsmnFindToken(jsmnexInfo * jExInfo, const char * format, ...)
{
//  jExInfo->token
//  jExInfo->tokenNum
//  jExInfo->jsonStr
  char *tmpTokenStr;
  va_list ap;

//  jsmnSearchInfo jsmnexSInfo;
  jsmnexSInfo.findFlag = 0;
  jsmnexSInfo.mode = JSMN_FIND_KEY;
  jsmnexSInfo.parent = 0;

  if(strlen(format) > 255) return JSMN_ERROR_KEY_WORD_TOO_LONG;

// Format only contain KkIi.
  for (int i = 0; format[i] != '\0'; i++)
  {
    switch (format[i])
    {
      case 'K':
      case 'k':
      case 'I':
      case 'i':
        break;
      default:
        return JSMN_ERROR_FORMAT_ERROR;
    }
  }

//       First key must be Kk.
  switch (*format)
  {
    case 'K':
    case 'k':
      break;
    default:
      return JSMN_ERROR_FORMAT_ERROR;
  }

  va_start(ap, format);
  //First arg must be a string.
  jsmnexSInfo.key = va_arg(ap, char*);
  jsmnexSInfo.keyLen = strlen(jsmnexSInfo.key);
  for (int i = 0; i < jExInfo->tokenNum; i++)
  {
    //type == JSMN_STRING and size != 0 means it is a key.
    if(jsmnexSInfo.mode == JSMN_FIND_KEY && jExInfo->token[i].size != 0
        && jExInfo->token[i].parent == jsmnexSInfo.parent && jExInfo->token[i].type == JSMN_STRING
        && jsmnexSInfo.keyLen == (jExInfo->token[i].end - jExInfo->token[i].start))
    {
      tmpTokenStr = jExInfo->jsonStr + jExInfo->token[i].start;
      jsmnexSInfo.findFlag = 1;
      for (int s = 0; s < jsmnexSInfo.keyLen; s++)
      {
        if(jsmnexSInfo.key[s] != tmpTokenStr[s])
        {
          jsmnexSInfo.findFlag = 0;
          break;
        }
      }
      if(jsmnexSInfo.findFlag) jsmnexSInfo.parent = i + 1;
    }
    else if(jsmnexSInfo.mode == JSMN_FIND_INDEX && jExInfo->token[i].parent == jsmnexSInfo.parent)
    {
      jsmnexSInfo.index--;
      if(jsmnexSInfo.index < 0)
      {
        jsmnexSInfo.findFlag = 1;
        jsmnexSInfo.parent = i;
      }
    }

    if(jsmnexSInfo.findFlag)
    {
      jsmnexSInfo.findFlag = 0;
      //Finish string compare and find right key.
      format++;
      if(*format == '\0')
      {
        if(jsmnexSInfo.mode == JSMN_FIND_INDEX)
        {
          va_end(ap);
          return i;
        }
        else
        {
          va_end(ap);
          return i + 1;
        }
      }

      switch (*format)
      {
        case 'K':
        case 'k':
          jsmnexSInfo.key = va_arg(ap, char*);
          jsmnexSInfo.keyLen = strlen(jsmnexSInfo.key);
          jsmnexSInfo.mode = JSMN_FIND_KEY;
          break;
        case 'I':
        case 'i':
          jsmnexSInfo.index = va_arg(ap, int);
          jsmnexSInfo.mode = JSMN_FIND_INDEX;
          break;
        default:
          va_end(ap);
          return JSMN_ERROR_FORMAT_ERROR;
      }
    }
  }
  va_end(ap);
  return JSMN_ERROR_NOT_FOUND;
}

int str2f(char * str, int strLen,float *res)
{
  double tmpRes = 0;
  char tmpStr[30] = "";
  int sign = 1;
  double mult = 10;

  if(strLen>29) return JSMN_ERROR_KEY_WORD_TOO_LONG;
  memcpy(tmpStr, str, strLen);
  tmpStr[strLen] = '\0';

//remote all non-digit characters.
  for(int i=0;i<=strLen;i++)
  {
    if(str[i] == '\0') break;
    switch(str[i]){
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
        break;
      default:
        //remote all non-digit characters.
        for(int j=0;j<=(strLen - i);j++)
        {
           str[i + j] = str[i + j + 1] ;
        }
        break;
    }
  }

  for(int i=0;i<strLen;i++)
  {
    if(str[i] == '\0') break;
    switch(str[i]){
      case '-':
        sign = -1;
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if(mult == 10)
          {
            tmpRes = tmpRes * 10 + str[i] - '0';
          }
        else
          {
            tmpRes = tmpRes + (str[i] - '0') * mult;
            mult = mult * 0.1;
          }
        break;
      case '.':
        mult = 0.1;
        break;
      default:
        return JSMN_ERROR_FORMAT_ERROR;
        break;
    }
  }
  * res = tmpRes;
  return 0;
}

float jsmnexStr2f(char * str)
{
  float res;
  str2f(str, strlen(str), &res);
  return res;
}

float jsmnexReadFloat(jsmnexInfo * jExInfo, int tokenInd, int * type)
{
  float tmpRes;
  if(jExInfo->token[tokenInd].type == JSMN_PRIMITIVE)
  {
    switch (jExInfo->jsonStr[jExInfo->token[tokenInd].start])
    {
      case 't':
        * type = JSMNEX_TRUE;
        return 1;
      break;
      case 'f':
        * type = JSMNEX_FALSE;
        tmpRes = 0;
        return 0;
      break;
      case 'n':
        * type = JSMNEX_NULL;
        tmpRes = 0;
        return 0;
      break;
      default:
        * type = JSMNEX_NUM;
        str2f(jExInfo->jsonStr + jExInfo->token[tokenInd].start, jExInfo->token[tokenInd].end - jExInfo->token[tokenInd].start, &tmpRes);
        return tmpRes;
        break;
    }
  }
  else {
        * type = JSMN_ERROR_NOT_FLOAT;
    return 0;
  }
}

int strShift(char * str, int strLen, int start, int shift)
{
  if(strLen <= start || start < 0 || strLen < 0) return JSMN_ERROR_STR_SHIFT_ERROR;

  if(shift == 0)
  {
  }
  else if(shift > 0)
  {
    for (int i = strLen - 1; i >= start; i--)
    {
      str[i + shift] = str[i];
    }
  }
  else if(shift < 0)
  {
    for (int i = start; i <= strLen - 1; i++)
    {
      str[i] = str[i - shift];
    }
  }
  strLen = strLen + shift;
  str[strLen] = '\0';
  return strLen;
}

int jsmnexWriteStr(jsmnexInfo * jExInfo, int ind, char * wStr, int wStrLen)
{
  int shift = wStrLen + jExInfo->token[ind].start - jExInfo->token[ind].end;
  char * tmpStr = jExInfo->jsonStr;
  int tmpStrLen = jExInfo->jsonStrLen;
  tmpStrLen = strShift(tmpStr, tmpStrLen, jExInfo->token[ind].start, shift);
  if(tmpStrLen < 0) return JSMN_ERROR_STR_SHIFT_ERROR;
  jExInfo->jsonStrLen = tmpStrLen;
  for(int i =  0 ;i<wStrLen; i++)
  {
    if('\0' == wStr[i]) break;
    tmpStr[i + jExInfo->token[ind].start] = wStr[i];
  }
  jsmnexInit(jExInfo);
  jsmnexParse(jExInfo);
  return tmpStrLen;
}

int jsmnexWriteInt(jsmnexInfo * jExInfo, int ind, int val)
{
  char tmpStr[30];
  memset(tmpStr, 0, 30);
  int tmpStrLen = sprintf(tmpStr, "%d", val);
  return jsmnexWriteStr(jExInfo, ind, tmpStr, tmpStrLen);
}

int jsmnexWriteFloat(jsmnexInfo * jExInfo, int ind, float val)
{
  char tmpStr[30];
  memset(tmpStr, 0, 30);
  int tmpStrLen = sprintf(tmpStr, "%f", val);
  return jsmnexWriteStr(jExInfo, ind, tmpStr, tmpStrLen);
}


int jsmnexWriteTrue(jsmnexInfo * jExInfo, int ind)
{
  return jsmnexWriteStr(jExInfo, ind, "true", 4);
}

int jsmnexWriteFalse(jsmnexInfo * jExInfo, int ind)
{
  return jsmnexWriteStr(jExInfo, ind, "false", 5);
}

int jsmnexWriteNull(jsmnexInfo * jExInfo, int ind)
{
  return jsmnexWriteStr(jExInfo, ind, "null", 4);
}




