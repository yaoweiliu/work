#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/timekeeping.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
//#include <asm-generic/uaccess.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <uapi/asm-generic/poll.h>

/* spi driver interface */

/* struct spi_driver
 * struct spi_device
 * struct spi_message
 * struct spi_transfer
 * spi_message_init()
 * spi_message_add_tail()
 * spi_async()
 * spi_sync()
 * spi_register_driver()
 * spi_unregister_driver()
 */

typedef enum {
	SVC_ID_DEV_FPGA_SPI = 1001,
}SVCID;

typedef struct {
	uint32_t code;		// event code
	uint32_t length;	// total  size of header and payload
	SVCID sid;			// sender id
	SVCID rid;			// receiver
	uint32_t checksum;	// code + length + sid + rid 
	uint32_t not_log;	// not log event
	int64_t sent;		// sent time, in nanoseconds since Jan. 1, 1970.
	int64_t received;	// received time, in nanoseconds since Jan. 1, 1970.
	int64_t complete;	// complete time, in nanoseconds since Jan. 1, 1970.
}EVENT_HEADER_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char devName[32];
	char buildTime[64];
	char svnVer[32];
	char builder[32];
}EV_SS_DEV_REPORT_VER_TYPE;

enum {
	EV_SS_DEV_REPORT_VER = 100,
};

#define FPGA_SPI_CMD_RD			(0x1)
#define FPGA_SPI_CMD_WR			(0x2)
#define FPGA_SPI_CMD_LEN_RD		(0x4)
#define FPGA_SPI_CMD_LEN_WR		(0x4)
#define FPGA_SPI_DAT_DUMMY		(0x5a5a)

#define CHECKSUM(x)				(((x >> 8) & 0xff) + (x & 0xff))
#define CHECKSUM_MASK			(0x7fff)
#define CHECKSUM_VALID(x)		((x & ~CHECKSUM_MASK) == 0)

#define BITS_PER_WORD			16
#define SPI_SPEED_HZ			1000000  /* 1Mhz */

typedef struct {
	unsigned int reg;
	unsigned int val;
}FPGA_COMM_REG_TYPE;

#ifndef FPGA_COMM_IOCTL_TYPE
#define	FPGA_COMM_IOCTL_TYPE 'f'
#endif

#ifndef	CMD_SET_REGISTER
#define	CMD_SET_REGISTER	_IOWR(FPGA_COMM_IOCTL_TYPE, 0, FPGA_COMM_REG_TYPE)
#endif

#ifndef	CMD_GET_REGISTER
#define	CMD_GET_REGISTER	_IOWR(FPGA_COMM_IOCTL_TYPE, 1, int)
#endif

struct fpga_spi_dev_t {
	unsigned int rlen, total_len;
	unsigned char *rdata;
};

struct event_queue_t {
	struct list_head queue;
	EVENT_HEADER_TYPE *evt;
};

static struct spi_device *spi_dev = NULL;

static wait_queue_head_t event_wait;
static struct list_head event_queue;
static DEFINE_MUTEX(evt_queue_mutex);

static void fpga_spi_transfer_complete(void *compl)
{
	complete((struct completion *)compl);
}

static int fpga_spi_transfer(void *tx_buf, uint32_t tx_len, void *rx_buf, uint32_t rx_len)
{
	int ret = 0;
	struct spi_transfer transfer;
	struct spi_message message;
	DECLARE_COMPLETION_ONSTACK(done);

	if(unlikely(spi_dev == NULL)) {
		printk(KERN_ERR "%s: spi_dev is null\n", __func__);
		return -EINVAL;
	}

	transfer.rx_buf = rx_buf;
	transfer.tx_buf = tx_buf;
	transfer.len = tx_len + rx_len;
	transfer.cs_change = 1;
	transfer.bits_per_word = BITS_PER_WORD;
	transfer.speed_hz = SPI_SPEED_HZ;

	spi_message_init(&message);
	spi_message_add_tail(&transfer, &message);
	message.complete = fpga_spi_transfer_complete;
	message.context = &done;

	ret = spi_async(spi_dev, &message);
	if(ret) {
		printk(KERN_ERR "%s: spi transfer error!\n", __func__);
		return ret;
	}

	wait_for_completion(&done);

	return 0;
}

