/* $Id: hex.c 4384 2007-06-04 05:06:34Z ckuethe $ */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "gpsd_config.h"
#include "gpsd.h"

char /*@ observer @*/ *gpsd_hexdump(const void *binbuf, size_t binbuflen)
{
    static char hexbuf[MAX_PACKET_LENGTH*2+1];
#ifndef SQUELCH_ENABLE
    size_t i, j = 0;
    size_t len = (size_t)((binbuflen > MAX_PACKET_LENGTH) ? MAX_PACKET_LENGTH : binbuflen);
    const char *ibuf = (const char *)binbuf;
    const char *hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen) 
	return "";

    /*@ -shiftimplementation @*/
    for (i = 0; i < len; i++) {
	hexbuf[j++] = hexchar[ (ibuf[i]&0xf0)>>4 ];
	hexbuf[j++] = hexchar[ ibuf[i]&0x0f ];
    }
    /*@ +shiftimplementation @*/
    hexbuf[j] ='\0';
#else /* SQUELCH defined */
    hexbuf[0] = '\0';
#endif /* SQUELCH_ENABLE */
    return hexbuf;
}

int gpsd_hexpack(char *src, char *dst, int len){
    int i, k, l;

    l = (int)(strlen(src) / 2);
    if ((l < 1) || (l > len))
	return -1;

    bzero(dst, len);
    for (i = 0; i < l; i++)
	if ((k = hex2bin(src+i*2)) != -1)
	    dst[i] = (char)(k & 0xff);
	else
	    return -1;
    return l;
}

/*@ +charint -shiftimplementation @*/
int hex2bin(char *s)
{
    int a, b;

    a = s[0] & 0xff;
    b = s[1] & 0xff;

    if ((a >= 'a') && (a <= 'f'))
	a = a + 10 - 'a';
    else if ((a >= 'A') && (a <= 'F'))
	a = a + 10 - 'A';
    else if ((a >= '0') && (a <= '9'))
	a -= '0';
    else
	return -1;

    if ((b >= 'a') && (b <= 'f'))
	b = b + 10 - 'a';
    else if ((b >= 'A') && (b <= 'F'))
	b = b + 10 - 'A';
    else if ((b >= '0') && (b <= '9'))
	b -= '0';
    else
	return -1;

    return ((a<<4) + b);
}
/*@ -charint +shiftimplementation @*/
