#include <openssl/err.h>
#include <openssl/ssl.h>
#include <string.h>


int main(int argc, char** argv) { 
	SSL_CTX *context = SSL_CTX_new(TLS_client_method());
	SSL_CONF_CTX *context_config = SSL_CONF_CTX_new();

	SSL_CONF_CTX_set_flags(context_config, SSL_CONF_FLAG_CLIENT);
	SSL_CONF_CTX_set_ssl_ctx(context_config, context);


	/*
	 * cmd arguments parsing using SSL_CONF_cmd_argv()
	 * and then
	if(!SSL_CONF_CTX_finish(cctx)) { // context configuration finished
		printf("finish failed\n");
		exit(EXIT_FAILURE);
	}
	*/



	

