#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "tinyfs.h"

struct file_blk block[MAX_FILES+1];
int curr_count = 0;

static int tinyfs_rmdir(struct inode *dir, struct dentry *dentry)
{
	
}

static int tinyfs_unlink(struct inode *dir, struct dentry *dentry)
{
	int i;
	struct inode *inode = dentry->d_inode;
	struct file_blk *blk = (struct file_blk *)inode->i_private;
	struct file_blk *pblk = (struct file_blk *)dir->i_private;
	struct dir_entry *entry;

	//update ../ dir
	entry = (struct dir_entry *)&pblk->data[0];
	for(i = 0; i < pblk->dir_children; i++) {
		if(!strcmp(entry[i].filename, dentry->d_name.name)) {
			int j;
			for(j = 0; j < pblk->dir_children-1; j++) {
				memcpy(&entry[j], &entry[j+1], sizeof(struct dir_entry));
			}
			pblk->dir_children--;
			break;
		}
	}

	blk->busy = 0;
	return simple_unlink(dir, entry);
}

static struct inode_operations tiny_inode_ops = {
	.create = tinyfs_create,
	.lookup = tinyfs_lookup,
	.mkdir = tinyfs_mkdir,
	.rmdir = tinyfs_rmdir,
	.unlink = tinyfs_unlink,
};

static int tinyfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *root_node;
	int mode = S_IFDIR;

	root_node = new_inode(sb);
	root_node->i_ino = 1;
	inode_init_owner(root_node, NULL, mode);
	root_node->i_sb = sb;
	root_node->i_op = &tiny_inode_ops;
	root_node->i_fop = &tiny_dir_operations;
	root_node->i_atime = root_node->i_mtime = root_node->i_ctime = CURRENT_TIME;

	block[1].mode = mode;
	block[1].dir_children = 0;
	block[1].idx = 1;
	block[1].busy = 1;
	root_node->i_private = &block[1];

	sb->s_root = d_make_root(root_node);
	curr_count++;

	return 0;
}

static struct dentry *tinyfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
	return mount_nodev(fs_type, flags, data, tinyfs_fill_super);
}

static void tinyfs_kill_superblock(struct super_block *sb)
{
	kill_anon_super(sb);
}

static struct file_system_type tinyfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "tinyfs",
	.mount = tinyfs_mount,
	.kill_sb = tinyfs_kill_superblock,
};

static int __init tinyfs_init(void)
{
	int ret;

	memset(block, 0, sizeof(block));
	ret = register_filesystem(&tinyfs_fs_type);
	if(ret) 
		pritk("register tinyfs failed\n");

	return ret;
}

static void __exit tinyfs_exit(void)
{
	unregister_filesystem(&tinyfs_fs_type);
}

module_init(tinyfs_init);
module_exit(tinyfs_exit);

MODULE_LICENSE("GPL");