int fpga_spi_read_reg(unsigned int offset, unsigned short *val)
{
	int ret = 0;
	unsigned short checksum = 0, checksum_rd = 0, data = 0;
	unsigned short tx_buf[FPGA_SPI_CMD_LEN_WR];
	unsigned short rx_buf[FPGA_SPI_CMD_LEN_RD];

	checksum = CHECKSUM(FPGA_SPI_CMD_RD) + CHECKSUM(offset);
	checksum &= CHECKSUM_MASK;

	memset(tx_buf, 0x0, sizeof(tx_buf));
	memset(rx_buf, 0x0, sizeof(rx_buf));
	tx_buf[0] = FPGA_SPI_CMD_RD;
	tx_buf[1] = offset;
	tx_buf[2] = checksum;
	tx_buf[3] = FPGA_SPI_DAT_DUMMY;

	ret = fpga_spi_transfer((void *)tx_buf, sizeof(tx_buf) / sizeof(tx_buf[0]), (void *)rx_buf, sizeof(rx_buf) / sizeof(rx_buf[0]));
	if(ret)
		return ret;

	data = rx_buf[2];
	checksum_rd = rx_buf[3];
	checksum = CHECKSUM(data);

	if(!CHECKSUM_VALID(checksum_rd) || (checksum & CHECKSUM_MASK) != (checksum_rd & CHECKSUM_MASK)) {
		printk(KERN_ERR "%s: checksum error!\n", __func__);
		return -EIO;
	}

	*val = data;

	return 0;
}
EXPORT_SYMBOL(fpga_spi_read_reg);

int fpga_spi_write_reg(unsigned int offset, unsigned short val)
{
	unsigned short checksum = 0;
	unsigned short tx_buf[FPGA_SPI_CMD_LEN_WR];
	unsigned short rx_buf[FPGA_SPI_CMD_LEN_RD];

	checksum = CHECKSUM(FPGA_SPI_CMD_WR) + CHECKSUM(offset) + CHECKSUM(val);
	checksum &= CHECKSUM_MASK;

	memset(tx_buf, 0x0, sizeof(tx_buf));
	memset(rx_buf, 0x0, sizeof(rx_buf));
	tx_buf[0] = FPGA_SPI_CMD_WR;
	tx_buf[1] = offset;
	tx_buf[2] = val;
	tx_buf[3] = checksum;

	return fpga_spi_transfer((void *)tx_buf, sizeof(tx_buf) / sizeof(tx_buf[0]), (void *)rx_buf, sizeof(rx_buf) / sizeof(rx_buf[0]));
}
EXPORT_SYMBOL(fpga_spi_write_reg);

int fpga_spi_write_reg_chk(unsigned int offset, unsigned short val)
{
	unsigned short chkval;
	int ret = 0;

	ret = fpga_spi_write_reg(offset, val);
	if(unlikely(ret < 0))
		return ret;

	ret = fpga_spi_read_reg(offset, &chkval);
	if(unlikely(ret < 0))
		return ret;

	if(unlikely(val != chkval)) {
		printk(KERN_WARNING "%s: write fpga register error!\n", __func__);
		return -EIO;
	}

	return 0;
}
EXPORT_SYMBOL(fpga_spi_write_reg_chk);

static int send_event(uint32_t code, uint32_t len, void *ev_pub)
{
	EVENT_HEADER_TYPE *ev_com;
	struct timespec send_t;
	struct event_queue_t *evt_q = NULL;

	evt_q = kmalloc(sizeof(struct event_queue_t), GFP_KERNEL);
	if(!evt_q) {
		printk(KERN_ERR "%s: alloc memory error!\n", __func__);
		return -ENOMEM;
	}

	ev_com = (EVENT_HEADER_TYPE *)ev_pub;
	ev_com->code = code;
	ev_com->length = len;
	ev_com->sid = SVC_ID_DEV_FPGA_SPI;
	ev_com->rid = 0;
	ev_com->checksum = ev_com->length + ev_com->sid + ev_com->rid + ev_com->code;
	ktime_get_real_ts(&send_t);
	ev_com->sent = timespec_to_ns(&send_t);

	evt_q->evt = ev_com;
	mutex_lock(&evt_queue_mutex);
	list_add_tail(&(evt_q->queue), &event_queue);
	mutex_unlock(&evt_queue_mutex);

	wake_up_interruptible(&event_wait);

	return 0;
}

