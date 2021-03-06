#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <string.h>

#include "rocksock.h"
#include "rocksock_internal.h"

#ifndef ROCKSOCK_FILENAME
#define ROCKSOCK_FILENAME __FILE__
#endif

int rocksock_add_proxy(rocksock* sock, rs_proxyType proxytype, const char* host, unsigned short port, const char* username, const char* password) {
	rs_proxy* prx;
	if (!sock)
		return RS_E_NULL;
	if(!host)
		return rocksock_seterror(sock, RS_ET_OWN, RS_E_NULL, ROCKSOCK_FILENAME, __LINE__);
	if(proxytype == RS_PT_SOCKS4 && (username || password))
		return rocksock_seterror(sock, RS_ET_OWN, RS_E_SOCKS4_NOAUTH, ROCKSOCK_FILENAME, __LINE__);
	if(proxytype == RS_PT_SOCKS5 && ((username && strlen(username) > 255) || (password && strlen(password) > 255)))
		return rocksock_seterror(sock, RS_ET_OWN, RS_E_SOCKS5_AUTH_EXCEEDSIZE, ROCKSOCK_FILENAME, __LINE__);
	if(!sock->proxies)
		return rocksock_seterror(sock, RS_ET_OWN, RS_E_NO_PROXYSTORAGE, ROCKSOCK_FILENAME, __LINE__);
	size_t l = strlen(host);
	if(l > 255)
		return rocksock_seterror(sock, RS_ET_OWN, RS_E_HOSTNAME_TOO_LONG, ROCKSOCK_FILENAME, __LINE__);
	sock->lastproxy++;
	prx = &sock->proxies[sock->lastproxy];
	prx->hostinfo.port = port;
	prx->proxytype = proxytype;
	memcpy(prx->hostinfo.host, host, l+1);
	memcpy(prx->username, username?username:"", username?strlen(username)+1:1);
	memcpy(prx->password, password?password:"", password?strlen(password)+1:1);
	return rocksock_seterror(sock, RS_ET_OWN, 0, NULL, 0);
}

