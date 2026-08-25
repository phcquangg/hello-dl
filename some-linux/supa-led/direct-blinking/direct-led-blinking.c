#include <linux/fs.h>		// file_operations & _chrdev_region
#include <linux/kdev_t.h>	// MAJOR() and MINOR() macros
#include <linux/cdev.h>		// to register the file_operations to the driver
#include <linux/device.h>	// class & device
#include <linux/err.h>		// IS_ERR & PTR_ERR

#define DEV_NAME	"supa_led"
#define CLASS_NAME	"supa_led"

static dev_t			dev_num;
static struct class		*dev_class;

struct dev_data {
	struct cdev		driver_cdev,
	struct device	dev
};

static int dev_open( struct inode *inode, struct file *filp)
{
	return 0;
}

static int dev_release( struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t dev_write( struct file *filp, char __user *user, size_t buf, loff_t *ppos)
{
	return 0;
}

static ssize_t dev_read( struct filp *filp, char __user *user, size_t buf, loff_t *ppos)
{
	return 0;
}

static void device_release( struct device *dev)
{
	struct 

static struct file_operation fops = {
	.ower = THIS_MODULE,
	.open = dev_open,
	.write = dev_write,
	.read = dev_read,
	.release = dev_release
};

static __init int dlb_init( void)
{
	struct dev_data *dd;
	int ret;

	dd = kzalloc(sizeof(*dd), GFP_KERNEL);
	if (!dd) return -ENOMEM;

	// alloc_chrdev_region( dev_t *, unsigned, unsigned, const char *);
	ret = alloc_chrdev_region( &dev_num, 0, 1, DEV_NAME);
	if (ret < 0) {
		kfree(dd);
		pr_err("Failed to alloc_chrdev_region\n");
		return -1;
	}

	// void cdev_init( struct cdev *, const struct file_operations *);
	cdev_init(&dd->driver_cdev, &fops);
	dd->driver_cdev.owner = THIS_MODULE;

	// void device_initialize( struct device *dev);
	device_initialize( &dd->dev);

	dd->dev.class = dev_class;
	dd->dev.devt = dev_num;
	dd->dev.release = NULL;
	// name in /dev/ and /sys/
	dev_set_name(&dd->dev, DEV_NAME);
	// int cdev_device_add( struct cdev *cdev, struct device *dev);

	ret = cdev_device_add(&dd->cdev, &dd->dev);

	if (ret) {
		put_device(&dd->dev);
		unregister_chrdev_region(dev_num, 1);
		return ret;
	}

	return 0;
}

static __exit void dlb_exit( void)
{
	// void unregsiter_chrdev_region( dev_t, unsigned);
	unregister_chrdev_region(dev_num, 1);
	// void cdev_device_del( struct cdev *cdev, struct device *dev);
	cdev_device_del(&dd->driver_cdev, &dd->dev);
}

MODULE_AUTHOR("supa quangg");
MODULE_DESCRIPTION("Blinking LED by modifying device file");
MODULE_VERSION("1.0");

module_init(dlb_init);
module_exit(dlb_exit);