static int report_ver(void)
{
	EV_SS_DEV_REPORT_VER_TYPE *ver_ev = NULL;
	int len = sizeof(EV_SS_DEV_REPORT_VER_TYPE);

	ver_ev = (EV_SS_DEV_REPORT_VER_TYPE *)kmalloc(len, GFP_KERNEL);
	if(!ver_ev) {
		printk(KERN_ERR "%s: alloc memory error!\n", __func__);
		return -ENOMEM;
	}

	memset(ver_ev, 0x0, len);
	strcpy(ver_ev->devName, "DeviceFpgaSpi");
	strcpy(ver_ev->builder, "Yaowei Liu");
	strcpy(ver_ev->buildTime, "2018-11-12");
	strcpy(ver_ev->svnVer, "svn9527");

	return send_event(EV_SS_DEV_REPORT_VER, len, ver_ev);
}

static int have_data(struct fpga_spi_dev_t *fpga_spi_dev)
{
	if((fpga_spi_dev->rdata != NULL) && (fpga_spi_dev->rlen < fpga_spi_dev->total_len))
		return -1;
	return !list_empty(&event_queue);
}

static int get_rdata(struct fpga_spi_dev_t *fpga_spi_dev)
{
	struct event_queue_t *evt_q = NULL;

	if((fpga_spi_dev->rdata != NULL) && (fpga_spi_dev->rlen < fpga_spi_dev->total_len))
		return 0;

	mutex_lock(&evt_queue_mutex);
	if(!list_empty(&event_queue)) {
		evt_q = list_first_entry(&event_queue, struct event_queue_t, queue);
		list_del_init(&(evt_q->queue));
	}
	mutex_unlock(&evt_queue_mutex);

	if(evt_q == NULL || evt_q->evt == NULL) {
		printk(KERN_ERR "%s: invalid event got!\n", __func__);
		kfree(evt_q);
		return -EINVAL;
	}

	fpga_spi_dev->rdata = (unsigned char *)evt_q->evt;
	fpga_spi_dev->rlen = 0;
	fpga_spi_dev->total_len = evt_q->evt->length;

	kfree(evt_q);

	return 0;
}

int fpga_spi_open(struct inode *inode, struct file *file)
{
	struct fpga_spi_dev_t *fpga_spi_dev = NULL;

	fpga_spi_dev = kmalloc(sizeof(struct fpga_spi_dev_t), GFP_KERNEL);
	if(!fpga_spi_dev) {
		printk(KERN_ERR "%s: no memory!\n", __func__);
		return -ENOMEM;
	}

	fpga_spi_dev->rdata = NULL;
	fpga_spi_dev->rlen = 0;
	fpga_spi_dev->total_len = 0;

	file->private_data = fpga_spi_dev;

	report_ver();

	return 0;
}

int fpga_spi_close(struct inode *inode, struct file *file)
{
	struct fpga_spi_dev_t *fpga_spi_dev = NULL;

	fpga_spi_dev = (struct fpga_spi_dev_t *)file->private_data;
	if(!fpga_spi_dev) {
		printk(KERN_WARNING "%s: fpga_spi_dev is null on close!\n", __func__);
		return -EINVAL;
	}

	kfree(fpga_spi_dev);

	return 0;
}

ssize_t fpga_spi_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	struct fpga_spi_dev_t *fpga_spi_dev = NULL;
	unsigned int len = 0;
	//int ret = 0; 

	if(count <= 0)
		return -EINVAL;

	fpga_spi_dev = (struct fpga_spi_dev_t *)file->private_data;
	if(!fpga_spi_dev) {
		printk(KERN_ERR "%s: get fpga_spi_dev error!\n", __func__);
		return -EINVAL;
	}

	if((file->f_flags & O_NONBLOCK) && !have_data(fpga_spi_dev))
		return -EAGAIN;

	if(get_rdata(fpga_spi_dev))
		return -EINVAL;

	//len = min(count, (fpga_spi_dev->total_len - fpga_spi_dev->rlen));
	len = count > (fpga_spi_dev->total_len - fpga_spi_dev->rlen) ? (fpga_spi_dev->total_len - fpga_spi_dev->rlen) : count;
	if(copy_to_user(buf, fpga_spi_dev->rdata + fpga_spi_dev->rlen, len)) {
		printk(KERN_ERR "%s: copy_to_user error!\n", __func__);
		return -1;
	}
	*offset += len;

	fpga_spi_dev->rlen += len;
	if(fpga_spi_dev->rlen >= fpga_spi_dev->total_len) {
		kfree(fpga_spi_dev->rdata);
		fpga_spi_dev->rdata = NULL;
		fpga_spi_dev->rlen = 0;
		fpga_spi_dev->total_len = 0;
	}

	return len;
}

