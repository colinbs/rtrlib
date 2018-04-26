/*
 * This file is part of RTRlib.
 *
 * This file is subject to the terms and conditions of the MIT license.
 * See the file LICENSE in the top level directory for more details.
 *
 * Website: http://rtrlib.realmv6.org/
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*#include <openssl/ec.h>*/
/*#include <openssl/ecdsa.h>*/
/*#include <openssl/obj_mac.h>*/
#include "rtrlib/rtrlib.h"

#ifdef BGPSEC

void create_key_test(void)
{
	EC_KEY *eckey;
	bgpsec_create_ec_key(&eckey);
	assert(eckey != NULL);
}

void create_signature_test(void)
{
	int rtval;

	EC_KEY *eckey;
	bgpsec_create_ec_key(&eckey);
	assert(eckey != NULL);

	unsigned char *str1 = "0123456789abcdef";
	unsigned char *str2 = "fedcba9876543210";

	ECDSA_SIG *signature1;
	ECDSA_SIG *signature2;

	rtval = bgpsec_create_ecdsa_signature("0123456789abcdef", &eckey, &signature1);
	assert(rtval == RTR_BGPSEC_SUCCESS);

	rtval = bgpsec_create_ecdsa_signature("fedcba9876543210", &eckey, &signature2);
	assert(rtval == RTR_BGPSEC_SUCCESS);

	EC_KEY_free(eckey);
	ECDSA_SIG_free(signature1);
	ECDSA_SIG_free(signature2);
}

void validate_signature_test(void)
{
	int rtval;
	enum bgpsec_result *result;

	EC_KEY *eckey;
	bgpsec_create_ec_key(&eckey);
	assert(eckey != NULL);

	unsigned char *str1 = "0123456789abcdef";
	unsigned char *str2 = "fedcba9876543210";

	ECDSA_SIG *signature1;
	ECDSA_SIG *signature2;

	// create the signatures.
	rtval = bgpsec_create_ecdsa_signature(str1, &eckey, &signature1);
	assert(rtval == RTR_BGPSEC_SUCCESS);

	rtval = bgpsec_create_ecdsa_signature(str2, &eckey, &signature2);
	assert(rtval == RTR_BGPSEC_SUCCESS);

	// validate the signatures.
	rtval = bgpsec_validate_ecdsa_signature(str1, &eckey, &signature1, &result);
	assert(rtval == RTR_BGPSEC_SUCCESS);
	assert(result == BGPSEC_VALID);

	rtval = bgpsec_validate_ecdsa_signature(str2, &eckey, &signature2, &result);
	assert(rtval == RTR_BGPSEC_SUCCESS);
	assert(result == BGPSEC_VALID);

	// validate the signatures with a wrong message.
	rtval = bgpsec_validate_ecdsa_signature("1a2b3c4d5e6f7890", &eckey, &signature1, &result);
	assert(rtval == RTR_BGPSEC_SUCCESS);
	assert(result == BGPSEC_NOT_VALID);

	rtval = bgpsec_validate_ecdsa_signature("a1b2c3d4e5f67890", &eckey, &signature2, &result);
	assert(rtval == RTR_BGPSEC_SUCCESS);
	assert(result == BGPSEC_NOT_VALID);

	EC_KEY_free(eckey);
	ECDSA_SIG_free(signature1);
	ECDSA_SIG_free(signature2);
}

// Taken from http://www.askyb.com/cpp/openssl-sha256-hashing-example-in-cpp/
static void ssl_test(void)
{
	// This is the input test string.
	char string[] = "Test String";
	// This is the expected result.
	char exp[] = "30c6ff7a44f7035af933babaea771bf177fc38f06482ad06434cbcc04de7ac14";
	// This is the array where the result of the SHA256 operation is stored in.
	// It is initialized to hold exactly the amount of characters that SHA256 produces.
	unsigned char digest[SHA256_DIGEST_LENGTH];

	// SHA256 takes the input string, the amount of characters to read (in this case
	// all) and the output array where to store the result in.
	SHA256((unsigned char*)&string, strlen(string), (unsigned char*)&digest);

	// Here is some debug output. First, print the integer representation of the
	// SHA256 result. Second, print the hex representation of the integer.
	// Do this with the first 5 positions.
	//    printf("Pos 0: %d, %02x\n", (unsigned int)digest[0], (unsigned int)digest[0]);
	//    printf("Pos 1: %d, %02x\n", (unsigned int)digest[1], (unsigned int)digest[1]);
	//    printf("Pos 2: %d, %02x\n", (unsigned int)digest[2], (unsigned int)digest[2]);
	//    printf("Pos 3: %d, %02x\n", (unsigned int)digest[3], (unsigned int)digest[3]);
	//    printf("Pos 4: %d, %02x\n", (unsigned int)digest[4], (unsigned int)digest[4]);

	// The result of the string representation has to be twice as large as the
	// SHA256 result array. This is because the hex representation of a single char
	// has a length of two, e.g. to represent the hex number 30 we need two characters,
	// "3" and "0".
	// The additional +1 is because of the terminating '\0' character.
	char result[SHA256_DIGEST_LENGTH*2+1];

	// Feed the converted chars into the result array. "%02x" means, print at least
	// two characters and add leading zeros, if necessary. The "x" stands for integer.
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	sprintf(&result[i*2], "%02x", (unsigned int)digest[i]);

	//printf("SHA256 digest: %s\n", result);
	// Assert the result string and the expected string.
	assert(strcmp(result, exp) == 0);
}

#endif

int main(void)
{
#ifdef BGPSEC
	create_key_test();
	create_signature_test();
	validate_signature_test();
	ssl_test();
	printf("Test successful\n");
#endif
	return EXIT_SUCCESS;
}
