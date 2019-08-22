/*
 *  communicate.c - communicate with user space
 *
 *  Copyright (C) sinovision 2013
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netlink.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/types.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/fcntl.h>
#include <net/net_namespace.h>
#include <linux/fs.h>
#include <linux/timekeeping.h>

#define MAX_LEN 	2048
#define COMM_MINOR	50
#define MAX_BUF_LEN 50

#define COMM_READ	_IO('k', 0x01)	
#define COMM_WRITE	_IO('k', 0x02)	

static DEFINE_MUTEX(comm_mutex);
static struct sock *comm_sk;
static DECLARE_WAIT_QUEUE_HEAD(comm_wait);

static int read_ready,write_ready;
static char buffer[MAX_BUF_LEN];
static char *reads = "strings from kernel";

//struct timespec g_send_t;
struct timespec64 g_send_t;

void send_to(u32 pid)
{
	struct sk_buff *skb;
	struct nlmsghdr  *nlh;
	//struct timespec send_t;
	struct timespec64 send_t;
	char *buf = "send form kernel to user space";
	int len;


	ssleep(10);
	
	//len = strlen(buf) + sizeof(struct timespec);
	len = MAX_LEN + sizeof(struct timespec64);
	skb = alloc_skb(NLMSG_SPACE(len), GFP_KERNEL);
	if (NULL == skb){
		printk("alloc_skb error\n");
		return;
		
	}

	//nlh = NLMSG_PUT(skb, 0, 0, 0, len);
	nlh = __nlmsg_put(skb, 0, 0, 0, len, 0);
	if(!nlh)
		goto nlmsg_failure;
	

	NETLINK_CB(skb).portid = 0;

	//ktime_get_real_ts(&send_t);
	ktime_get_real_ts64(&send_t);
	
	//send_t = current_kernel_time();
	*((struct timespec64 *)NLMSG_DATA(nlh)) = send_t;
	//sprintf(NLMSG_DATA(nlh) + sizeof(send_t), "%s", buf);
	memset(NLMSG_DATA(nlh) + sizeof(send_t), 0x41, MAX_LEN);
	
	netlink_unicast(comm_sk, skb, pid, MSG_DONTWAIT);
	
	
	return;

nlmsg_failure:
	printk("NLMSG_PUT error\n");
	kfree_skb(skb);
	return ;	
	
}

void msg_input(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	char *msg;
	int len;
	u32 s_pid;

	if (unlikely(NULL == skb)) {
		return;
	}

	s_pid = NETLINK_CB(skb).portid;
	printk("%s rec: portid = %d, dst_group= %d\n", __func__, 
		NETLINK_CB(skb).portid, NETLINK_CB(skb).dst_group);
	
	nlh = nlmsg_hdr(skb);
	
	printk("%s rec: nlmsg_pid = %d, nlmsg_len = %d skb_len = %d\n", __func__, 
		nlh->nlmsg_pid, nlh->nlmsg_len, skb->len);
	
	if (nlh->nlmsg_len < NLMSG_HDRLEN ||
		skb->len < nlh->nlmsg_len) {
		return;
	}

	len = skb->len;
	while (NLMSG_OK(nlh, len)) {
		msg = NLMSG_DATA(nlh);
		printk("%s rec: %s\n", __func__, msg);
		nlh = NLMSG_NEXT(nlh, len);
	}

	send_to(s_pid);
}

static ssize_t comm_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos) 
{

	size_t len;
	

	
	if (count <= 0) {
		return -EINVAL;
	}

	if (file->f_flags & O_NONBLOCK && !read_ready) {
		return -EAGAIN;
	}

	read_ready = 0;
	
	len = strlen(reads) + sizeof(struct timespec64);
	memset(buffer, 0x0, len);
	//ktime_get_real_ts(&send_t);
	*((struct timespec64 *)buffer) = g_send_t;
	sprintf(buffer + sizeof(struct timespec64), "%s", reads);
	
	if (copy_to_user(buf, buffer, len)) {
		printk("%s copy_to_user error\n", __func__);
		return -EINVAL;
	} else {
		*ppos += len;
		return len;
	}

	return 0;

}

static int comm_open(struct inode *inode, struct file *file)
{
	write_ready = 1;
	read_ready = 1;
	return 0;
}

static ssize_t comm_write(struct file *file, const char __user *buf,
			  size_t count, loff_t * ppos)
{
	if (count > MAX_BUF_LEN) {
		return -EINVAL;
	}

	if (file->f_flags & O_NONBLOCK && !write_ready) {
		return -EAGAIN;
	}

	write_ready = 0;
	memset(buffer, 0x0, MAX_BUF_LEN);
	if (copy_from_user(buffer, buf, count)) {
		printk("%s copy_from_user error\n", __func__);
		return -EINVAL;
	} else {
		printk("%s: %s\n", __func__, buffer);
		*ppos += count;
		return count;
	}
	
	return 0;
}

static unsigned int comm_poll(struct file *file,
				 struct poll_table_struct *wait)
{
	int mask = 0;

	poll_wait(file, &comm_wait, wait);
	if (read_ready) {
		mask = POLLIN | POLLRDNORM;
	}
	if (write_ready) {
		mask |= POLLOUT | POLLWRNORM;
	}
	
	//ktime_get_real_ts(&g_send_t);
	ktime_get_real_ts64(&g_send_t);
	
	return mask;
}

static long comm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case COMM_READ:
			read_ready = 1;
			break;
		case COMM_WRITE:
			write_ready = 1;
			break;
		default:
			printk("%s: args is error\n", __func__);
			return -EAGAIN;
	}
	return 0;
}

static const struct file_operations comm_fops = {
	.open = comm_open,
	.read = comm_read,
	.write = comm_write,
	.poll = comm_poll,
	.unlocked_ioctl = comm_ioctl,
};

static struct miscdevice comm_dev = 
{
	.minor = COMM_MINOR,
	.name = "communicate",
	.fops = &comm_fops,
};

static int __init communicate_init(void)
{
	int retval;
	struct netlink_kernel_cfg cfg = {
		.groups = 0,
		.input = msg_input,
		.cb_mutex = NULL,
	};
	//comm_sk = netlink_kernel_create(&init_net, NETLINK_UNUSED, 0, msg_input, NULL, THIS_MODULE);
	comm_sk = netlink_kernel_create(&init_net, NETLINK_UNUSED, &cfg);
	if (unlikely(NULL == comm_sk)) {
		return -ENOMEM;
	}

	retval = misc_register(&comm_dev);
	if (retval < 0){
		printk("register 'communicate' misc cdev error");
		return retval;
	}
	return 0;
}

static void __exit communicate_exit(void)
{	
	misc_deregister(&comm_dev);
	if (likely(NULL != comm_sk)) {
		netlink_kernel_release(comm_sk);
	}
}

module_init(communicate_init);
module_exit(communicate_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangxiwei");
MODULE_DESCRIPTION("communicate unit");

