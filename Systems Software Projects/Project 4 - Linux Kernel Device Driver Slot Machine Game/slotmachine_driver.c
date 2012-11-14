/*NAME: Ben Isenberg*/
/*DATE: 7/28/11*/

/*This program is a char device driver for the Linux kernel.  It has a read function that generates
a random number between 0-32 and then returns a 0-6 based on the distribution of slot machine symbols
for each reel in the 0-32 number range*/

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include<linux/module.h>
#include<linux/init.h>

#include<linux/random.h>	/*Need for random numbers*/
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

/*Function prototypes*/
static unsigned char my_read(struct file *filep, char *data, int size);
static int my_open(struct inode *inode, struct file *filep);

/*Random number*/
static unsigned char my_data;
/*Counter*/
static unsigned int counter;

/*File operations struct for device driver*/
const struct file_operations my_fops={
	.owner	= THIS_MODULE,	
	open: my_open,
	read: my_read,
};


static struct miscdevice slot_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/slotmachine.
	 */
	"slotmachine",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&my_fops
};


int my_open(struct inode *inode, struct file *filep)
{
	return 0;
}

/*Function returns a random number <= max*/
static unsigned char get_random_byte(int max) {
   unsigned char c;
   get_random_bytes(&c, 1);
   return c % max;
}

/*Read function that determines what symbol the user spun on the slot machine*/
static unsigned char my_read(struct file *filep, char *data, int size)
{
	/*Get random number <= 32*/
	my_data = get_random_byte(32);

	int symbol = 0;

	counter++;

	/*Determine what symbol the user spun on the first reel*/
	/*Each reel has different symbol distributions*/
	if(counter == 1){
		if(my_data <= 16){
			symbol = 0;
		}
		if(my_data > 16 && my_data < 20){
			symbol = 1;
		}
		if(my_data > 19 && my_data < 24){
			symbol = 2;
		}
		if(my_data > 23 && my_data < 28){
			symbol = 3;
		}
		if(my_data > 27 && my_data < 30){
			symbol = 4;
		}
		if(my_data > 29 && my_data < 32){
			symbol = 5;
		}
		if(my_data == 32){
			symbol = 6;
		}
	}

	/*Determine what symbol the user spun on the second reel*/
	/*Each reel has different symbol distributions*/
	if(counter == 2){
		if(my_data <= 16){
			symbol = 0;
		}
		if(my_data > 16 && my_data < 22){
			symbol = 1;
		}
		if(my_data > 21 && my_data < 25){
			symbol = 2;
		}
		if(my_data > 24 && my_data < 27){
			symbol = 3;
		}
		if(my_data > 26 && my_data < 30){
			symbol = 4;
		}
		if(my_data == 30){
			symbol = 5;
		}
		if(my_data > 30 && my_data < 33){
			symbol = 6;
		}
	}

	/*Determine what symbol the user spun on the third reel*/
	/*Each reel has different symbol distributions*/
	if(counter == 3){
		/*After third reel has spun reset counter*/
		counter = 0;

		if(my_data <= 16){
			symbol = 0;
		}
		if(my_data > 16 && my_data < 22){
			symbol = 1;
		}
		if(my_data > 21 && my_data < 26){
			symbol = 2;
		}
		if(my_data > 25 && my_data < 29){
			symbol = 3;
		}
		if(my_data == 29){
			symbol = 4;
		}
		if(my_data > 29 && my_data < 32){
			symbol = 5;
		}
		if(my_data == 32){
			symbol = 6;
		}
	}

	/*Place symbol into passed in char buffer*/
	*data = symbol;
	
	return 0;

}

/*Device init function, executes when module is loaded into kernel*/
static int slot_init(void){
	
	int ret = misc_register(&slot_dev);

	printk("Slot machine inserted\n");
	
	return ret;
}

/*Device exit function, executes when module is removed from kernel*/
static void slot_exit(void){
	
	misc_deregister(&slot_dev);

	printk("Slot machine removed\n");
}

module_init(slot_init);

module_exit(slot_exit);
