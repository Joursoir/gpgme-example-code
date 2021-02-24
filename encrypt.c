#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gpgme.h>

#include "routines.h"

const char gpg_key[] = "your-public-key";

int main()
{
	gpgme_error_t err;	
	gpgme_ctx_t ctx;
	gpgme_data_t plain, cipher;
	gpgme_key_t key[] = { NULL, NULL }; /* must be null-terminated! */
	gpgme_encrypt_result_t result;
	init_gpgme();

	err = gpgme_new(&ctx);
	exit_if_err(err);
	init_ctx(ctx, GPGME_PROTOCOL_OPENPGP);

	/* start encrypt */

	err = gpgme_data_new_from_mem(&plain, "Hello world!\n", 13, 0);
	exit_if_err(err);
	err = gpgme_data_new(&cipher);
	exit_if_err(err);

	err = gpgme_get_key(ctx, gpg_key, &key[0], 0);
	exit_if_err(err);

	err = gpgme_op_encrypt(ctx, key, GPGME_ENCRYPT_ALWAYS_TRUST,
		plain, cipher);
	exit_if_err(err);
	result = gpgme_op_encrypt_result(ctx);
	while(result->invalid_recipients) {
		fprintf(stderr, "Invalid recipient: %s\n"
			"%s: %s\n",
			result->invalid_recipients->fpr,
			gpgme_strsource(result->invalid_recipients->reason),
			gpgme_strerror(result->invalid_recipients->reason));
		result->invalid_recipients = result->invalid_recipients->next;
	}
	loop_read(cipher);
	
	gpgme_data_release(plain);
	gpgme_data_release(cipher);
	gpgme_release(ctx);
	return 0;
}