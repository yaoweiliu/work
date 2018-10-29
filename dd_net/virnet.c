#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/errno.h>

static struct net_device *sg_dev;

static int virnet_send_packet(struct sk_buff *skb, struct net_device *dev)
{
	int i;

	netif_stop_queue(dev);

	printk("send: lenth=%d\n", skb->len);
	for(i=0; i<skb->len; i++)
		printk("%x ", skb->data[i]);
	printk("\n");

	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	dev_kfree_skb(skb);

	netif_wake_queue(dev);

	return 0;
}

static const struct net_device_ops sg_ops = 
{
	.ndo_start_xmit = virnet_send_packet,
};

static int __init virnet_init(void)
{
	sg_dev = alloc_netdev(0, "virnet%d", ether_setup);
	sg_dev->netdev_ops = &sg_ops;
	memcpy(sg_dev->dev_addr, "\x01\x02\x03\x04\x05\x06", 6);

	register_netdev(sg_dev);
	return 0;
}

static void __exit virnet_exit(void)
{
	unregister_netdev(sg_dev);
	free_netdev(sg_dev);
}

module_init(virnet_init);
module_exit(virnet_exit);

MODULE_LICENSE("GPL");