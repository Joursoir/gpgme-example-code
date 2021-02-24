#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gpgme.h>

#include "routines.h"

/* The only format supported for now is "internal" */
const char key_info[] = "<GnupgKeyParms format=\"internal\">\n"
						"Key-Type: default\n"
						"Subkey-Type: default\n"
						"Name-Real: Joe Tester\n"
						"Name-Comment: with stupid passphrase\n"
						"Name-Email: joe@foo.bar\n"
						"Expire-Date: 0\n"
						"Passphrase: abc\n"
						"</GnupgKeyParms>\n";

int main()
{
	gpgme_error_t err;	
	gpgme_ctx_t ctx;
	gpgme_genkey_result_t result;
	init_gpgme();

	err = gpgme_new(&ctx);
	exit_if_err(err);
	init_ctx(ctx, GPGME_PROTOCOL_OPENPGP);

	/* start generate key */
	/* You can find the gpgme_op_createkey function in the GPGME
	documentation. This func equals to gpg --quick-generate-key */

	/* GPG does not support public and secret, they should be NULL.
	GnuPG will generate a key pair and add it to the standard key ring.*/
	err = gpgme_op_genkey(ctx, key_info,
		/*public*/ NULL, /*private*/ NULL);
	exit_if_err(err);
	result = gpgme_op_genkey_result(ctx);

	printf("Key: %s, %s, %s\n", result->fpr,
		result->primary ? "primary" : "no primary",
		result->sub ? "sub" : "no sub");

	gpgme_release(ctx);
	return 0;
}
