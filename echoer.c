#include <linux/init.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/string.h>

#define start_minor_number 1
#define number_of_device 1
#define DEVICE_NAME "echoer"

static dev_t device_number;
static int major = -1;
static int minor = -1;
static struct cdev dev_echoer;

static char msg[] = "";

static int echoer_open(struct inode *inode, struct file *fp){
	printk("Device " DEVICE_NAME " open.\n");
	return 0;
}

static ssize_t echoer_write(struct file *fp, const char *buf, size_t count, loff_t *position){
	int num;
	int ret;

	if (count < strlen(msg)){
		num = count;
	}
	else{
		num = strlen(msg);
	}

	ret = copy_from_user(&msg, buf, num);
	if (ret){
		printk("Fail to copy from user space.\n");
	}
	return num;

}

static ssize_t echoer_read(struct file *fp, char *buf, size_t count, loff_t *position){
	int num;
	int ret;

	if (count < strlen(msg)){
		num = count;
	}
	else{
		num = strlen(msg);
	}

	ret = copy_to_user(buf, msg, num);
	if (ret){
		printk("Fail to copy from kernel space.\n");
	}
	return num;
}

static int echoer_release(struct inode *inode, struct file *fp){
	printk("Device " DEVICE_NAME " release.\n");
	return 0;
}

static struct file_operations echoer_fops = {
	owner: THIS_MODULE,
	open: echoer_open,
	write: echoer_write,
	read: echoer_read,
	release: echoer_release,
};

static int __init echoer_init(void){
	int ret;

	ret = alloc_chrdev_region(&device_number, start_minor_number, number_of_device, DEVICE_NAME);
	if (ret < 0){
		printk("Device " DEVICE_NAME " cannot get major number.\n");
		return ret;
	}
	major = MAJOR(device_number);
	minor = MINOR(device_number);
	printk("Device " DEVICE_NAME " initialized with major -- %d, minor -- %d.\n", major, minor);

	cdev_init(&dev_echoer, &echoer_fops);
	dev_echoer.owner = THIS_MODULE;
	dev_echoer.ops = &echoer_fops;
	ret = cdev_add(&dev_echoer, device_number, number_of_device);
	if (ret){
		printk("Device " DEVICE_NAME " register fail.\n");
		return ret;
	}
	return 0;
}

static void __exit echoer_exit(void){
	cdev_del(&dev_echoer);
	unregister_chrdev_region(device_number, number_of_device);
	printk("Device " DEVICE_NAME " unloaded.\n");
}

module_init(echoer_init);
module_exit(echoer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JYWu");
MODULE_DESCRIPTION("COMP3438 Final Project");
