// SPDX-License-Identifier: GPL-3.0-only
/*
     Copyright (C) 2020  Haruki Shimotori and Ryuichi Ueda. All right reserved.
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

#define CAREER_TIME 13 // [us]

MODULE_AUTHOR("Haruki Shimotori and Ryuichi Ueda");
MODULE_DESCRIPTION("driver for ir remote controller control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
struct timer_list timer0;

void irled_write(char state)
{
	if(state == '0'){ 
		volatile int i = 0;	
		for(i = 0; i < 28; i++){ //high 0.8[ms]
			gpio_base[7] = 1 << 25; //on
			udelay(CAREER_TIME);
			gpio_base[10] = 1 << 25; //off
			udelay(CAREER_TIME);
		}
		for(i = 0; i < 14; i++){ //low 0.4 [ms]
			udelay(CAREER_TIME);
			udelay(CAREER_TIME);
		}

	}
	else if(state == '1'){ 
		volatile int i = 0;	
		for(i = 0; i < 28; i++){ //high 0.8[ms]
			gpio_base[7] = 1 << 25; //on
			udelay(CAREER_TIME);
			gpio_base[10] = 1 << 25; //off
			udelay(CAREER_TIME);
		}
		for(i = 0; i < 63; i++){ //low 1.6 [ms]
			udelay(CAREER_TIME);
			udelay(CAREER_TIME);
		}

	}
	else if(state == 's'){ 
		volatile int i = 0;	
		for(i = 0; i < 450; i++){ //high 12[ms]
			gpio_base[7] = 1 << 25; //on
			udelay(CAREER_TIME);
			gpio_base[10] = 1 << 25; //off
			udelay(CAREER_TIME);
		}
		for(i = 0; i < 157; i++){ //low 4 [ms]
			udelay(CAREER_TIME);
			udelay(CAREER_TIME);
		}

	}
	else if(state == 'e'){ 
		volatile int i = 0;	
		for(i = 0; i < 28; i++){ //high 0.8[ms]
			gpio_base[7] = 1 << 25; //on
			udelay(CAREER_TIME);
			gpio_base[10] = 1 << 25; //off
			udelay(CAREER_TIME);
		}
	}
}

void fan_on_off(void)
{
	irled_write('s');

	irled_write('0');
	irled_write('1');
	irled_write('1');
	irled_write('0');

	irled_write('0');
	irled_write('0');
	irled_write('0');
	irled_write('0');

	irled_write('1');
	irled_write('1');
	irled_write('0');
	irled_write('0');

	irled_write('0');
	irled_write('1');
	irled_write('0');
	irled_write('1');

	irled_write('0');
	irled_write('0');
	irled_write('0');
	irled_write('0');

	irled_write('1');
	irled_write('1');
	irled_write('1');
	irled_write('0');

	irled_write('0');
	irled_write('0');
	irled_write('0');
	irled_write('0');

	irled_write('0');
	irled_write('1');
	irled_write('1');
	irled_write('1');
	irled_write('1');

	irled_write('e');

}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

	if(c == '1'){
		fan_on_off();
	}
	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
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

	return 0;
}

static void __exit cleanup_mod(void) 
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. mojor:%d\n", __FILE__, MAJOR(dev));
	
}

module_init(init_mod);    
module_exit(cleanup_mod);
