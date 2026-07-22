#include <openssl/err.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>



int main(int argc, char** argv) {
	int port = atoi(argv[1]);
	SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
	SSL_CONF_CTX *cctx = SSL_CONF_CTX_new();

	char buf[BUFSIZ];

	SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_SERVER);
	SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_CERTIFICATE);
	SSL_CONF_CTX_set_ssl_ctx(cctx, ctx);

	BIO *ssl_bio = BIO_new_ssl(ctx, 0);
	BIO *input_stream = NULL;
	BIO *tmp = NULL;

	if((input_stream = BIO_new_accept(argv[1])) == NULL) { // creates stream (without calling accept())
		perror("BIO_new_accept() failed");
		exit(EXIT_FAILURE);
	}

	/*
	 * BIO_set_accept_bios() call pushes ssl_bio contents
	 * into input_stream object 
	 */ 

	BIO_set_accept_bios(input_stream, ssl_bio);
	ssl_bio = NULL;

	if(BIO_do_accept(input_stream) <= 0) {
		perror("accept() failed");
		exit(EXIT_FAILURE);
	}

	 
	ssize_t i = 0;
	for(;;) {
		
		/*
		 * BIO_read() calls accept() in case 
		 * BIO_do_accept() has been called only once
		 */

		if((i = BIO_read(input_stream, buf, 512)) > 0) {
			fwrite(buf, 1, i, stdout);
			fflush(stdout);
		}

		if(i == 0) {
			printf("Connection closed\n");
			tmp = BIO_pop(input_stream);
			BIO_free_all(tmp);
		}

		if(i < 0) perror("BIO_read() failed");

	}

	if(input_stream != NULL) BIO_free(input_stream);
	if(ssl_bio != NULL) BIO_free(ssl_bio);


	exit(EXIT_SUCCESS);
}

