#include <stdio.h>

#include "libbcachefs/bcachefs.h"
#include "libbcachefs/alloc_foreground.h"
#include "libbcachefs/btree_iter.h"
#include "libbcachefs/buckets.h"
#include "libbcachefs/dirent.h"
#include "libbcachefs/errcode.h"
#include "libbcachefs/error.h"
#include "libbcachefs/fs-common.h"
#include "libbcachefs/inode.h"
#include "libbcachefs/io_read.h"
#include "libbcachefs/io_write.h"
#include "libbcachefs/opts.h"
#include "libbcachefs/super.h"

#define QSTR(n) { { { .len = strlen(n) } }, .name = n }


#include <yaml.h>

void key_function(struct bch_fs *c,struct bkey_s_c *k){


    switch (k->k->type) {
    case KEY_TYPE_inline_data:
    {
        struct bkey_s_c_inline_data d = bkey_s_c_to_inline_data(*k);
        //printf("key type %s %d bytes %d\n",bch2_bkey_types[k->k->type],k->k->size,bkey_inline_data_bytes(k->k));

    }
    break;

    case KEY_TYPE_extent:
    {
        struct bkey_s_c_extent d= bkey_s_c_to_extent(*k);
        printf("key type %s %p\n",bch2_bkey_types[k->k->type],d.v->start);

        struct printbuf buf = PRINTBUF;

        struct bkey_ptrs_c ptrs = bch2_bkey_ptrs_c(*k);
        const struct bch_extent_ptr *entry;


        bkey_for_each_ptr(ptrs,entry){
            bch2_extent_ptr_to_text(&buf,c,entry);

        }

        if (buf.buf != NULL){
            printf("%s\n",buf.buf);
        }
        printbuf_reset(&buf);


    }
 break;
    default:
        printf("key type %s %d\n",bch2_bkey_types[k->k->type],k->k->u64s);
        break;
    }


}

void _dump_subvoluime(struct bch_fs *c,struct btree_iter iter,struct bkey_s_c k){

    assert(k.k->type == KEY_TYPE_subvolume);


    struct bkey_s_c_subvolume sub = bkey_s_c_to_subvolume(k);

    //sub.v->creation_parent
    printf("subvol %d:%d:%d\n",iter.pos.inode,iter.pos.offset,iter.pos.snapshot);


}
void dump_subvolumes(struct bch_fs *c){
    struct bkey_s_c k;

     struct btree_trans *trans = bch2_trans_get(c);

    for_each_btree_key_upto(trans, iter, BTREE_ID_subvolumes,SUBVOL_POS_MIN, SUBVOL_POS_MAX,
                            0,k,({
                                 _dump_subvoluime(c,iter,k);
                0;
                            }));


    bch2_trans_put(trans);
}
int main()
{

    struct bch_opts opts = bch2_opts_empty();


    opt_set(opts, nochanges,        1);
    opt_set(opts, read_only,        1);
    opt_set(opts, norecovery,       1);
    opt_set(opts, degraded,         1);
    opt_set(opts, very_degraded,         1);

    char *argv1[] = {"/dev/nbd0"};

    struct bch_fs *c = bch2_fs_open(argv1,1, opts);

    if (IS_ERR(c))
        die("error opening devices: %s", bch2_err_str(PTR_ERR(c)));



    struct bkey_ptrs_c ptrs;
    struct btree_trans *trans = bch2_trans_get(c);
    struct btree_iter iter;

    struct bkey_s_c k;


    int ret = 0;

    printf("***\n");



/*
    for_each_btree_key_upto(trans, iter, BTREE_ID_extents,
                                        SPOS(0, 0, U32_MAX), SPOS(U64_MAX, U64_MAX,U32_MAX),
                                        0, k, ({
                key_function(c,&k);
                0;
                }));

*/

    dump_subvolumes(c);

    bch2_trans_put(trans);
    bch2_fs_stop(c);
    printf("***\n");
    return 0;
}
