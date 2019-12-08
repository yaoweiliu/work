#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "tinyfs.h"

struct file_blk block[MAX_FILES+1];
int curr_count = 0;

static int tinyfs_fill_super(struct super_block *sb, void *data, int silent)
{
	
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


