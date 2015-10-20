/*
 * utils.h
 *
 *  Created on: 21 Aug 2015
 *      Author: R. D. Beyers
 */

#ifndef UTILS_H_
#define UTILS_H_

// struct for string needed in "writefunc"
struct string;

// initializes the string struct needed for "writefunc"
void init_string(struct string *s);

// function for writing HTTP
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

// encodes 3 bytes into a 4-byte block
void encodeblock(const char *inS, char *outS, int len);

// decodes a block (4 bytes) into three bytes
void decodeblock(const char *inS, char *outS);

// does something secret
int secretFunction(const char *arr);

// base64 encodes a string
void base64EncodeString(char *encodedString, const char *string);

// base64 decodes a string
void base64DecodeString(char *decodedString, const char *string);

// performs the HTTP request and gets the response
void postRequest(JSON_Value **responseValue, const char *host, int port, const char *path, const char *request);
#endif /* UTILS_H_ */
