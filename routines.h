#ifndef GPGEXAMPLE_ROUTINES_H
#define GPGEXAMPLE_ROUTINES_H

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <errno.h>
#include <gpgme.h>

#define BUF_SIZE 256

#define exit_if_err(err) \
	do { \
		if(err != GPG_ERR_NO_ERROR) { \
			fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, \
				__LINE__, gpgme_strsource(err), gpgme_strerror(err)); \
			exit(1); \
		} \
	} while(0) \

void init_gpgme()
{
	/* The GPGME library communicates with child processes (the 
	crypto engines). If a child process dies unexpectedly, for
	example due to a bug, or system problem, a SIGPIPE signal
	will be delivered to the application. The default action is
	to abort the program. To protect against this,
	gpgme_check_version sets the SIGPIPE signal action to SIG_IGN
	which means that the signal will be ignored. */
	setlocale(LC_ALL, "");
	gpgme_check_version(NULL);
	gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));
#ifdef LC_MESSAGES
	gpgme_set_locale(NULL, LC_MESSAGES, setlocale(LC_MESSAGES, NULL));
#endif
}

void init_ctx(gpgme_ctx_t ctx, gpgme_protocol_t protocol)
{
	gpgme_error_t err;
	err = gpgme_engine_check_version(protocol);
	exit_if_err(err);
	err = gpgme_set_protocol(ctx, protocol);
	exit_if_err(err);
	/* output be ASCII armored */
	gpgme_set_armor(ctx, 1);
}

void loop_read(gpgme_data_t dh)
{
	char buf[BUF_SIZE + 1];
	int ret;

	ret = gpgme_data_seek(dh, 0, SEEK_SET);
	if(ret)
		exit_if_err(gpgme_err_code_from_errno(errno));
	while((ret = gpgme_data_read(dh, buf, BUF_SIZE)) > 0)
		fwrite(buf, ret, 1, stdout);
	if(ret < 0)
		exit_if_err(gpgme_err_code_from_errno(errno));

	/* Reset read position to the beginning */
	gpgme_data_seek(dh, 0, SEEK_SET);
}

#endif /* GPGEXAMPLE_ROUTINES_H */