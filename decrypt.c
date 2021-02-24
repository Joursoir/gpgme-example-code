#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gpgme.h>

#include "routines.h"

const char file_name[] = "encrypted-file";

int main()
{
	gpgme_error_t err;	
	gpgme_ctx_t ctx;
	gpgme_data_t cipher, plain;
	gpgme_decrypt_result_t result;
	init_gpgme();

	err = gpgme_new(&ctx);
	exit_if_err(err);
	init_ctx(ctx, GPGME_PROTOCOL_OPENPGP);

	/* start decrypt */

	/* If copy is not zero, the whole file is read in at
	initialization time and the file is not used anymore
	after that. This is the only mode supported currently. */
	err = gpgme_data_new_from_file(&cipher, file_name, /* copy */ 1);
	exit_if_err(err);
	err = gpgme_data_new(&plain);
	exit_if_err(err);

	err = gpgme_op_decrypt(ctx, cipher, plain);
	exit_if_err(err);
	result = gpgme_op_decrypt_result(ctx);
	if(result->unsupported_algorithm) {
		fprintf(stderr, "%s:%d: Unsupported algorithm: %s\n"
			"Wrong key usage: %s\n"
			"Legacy cipher nomdc: %s\n"
			"Is mime: %s\n"
			"Is de vs: %s\n"
			"File name: %s\n"
			"Symkey algo: %s\n",
			__FILE__, __LINE__,
			result->unsupported_algorithm,
			result->wrong_key_usage ? "yes" : "no",
			result->legacy_cipher_nomdc ? "yes" : "no",
			result->is_mime ? "yes" : "no",
			result->is_de_vs ? "yes" : "no",
			result->file_name ? result->file_name : "null",
			result->symkey_algo);
		return 1;
	}
	loop_read(plain);
	
	gpgme_data_release(cipher);
	gpgme_data_release(plain);
	gpgme_release(ctx);
	return 0;
}