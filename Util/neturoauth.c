/*
 * neturoauth.c - Functions for NeturoWV password management and authentication.
 */

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
*/

#include "neturoauth.h"
#include "d3des.h"


/*
 * We use a fixed key to store passwords, since we assume that our local
 * file system is secure but nonetheless don't want to store passwords
 * as plaintext.
 */

unsigned char fixedkey[8] = {23,82,107,6,35,78,88,7};
static unsigned char s_fixedkey[8] = {24,25,132,9,42,56,43,2};

/*
 * Encrypt CHALLENGESIZE bytes in memory using a password.
 */
int
neturoEncryptPasswd(char *passwd, char *encryptedPasswd)
{
    int i;
	
    /* pad password with nulls */
	
    for (i = 0; i < MAXPWLEN; i++) {
		if (i < (int)strlen(passwd)) {
			encryptedPasswd[i] = passwd[i];
		} else {
			encryptedPasswd[i] = 0;
		}
    }
	
    /* Do encryption in-place - this way we overwrite our copy of the plaintext
	password */
	
    deskey(s_fixedkey, EN0);
    //des(encryptedPasswd, encryptedPasswd); - by scyrie
	
	// by scyrie
	for (i = 0; i < MAXPWLEN; i += 8) {
		des(encryptedPasswd+i, encryptedPasswd+i);
    }
	// ~by scyrie
    return 8;
}

/*
*   Decrypt a password.  Returns a pointer to a newly allocated
*   string containing the password or a null pointer if the password could
*   not be retrieved for some reason.
*/
char *
neturoDecryptPasswd(char *inouttext)
{
	int i;
    unsigned char *passwd = (unsigned char *)malloc(MAXPWLEN+1);
	
    deskey(s_fixedkey, DE1);
	//    des(inouttext, passwd); - by scyrie
	
	for (i = 0; i < MAXPWLEN; i += 8) {
		des(inouttext+i, passwd+i);
    }
	
    passwd[MAXPWLEN] = 0;
	
    return (char *)passwd;
}

/*
*   Generate a set of random bytes for use in challenge-response authentication.
*/
void
neturoRandomBytes(unsigned char *where) {
	int i;
	unsigned int seed = (unsigned int) time(0);
	
	srand(seed);
	for (i=0; i < CHALLENGESIZE; i++) {
		where[i] = (unsigned char)(rand() & 255);    
	}
}

void
neturoEncryptBytes(unsigned char *bytes, char *passwd)
{
    unsigned char key[8];
    unsigned int i;

    /* key is simply password padded with nulls */

    for (i = 0; i < 8; i++) {
	if (i < strlen(passwd)) {
	    key[i] = passwd[i];
	} else {
	    key[i] = 0;
	}
    }

    deskey(key, EN0);

    for (i = 0; i < CHALLENGESIZE; i += 8) {
		des(bytes+i, bytes+i);
    }
}


/*
 * Encrypt a password into the specified space.
 * encryptedPasswd will be 8 bytes long - sufficient space 
 *   should be allocated.
 */

void
neturoEncryptPassword( unsigned char *encryptedPasswd, char *passwd )
{
	unsigned int i;

    /* pad password with nulls */
    for (i = 0; i < MAXPWLEN; i++) {
		if (i < strlen(passwd)) {
			encryptedPasswd[i] = passwd[i];
		} else {	
			encryptedPasswd[i] = 0;
		}
    }

    /* Do encryption in-place - this way we overwrite our copy of the plaintext
       password */
    deskey(fixedkey, EN0);
    des(encryptedPasswd, encryptedPasswd);
}


/*
 * Decrypt a password.  Returns a pointer to a newly allocated
 * string containing the password or a null pointer if the password could
 * not be retrieved for some reason.
 */

char *
neturoDecryptPassword(const unsigned char *encryptedPasswd)
{
    unsigned int i;
    unsigned char *passwd = (unsigned char *)malloc(MAXPWLEN+1);

	memcpy(passwd, encryptedPasswd, MAXPWLEN);

    for (i = 0; i < MAXPWLEN; i++) {
		passwd[i] = encryptedPasswd[i];
    }

    deskey(fixedkey, DE1);
    des(passwd, passwd);

    passwd[MAXPWLEN] = 0;

    return (char *)passwd;
}


