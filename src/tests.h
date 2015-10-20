#ifndef TESTS_H_
#define TESTS_H_

void testJSONUtils();
void testEncryptionUtils();
void testHttpUtils();

// Part  1
int testCreateJSONObject();
int testSetNameValuePair();
int	testGetValueFromNameAndJSONObject();
int testConvertJSONToString();
int testGetStringLength();
int testRemoveWhiteSpace();
int testHexToDec();

// Part 2
int testAppendLineToLogFile();
int testGenerateHash();
int testGenerateEncryptionKey();
int testEncryptMessage();
int testDecryptMessage();

// Part 3
int testGenerateAuthorisationContent();
int testProcessAuthorisationResponse();
int testGenerateChallengeContent();
int testProcessChallengeResponse();
int testGenerateWhyCommandContent();
int testProcessWhyCommandResponse();

#endif /* TESTS_H_ */
