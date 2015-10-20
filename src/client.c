/*
 ============================================================================
 Name        : client.c
 Author      : HA Engelbrecht
 Version     :
 Copyright   : Your copyright notice
 Description : Client application - Part 1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "client.h"

char *convertToString(const JSON_Value *rootValue);
char* dec2Hex4bit(int dec);
char* dec2Hex16bit(int dec);

char decDigit2Hex(int dec);

int getStringLength(const char *string);
int hexDigit2Dec(char hexDigit);
int login(char* sessionID, char* encryptionKey);

void appendToLogFile(int level, const char *location, const char *part1, const char *part2);
void generateChecksum(char *checksum, char *message);
void generateEncryptionKey(char *key, char *sessionID, char *username, char *password);
void encryptMessage(char *encryptedMessage, char *message, char *encryptionKey);
void decryptMessage(char *decryptedMessage, char *message, char *encryptionKey);
void menu();
void why(const char* sessionID, const char* encryptionKey);

// Main function used for testing functions
int main(void)
{
	setbuf(stdout,0);
	testJSONUtils();
	testEncryptionUtils();
	testHttpUtils();
	return 0;
}


/**************************************************
 * Part 1
 **************************************************/
//3.4.1 Creating JSON objects
//******************************************************************
JSON_Value* createJSONObject()
{
	return json_value_init_object();
}


//3.4.2 Read value for a specified name from a given JSON object
//******************************************************************
const char* getValueFromName(const JSON_Value *rootValue, const char *name)
{
	JSON_Object *rootObject = json_value_get_object(rootValue);
	return json_object_dotget_string(rootObject, name);
}


//3.4.3 Write name/value pair to JSON object
//******************************************************************
void setNameValuePair(const JSON_Value *rootValue, const char *name, const char *value)
{
	JSON_Object *rootObject = json_value_get_object(rootValue);
	json_object_set_string(rootObject, name, value);
}


//3.4.4 Get string representation of a JSON object
//******************************************************************
char *convertToString(const JSON_Value *rootValue)
{
	return json_serialize_to_string_pretty(rootValue);
}


//3.4.5 Determine length of string (character array)
//******************************************************************
int getStringLength(const char *string)
{
	int length = 0; //initialise a counter
	while(string[length] != '\0') //while there is another letter
	{
		length++; //increment the counter
	}

	return length;
}


//3.4.6 Remove white space from a string (character array)
//******************************************************************
void removeWhiteSpace(char *stringNoSpace, const char *string)
{
	int stringCount = 0, noSpaceStringCount = 0; //initialise counters to keep track of where you are in each string
	while(string[stringCount] != '\0') //while the original string has a next letter
	{
		if(string[stringCount] != ' ' && string[stringCount] != '\t' && string[stringCount] != '\n') //if the current character is not white space
		{
			stringNoSpace[noSpaceStringCount] = string[stringCount]; //then add it to the new string
			noSpaceStringCount++; //look at the next position in the new string
		}
		stringCount++; //look at the next position in the original string
	}

	stringNoSpace[noSpaceStringCount] = '\0'; //add a terminating char at the end of the new string
}


//3.4.7 Convert hexadecimal digit to a decimal number
//******************************************************************
int hexDigit2Dec(char hexDigit)
{
			if(hexDigit >= '0' && hexDigit <= '9')	return (int)hexDigit - 48; //the hex is a number subtract 48 to take the ASCII to its integer value
	else 	if(hexDigit >= 'A' && hexDigit <= 'F')	return (int)hexDigit - 55; //if the hex is an upper case letter subtract 55 to take it to its integer value
	else	if(hexDigit >= 'a' && hexDigit <= 'f')	return (int)hexDigit - 87; //if the hex is an lower case letter subtract 87 to take it to its integer value
	else	printf("error with hexDigit2Dec, %c is not hex", hexDigit);

	return 0;
}


/**************************************************
 * Part 2
 **************************************************/
//4.4.1 Append a line describing an event to a log file
//******************************************************************
void appendToLogFile(int level, const char *location, const char *part1, const char *part2)
{
	FILE *file = fopen("log.txt", "a"); //open the log.txt file in append mode
	char *levelS; //create a string to store the string representation of the error code

			if(level == 0) 	levelS = "ERROR";
	else 	if(level == 1) 	levelS = "WARNING";
	else 	if(level == 2)	levelS = "INFO";
	else 	printf("error - level %d not defined", level);

	fprintf(file, "[%s] in %s : %s %s \n", levelS, location, part1, part2); //write the string rep. to the file log.txt
	fclose(file); //close the file
}


//4.4.2 Calculate a checksum of a string (character array)
//******************************************************************
void generateChecksum(char *checksum, char *message)
{
	int  i = 0, value = 0, sum = 0; //create various integers to strore values for the calculations

	//instert code to remove white space from message but strore it in new variable messageNWS
	char *messageNWS = malloc(sizeof(char) * getStringLength(message));
	removeWhiteSpace(messageNWS, message); //remove the white space

	while(messageNWS[i] != '\0') //while the message has another letter ie, stop if you get to the terminating character '\0'
	{ //this is an algorithim that the Project page gives
		value = 65521 * ((int) messageNWS[i]); //convert the next letter to a number and multiply it with 65521
		sum = (sum + value) % 65535; // add the sum to the total and modulus it with 65535
		i++; //increment counter
	}
	//insert the code to convert the sum to hex and strore it tin the checksum char pointer
	checksum = dec2Hex4bit(sum);
	free(messageNWS);
}

