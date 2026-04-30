/* 
 * neturoauth.h - describes the functions provided by the neturoauth library.
 */

#define MAXPWLEN 16
#define CHALLENGESIZE 16

void neturoEncryptBytes(unsigned char *bytes, char *passwd);
void neturoEncryptPassword(unsigned char *encryptedPasswd, char *passwd);
char *neturoDecryptPassword(const unsigned char *encryptedPasswd);

extern int neturoEncryptPasswd(char *passwd, char *fname);
extern char *neturoDecryptPasswd(char *fname);
extern void neturoRandomBytes(unsigned char *bytes);
