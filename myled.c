#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/time.h>

MODULE_AUTHOR("Haruki Shimotori and Ryuchi Ueda");
MODULE_DESCRIPTION("driver for irLED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
struct timer_list timer0;


int print_timestamp(void){
	/*
	struct timespec time;
	long timestamp;

	getnstimeofday(&time);
	timestamp = time.tv_sec + time.tv_nsec;
	*/
	
	//printk(KERN_INFO "time = %ld nsec. \n", timestamp);
	ock_gettime(before);
	recv()
		clock_gettime(after);
	global_time += after - before.
	printk(KERN_INFO "time = %ld nsec. \n", timestamp);
	return 0;
}
static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

	//printk(KERN_INFO "receive %c\n", c);
	
	if(c == '0'){ 
		gpio_base[10] = 1 << 25; //off

	}
	else if(c == '1'){ 
		gpio_base[7] = 1 << 25; //on
		mdelay(1000);
		gpio_base[10] = 1 << 25; //off
	}
	return 1;
}


static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'s','u', 's', 'h', 'i'};
	if(copy_to_user(buf + size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_ERR "sushi : popy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(sushi);

	return size;
}

void timer0_callback(struct timer_list *timer){

	static int flag = 0;

	if(flag == 0){
		gpio_base[7] = 1 << 25; //on
		flag = 1;
	}
	else{
		gpio_base[10] = 1 << 25; //off
		flag = 0;
	}
	print_timestamp();

	mod_timer (&timer0, jiffies + ( msecs_to_jiffies(500)));

}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};

static int __init init_mod(void) 
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chdev_region failed. \n");
		return retval;
	}	

	printk(KERN_INFO "%s is loade, major:%d \n", __FILE__, MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add falid. major:%d, minor:%d\n", MAJOR(dev), MINOR(dev));
		return retval;
	}	

	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));

	gpio_base = ioremap_nocache(0xfe200000, 0xA0);

	const u32 led = 25;
	const u32 index = led / 10;
	const u32 shift = (led % 10) * 3;
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);

	//-----Timer setup------//
	
	timer_setup(&timer0, timer0_callback, 0);
	mod_timer(&timer0, jiffies + msecs_to_jiffies(500));

	return 0;
}

static void __exit cleanup_mod(void) 
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. mojor:%d\n", __FILE__, MAJOR(dev));
	
	//-----Timer delete----//
	del_timer(&timer0);
}

module_init(init_mod);    
module_exit(cleanup_mod);
