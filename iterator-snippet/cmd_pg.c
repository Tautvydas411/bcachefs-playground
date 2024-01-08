#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cmds.h"
#include "libbcachefs.h"
#include "tools-util.h"

#include "libbcachefs/bcachefs.h"
#include "libbcachefs/btree_iter.h"
#include "libbcachefs/btree_update.h"
#include "libbcachefs/errcode.h"
#include "libbcachefs/error.h"
#include "libbcachefs/journal_io.h"
#include "libbcachefs/journal_seq_blacklist.h"
#include "libbcachefs/super.h"
#include "libbcachefs/dirent.h"

static const char *NORMAL	= "\x1B[0m";
static const char *RED		= "\x1B[31m";

typedef DARRAY(struct bbpos) d_bbpos;
typedef DARRAY(enum btree_id) d_btree_id;

void v(struct btree_trans *trans,
         struct btree_iter *iter,
         struct bkey_s_c k
      )
{


  volatile struct bkey_s_c_dirent d = bkey_s_c_to_dirent(k);
  volatile struct qstr s= bch2_dirent_get_name(d);

  printf("dn %02x `%s'\n",d.v->d_type,s.name);
}
int pg_special(struct bch_fs *c)
{
    //struct btree_trans *trans = bch2_trans_get(c);
    //bch2_trans_begin(trans);

    int ret = bch2_trans_run(c,
                for_each_btree_key_commit(trans, iter, BTREE_ID_dirents,
                                POS_MIN,
                                BTREE_ITER_PREFETCH|BTREE_ITER_ALL_SNAPSHOTS, k,
                                NULL, NULL,
                                BCH_TRANS_COMMIT_no_enospc, ({
                        v(trans,&iter,k);0; // expects return value 0, otherwise iterator is stopped and treated as error
                })));


      bch_err_fn(c, ret);

      printf("\nret=%d\n",ret);
//    bch2_trans_put(trans);


}


int cmd_pg(int argc, char *argv[])
{
	static const struct option longopts[] = {
		{ "nr-entries",		required_argument,	NULL, 'n' },
		{ "transaction-filter",	required_argument,	NULL, 't' },
		{ "key-filter",		required_argument,	NULL, 'k' },
		{ "verbose",		no_argument,		NULL, 'v' },
		{ "help",		no_argument,		NULL, 'h' },
		{ NULL }
	};
	struct bch_opts opts = bch2_opts_empty();
	u32 nr_entries = U32_MAX;
	d_bbpos		transaction_filter = { 0 };
	d_btree_id	key_filter = { 0 };
	int opt;


	//opt_set(opts, nochanges,	true);
	//opt_set(opts, norecovery,	true);
	//opt_set(opts, read_only,	true);
	//opt_set(opts, degraded,		true);
	//opt_set(opts, errors,		BCH_ON_ERROR_continue);
	//opt_set(opts, fix_errors,	FSCK_FIX_yes);

	// it will crash, as there will be no tree read
	//opt_set(opts, keep_journal,	true);
	//opt_set(opts, read_journal_only,true);


	args_shift(optind);

	if (!argc)
		die("Please supply device(s) to open");

	darray_str devs = get_or_split_cmdline_devs(argc, argv);

	struct bch_fs *c = bch2_fs_open(devs.data, devs.nr, opts);
	if (IS_ERR(c))
		die("error opening %s: %s", argv[0], bch2_err_str(PTR_ERR(c)));


	pg_special(c);

	// lets do something
	bch2_fs_stop(c);
	return 0;
}
