#include "jsmnex.h"

const char testJsonStr[] = "{\n  \"name\": \"jsmn\",\n  \"keywords\": \"json\",\n  \"description\": \"Minimalistic JSON parser/tokenizer in C. It can be easily integrated into resource-limited or embedded projects\",\n  \"repository\": {\n    \"type\": \"git\",\n    \"url\": \"https://github.com/zserge/jsmn.git\"\n  },\n  \"frameworks\": \"*\",\n  \"platforms\": \"*\",\n  \"examples\": [\n    \"example/*.c\"\n  ],\n  \"testArray\": [\n    {\n      \"type\": \"git\",\n      \"url\": \"https://github.com/zserge/jsmn.git\"\n    },\n    {\n      \"type\": \"baidu\",\n      \"url\": \"https://baidu.com\"\n    },\n    {\n      \"type\": \"taobao\",\n      \"url\": \"https://taobao.com\"\n    },\n    1,\n    2,\n    3,\n    [\n      {\n        \"type\": \"git\",\n        \"url\": \"https://github.com/zserge/jsmn.git\"\n      },\n      {\n        \"type\": \"baidu\",\n        \"url\": \"https://baidu.com\"\n      },\n      {\n        \"type\": \"taobao\",\n        \"url\": \"https://taobao.com\"\n      },\n      {\n        \"testObject\": {\n          \"frameworks\": \"*\",\n          \"platforms\": \"*\",\n          \"examples\": [\n            \"example/*.c\"\n          ]\n        }\n      }\n    ]\n  ],\n  \"exclude\": \"test\"\n}";
//Char array must larger than original json string.
char tmpStr [2000];
jsmnexInfo js;
int resInd;
int resErr;

main()
{
  memset(tmpStr, 0, sizeof(tmpStr));
  strcpy(tmpStr, testJsonStr);
  printf("%s\n", tmpStr);

  // init jsmnex
  js.jsonStr = tmpStr;
  jsmnexInit(&js);
  jsmnexParse(&js);
  if (js.tokenNum < 0)
  {
    printf("Failed to parse JSON: %d\n", js.tokenNum);
    return;
  }

  //print all tokens.
  for (int i = 0; i < js.tokenNum; i++)
  {
    printf("token%d: type=%d start=%d end=%d size=%d parent=%d.\n", i,
           js.token[i].type, js.token[i].start, js.token[i].end,
           js.token[i].size, js.token[i].parent);
    printf("%.*s\n", js.token[i].end - js.token[i].start,
           js.jsonStr + js.token[i].start);
  }

  // search and read token
  resInd = jsmnFindToken(&js, "ki", "testArray", 5);
  float resFloat = jsmnexReadFloat(&js, resInd, &resErr);
  printf("key = %d\n", resInd);
  printf("value = %f\n", resFloat);

  // modify token
  resErr = jsmnexWriteFloat(&js, resInd, 654.321);
  printf("resErr = %d\n", resErr);
  printf("%s\n", js.jsonStr);

  const char testWrite[] = "hello world.";
  resInd = jsmnFindToken(&js, "k", "description");
  resErr = jsmnexWriteStr(&js, resInd, testWrite, strlen(testWrite));
  printf("key = %d\n", resInd);
  printf("resErr = %d\n", resErr);
  printf("%s\n", js.jsonStr);

  while(1);
}