ssize_t fpga_spi_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	EVENT_HEADER_TYPE *evt_h = NULL;
	uint32_t checksum = 0;
	//int ret = 0;

	if(count < sizeof(EVENT_HEADER_TYPE)) {
		printk(KERN_WARNING "%s: write size is less than event haader\n", __func__);
		return -EINVAL;
	}

	evt_h = (EVENT_HEADER_TYPE *)kmalloc(count, GFP_KERNEL);
	if(!evt_h) {
		printk(KERN_ERR "%s: alloc memory error!\n", __func__);
		return -ENOMEM;
	}

	memset(evt_h, 0x0, count);
	if(copy_from_user(evt_h, buf, count)) {
		printk(KERN_WARNING "%s: copy_from_user error!\n", __func__);
		kfree(evt_h);
		return -1;
	}
	*offset += count;

	checksum = evt_h->length + evt_h->code + evt_h->sid + evt_h->rid;
	if(checksum != evt_h->checksum) {
		printk(KERN_WARNING "%s: checksun error!\n", __func__);
		return -EINVAL;
	}

	switch(evt_h->code) {
	default:
		printk("%s: no switch function!\n", __func__);
		break;
	}

	return count;
}

unsigned int fpga_spi_poll(struct file *file, struct poll_table_struct *wait)
{
	struct fpga_spi_dev_t *fpga_spi_dev = NULL;
	unsigned int mask = 0;

	fpga_spi_dev = (struct fpga_spi_dev_t *)file->private_data;
	if(!fpga_spi_dev) {
		printk(KERN_ERR "%s: get fpga_spi_dev error!\n", __func__);
		return -EINVAL;
	}

	poll_wait(file, &event_wait, wait);
	if(have_data(fpga_spi_dev)) {
		mask = POLLIN | POLLRDNORM;
	}

	return mask;
}

long fpga_spi_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	uint16_t val = 0;
	FPGA_COMM_REG_TYPE reg;

	switch(cmd)
	{
	case CMD_SET_REGISTER:
		ret = copy_from_user(&reg, (void *)arg, sizeof(FPGA_COMM_REG_TYPE));
		if(ret)
			break;
		val = reg.val;
		ret = fpga_spi_write_reg(reg.reg, val);
		break;
	case CMD_GET_REGISTER:
		ret = copy_from_user(&reg, (void *)arg, sizeof(uint32_t));
		if(ret)
			break;
		ret = fpga_spi_read_reg(reg.reg, &val);
		if(!ret) {
			reg.val = val;
			ret = copy_to_user((void *)arg, &reg.val, sizeof(uint32_t));
		}
		break;
	default:
		printk("%s: unsupport command!\n", __func__);
		break;
	}

	return ret;
}

static const struct file_operations fpga_spi_ops = 
{
	.open = fpga_spi_open,
	.release = fpga_spi_close,
	.read = fpga_spi_read,
	.write = fpga_spi_write,
	.poll = fpga_spi_poll,
	.unlocked_ioctl= fpga_spi_ioctl,
};

static struct miscdevice fpga_spi_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fpga_spi_miscdevice",
	.fops = &fpga_spi_ops,
};

static int fpga_spi_probe(struct spi_device *spi)
{
	int ret = 0;

	ret = misc_register(&fpga_spi_miscdev);
	if(ret) {
		printk(KERN_ERR "%s: misc_register() error!\n", __func__);
		return ret;
	}

	spi_dev = spi;
	INIT_LIST_HEAD(&event_queue);
	init_waitqueue_head(&event_wait);

	return 0;
}

static int fpga_spi_remove(struct spi_device *spi)
{
	spi_dev = NULL;
	misc_deregister(&fpga_spi_miscdev);

	return 0;
}

static struct spi_driver fpga_spi_driver = {
	.driver = {
		.name = "fpga_spi_driver",
		.owner = THIS_MODULE,
	},
	.id_table = NULL,
	.probe = fpga_spi_probe,
	.remove = fpga_spi_remove,
};

static int __init fpga_spi_init(void)
{
	int ret = 0;

	ret = spi_register_driver(&fpga_spi_driver);
	if(ret) {
		printk(KERN_ERR "%s: register fpga error!\n", __func__);
		return ret;
	}
	return 0;
}

static void __exit fpga_spi_exit(void)
{
	spi_unregister_driver(&fpga_spi_driver);
}

module_init(fpga_spi_init);
module_exit(fpga_spi_exit);

MODULE_LICENSE("GPL");