char decDigit2Hex(int dec)
{
			if(dec >= 0 && dec <= 9)	return (char)(dec + 48); //if the decimal is between 0 and 9 add 48 to convert it to the ASCII or char representation of that number
	else 	if(dec >= 10 && dec <= 15)	return (char)(dec + 55); //if the decimal is between 10 and 15 add 55 to take it to the letter representations (A, B, C, D, E, F)
	else 	printf("error with decDigit2Hex, %d is not convertible to hex", dec);

	return '0';
}

char* dec2Hex4bit(int dec)
{
	char* hex = malloc(sizeof(char) * 4); //create the 4 bit hex number in string format
	int count = 0; //initialise a counter

	for(; count < 4; count++)	hex[count] = '0'; //fill the number with default values

	count = 0;

	while(dec > 0) //while dec is bigger that 0 --> convert what is left to hex
	{
		if(count > 3) //there are only 4 bits to store the hex, thus if the counter goes above 3 the decimal number is to big
		{
			printf("Overflow has occurred in dec2Hex4bit, %d is too big to be represented in 4 bits of hex", dec);
			dec = 0;
			break;
		}
		else
		{
			hex[3 - count] = decDigit2Hex(dec % 16); //convert the remainder of dec / 16 into hex and store it in the hex char*
			dec /= 16; //divide dec by 16 so that it gets smaller and you can calculate the next bit
			count++;
		}
	}

	return hex;
}


//4.4.3 Generate an encryption key
//******************************************************************
void generateEncryptionKey(char *key, char *sessionID, char *username, char *password)
{
	unsigned long sum = 0, i = 0, value = 0; //initialize various ints
	char *bigString = malloc((getStringLength(sessionID) + getStringLength(username) + getStringLength(password)) * sizeof(char));
	//sscanf(bigString, "%s%s%s", sessionID, username, password); //this should put all the strings together into 1 string
	sprintf(bigString, "%s%s%s", sessionID, username, password);

	while(bigString[i] != '\0') //this is an algorithm that the Project page gives
	{
		value = 18014398509481984 * ((int) bigString[i]);
		sum = (sum + value) % 9146744073709551614;
		i++;
	}
	free(bigString);
	//insert the code to convert the sum to hex and store it in the key char pointer
	key = dec2Hex16bit(sum);
}

char* dec2Hex16bit(int dec)
{
	char* hex =  malloc(sizeof(char) * 16);
	int count = 0;

	for(; count < 4; count++)	hex[count] = '0'; //fill the number with default values

	count = 0;

	while(dec > 0)
		{
			if(count > 15) //there are only 16 bits to store the hex, thus if the counter goes above 3 the decimal number is to big
			{
				printf("Overflow has occurred in dec2Hex4bit, %d is too big to be represented in 16 bits of hex", dec);
				dec = 0;
				break;
			}
			else
			{
				hex[15 - count] = decDigit2Hex(dec % 16); //convert the remainder of dec / 16 into hex and store it in the hex char*
				dec /= 16; //divide dec by 16 so that it gets smaller and you can calculate the next bit
				count++;
			}
		}

	return hex;
}

//4.4.4 Encrypt a message
//******************************************************************
void encryptMessage(char *encryptedMessage, char *message, char *encryptionKey)
{
	int i = 0;
	while(message[i] != '\0') //while there is another letter in the message
	{
		encryptedMessage[i] = (char) ((int) message[i] + hexDigit2Dec(encryptionKey[i % 16])); 	//take the next letter in the message and add the dec value of the next letter in the encryption key
		i++;																					//(the key is only 16 digits long and must cycle - thus the % 16)
	}

}


//4.4.5 Decrypt an encrypted message
//******************************************************************
void decryptMessage(char *decryptedMessage, char *message, char *encryptionKey)
{
	int i = 0;
	while(message[i] != '\0') //while there is another letter in the message
	{
		decryptedMessage[i] = (char) ((int) message[i] - hexDigit2Dec(encryptionKey[i % 16]));	//take the next letter in the message and subtract the dec value of the next letter in the encryption key
		i++;																					//(the key is only 16 digits long and must cycle - thus the % 16)
	}
}


/**************************************************
 * Part 3
 **************************************************/
void generateAuthorisationContent(JSON_Value *content, const char *username)
{
	//store the necessary values in a JSON value
	setNameValuePair(content, "sessionID", "unauthed");
	setNameValuePair(content, "message", username);
}


void processAuthorisationResponse(char *sessionID, const JSON_Value *responseValue)
{
	//retrieve the sessionID recieved from the server and store it in a string
	sessionID = getValueFromName(responseValue, "sessionID");
}


