/*
 * client.h
 *
 *  Created on: 12 Sep 2015
 *      Author: hebrecht
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "parson.h"

// Part 1
JSON_Value* createJSONObject();
void setNameValuePair(const JSON_Value *rootValue, const char *name, const char *value);
const char* getValueFromName(const JSON_Value *rootValue, const char *name);
char* convertToString(const JSON_Value *rootValue);
int getStringLength(const char *array);
void removeWhiteSpace(char *stringNoSpace, const char *string);
int hexDigit2Dec(char hexDigit);

#endif /* CLIENT_H_ */
