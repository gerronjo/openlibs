#if !defined(SOCKUTIL_H)
#define SOCKUTIL_H

int inet_addr(const char *sIp, unsigned long *lIp);

unsigned short htons(unsigned short port);

char *IpToString(char *ip, unsigned long lIp);

#endif