void generateChallengeContent(JSON_Value* content, const char *sessionID, const char *message, const char* encryptionKey)
{
	//allocate memory to all needed strings
	char* encryptedMessage = malloc(sizeof(char) * getStringLength(message));
	char* checksum = malloc(sizeof(char) * 4);
	char* encodedMessage = malloc(sizeof(char) * getStringLength(message));

	//encrypt the message --> encode the message (convert back to ASCII) --> generate the check sum for the original message
	encryptMessage(encryptedMessage, message, encryptionKey);
	base64EncodeString(encodedMessage, encryptedMessage);
	generateChecksum(checksum, message);

	//store the sessionID, encoded message and checksum in a JSON Value
	setNameValuePair(content, "sessionID", sessionID);
	setNameValuePair(content, "message", encodedMessage);
	setNameValuePair(content, "checksum", checksum);

	//free all used variable as they have served their purpose
	free(encryptedMessage);
	free(encodedMessage);
	free(checksum);
}


int processChallengeResponse(const JSON_Value *responseValue)
{
	int result = 0; //initialise return value to false
	char* resultS = malloc(sizeof(char) * 7); //allocate memory for the json answer

	resultS = getValueFromName(responseValue, "result"); //retrieve the answer
	//decryptMessage(resultS, resultS); //************************************use different chars??? and necessary????
	//base64EncodeString(resultS, resultS);

	if(resultS[0] == 's') //if the first letter is an s then it must have been a "success"
		result = 1; //set return value to true

	free(resultS); //free the memory used by the json answer
	return result;
}


void generateWhyCommandContent(JSON_Value *content, const char* sessionID, const char* encryptionKey)
{
	//allocate memory for the string values that will be used in this function
	char* command = "why"; //why might be different
	char* encryptedCommand = malloc(sizeof(char) * getStringLength(command));
	char* checksum = malloc(sizeof(char) * 4);
	char* encodedCommand = malloc(sizeof(char) * getStringLength(command)); //is this necessary??????

	//encrypt the command --> encode the command (convert back to ASCII) --> generate the check sum for the original command
	encryptMessage(encryptedCommand, command, encryptionKey);
	base64EncodeString(encodedCommand, encryptedCommand);
	generateChecksum(checksum, command);

	//store the sessionID, encoded command and checksum in a JSON Value
	setNameValuePair(content, "sessionID", sessionID);
	setNameValuePair(content, "message", encryptedCommand);
	setNameValuePair(content, "checksum", checksum);

	//free all used memory in this function
	free(command);
	free(encryptedCommand);
	free(encodedCommand);
	free(checksum);
}


void processWhyCommandResponse(char *answer, const JSON_Value *responseValue, const char *encryptionKey)
{
	answer = getValueFromName(responseValue, "result"); //retrieve the answer

	if(!strcmp(answer, "failure")) //if answer is not equal to "failure"
	{
		base64DecodeString(answer, answer); //check if another string is needed*****************
		decryptMessage(answer, answer, encryptionKey);
	}
}

void menu()
{
	FILE* file = fopen("menu.txt", "r");
	char* menu = malloc(sizeof(char) * 125); //I know how many chars are in the file
	char input = '0';

	//scan the menu text in from a file
	fgets(menu, 124, file); //I know how many chars are in the file

	do
	{
		printf("%s", menu); //print the contents of the file
		scanf("%c", &input); //scan in the user input
	}while(input != '1' || input != '2' || input != 'Q' || input != 'q'); //repeat unless valid input is entered

	//check which operation should be performed next
			if(input == '1')
			{
				//must allocate memory and stuff
				login();
			}
	else 	if(input == '2')
			{
				//must allocate memory and stuff
				why();
			}

	free(menu);
	fclose(file);
	printf("quit must have been selected");
	exit(0); //if neither login or why was chosen then the program must end
}

int login(char* sessionID, char* encryptionKey)
{
	int result = 0;
	char username[20] = {'\0'};
	char password[50] = {'\0'};
	JSON_Value* responseValue; //need to initialise this???
	JSON_Value* json_value;

	//read the username from the keyboard
	printf("Enter your username: \n");
	scanf("%s", username);
	printf("Enter your password: \n");
	scanf("%s", password);

	//get the json value ready for the server to proccess it
	json_value = createJSONObject();
	generateAuthorisationContent(json_value, username);

	//get the server to process the value????
	postRequest(&responseValue, "ml.sun.ac.za", 8888, "/auth", convertToString(json_value));

	//process the response from the server
	processAuthorisationResponse(sessionID, responseValue);

	//generate the challenge content -- ask the user for the password and feed it in as the message??? -- generate an encryption key???
	generateChallengeContent(json_value, sessionID, password, encryptionKey);

	//get the response from the server
	postRequest(&responseValue, "ml.sun.ac.za", 8888, "/auth", convertToString(json_value));

	//process the response
	result = processChallengeResponse(responseValue);

	(result) ? printf("Login: Success!") : printf("Login: Failed!");
	return result;
}

void why(const char* sessionID, const char* encryptionKey)
{

}
