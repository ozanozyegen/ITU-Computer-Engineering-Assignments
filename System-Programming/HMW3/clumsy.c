#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>         // Header to support the kernel Driver Model

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */

#include <asm/uaccess.h>	/* copy_*_user */
#include <linux/cdev.h>
#include <linux/random.h>	/* get_random_byte */

#include "clumsy_ioctl.h"

#define BUFFER_SIZE 4096
#define DEVICE_NAME "clumsy"
#define CLASS_NAME "clumsy"

static struct class* clumsyClass = NULL;
static struct device* clumsyDevice = NULL;

//credentials
static int major_number;
static int minor_number = 0;
static dev_t devno;
struct cdev cdev;

static int     clumsy_open(struct inode *, struct file *);
static ssize_t clumsy_read(struct file *, char *, size_t, loff_t *);
static ssize_t clumsy_write(struct file *, const char *, size_t, loff_t *);
static int     clumsy_release(struct inode *, struct file *);
long clumsy_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
int get_random(int length);
static struct file_operations fops =
{
	.open = clumsy_open,
   .read = clumsy_read,
   .write = clumsy_write,
   .release = clumsy_release,
    .unlocked_ioctl =  clumsy_ioctl,
};


//params
static int buffer_size = BUFFER_SIZE;
static int mode = 1;
static int n = 0;
//buffer
static char * buffer;
static int length;

module_param(buffer_size, int, S_IRUGO);
module_param(mode, int, S_IRUGO);
module_param(n, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPL");

static int clumsy_init_module(void){
	
	if((mode != 1 && mode != 2) || buffer_size < 0 || n < 0){
			printk(KERN_ERR "Invalid module arguments\n");
			return -EINVAL;
	}
	printk(KERN_INFO "Initializing device module\n");
	
	//Register major number
	major_number=register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number<0){
      printk(KERN_ALERT "Clumsy failed to register a major number\n");
      return major_number;
	}
	printk(KERN_INFO "Module registered with major number %d\n", major_number);
	
	//create class
	clumsyClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(clumsyClass)){
			unregister_chrdev(major_number, DEVICE_NAME);
			printk(KERN_ERR "Failed to create class\n");
			return PTR_ERR(clumsyClass);
	}
	printk(KERN_INFO "Device class created successfully");
	
	//create device
	devno = MKDEV(major_number, minor_number);
	clumsyDevice = device_create(clumsyClass, NULL, devno, NULL, DEVICE_NAME);
	if(IS_ERR(clumsyDevice)){
			class_destroy(clumsyClass);
			unregister_chrdev(major_number, DEVICE_NAME);
			printk(KERN_ERR "Failed to create device\n");
			return PTR_ERR(clumsyDevice);
	}
	printk(KERN_INFO "Created device with devno %d\n", devno);
	
	
	//allocate buffer
	buffer = kmalloc(buffer_size * sizeof(char), GFP_KERNEL);
	if(!buffer){
			printk(KERN_ERR "There was an error while allocating buffer\n");
			return -ENOBUFS;
	}
	memset(buffer, 0, buffer_size * sizeof(char));
	length = 0;
	printk(KERN_DEBUG "Allocated %d bytes\n", buffer_size);
	
	
	return 0;
}

static void clumsy_cleanup_module(void){
	printk(KERN_ALERT "Clening up cdriver module\n");
	
	
	kfree(buffer);
	printk(KERN_INFO "Freed buffer\n");
	class_unregister(clumsyClass);
	printk(KERN_INFO "Unregistered class\n");
	class_destroy(clumsyClass);
	printk(KERN_INFO "Destroyed class\n");
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "Unregistered device \n");
}

static int clumsy_open(struct inode * inode, struct file * filp){
	printk(KERN_INFO "Opening driver\n");
	return 0;
}

static ssize_t clumsy_read(struct file * filp, char * buf, size_t count, loff_t * fpos){
	
		int result;
		int i;
		int rand_num=-1;
		int t_count=0;//Temp counter
		char *temp = kmalloc(sizeof(char)*length, GFP_KERNEL);
		printk(KERN_INFO "###Buffer:%s Length:%d\n", buffer, length);
		
		//Distrupting data
		if(mode==1){//Regular mode, every nth character
			for(i=0;i<length;i++)
			{
				if((i+1)%n!=0)
					temp[t_count++]=buffer[i];
			}
		}else if(mode==2){//Random characters will be dropped
			rand_num = get_random(length);
			for(i=0;i<length;i++)
			{
				if(i==rand_num){
					printk("1stDEBUG: RANDNUM: %d\n",rand_num);
					rand_num += get_random(length);
					printk("2ndDEBUG: RANDNUM: %d\n",rand_num);
				}else
					temp[t_count++]=buffer[i];	
			}
			

		}
		
		printk(KERN_INFO "###Buffer:%s Length:%d\n", buffer, length);
		result = copy_to_user(buf, temp, t_count);
		printk(KERN_INFO "###buf: %s\n", buf);
		if(result == 0){
			printk(KERN_INFO "%d characters read from device\n", length);
			return (length = 0);
		} else {
			printk(KERN_ERR "There was an error while reading buffer");
			return -EFAULT;
		}
}

int get_random(int length){
	int randnum;
	get_random_bytes(&randnum, sizeof(int));
	randnum=randnum % length;//Select a random num in length range
	return randnum < 0 ? -randnum : randnum;
}

static ssize_t clumsy_write(struct file * filp, const char * buf, size_t count, loff_t * fpos){
	
		sprintf(buffer, "%s", buf);
		length = strlen(buffer);
		printk(KERN_INFO "Recieved %zu characters\n", count);
		return count;
}

static int clumsy_release(struct inode * inode, struct file * filp){
	printk(KERN_INFO "Driver released\n");
	return 0;
}


long clumsy_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	
	int err = 0, tmp;
	int retval = 0;
	
	if (_IOC_TYPE(cmd) != CLUMSY_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > CLUMSY_IOC_MAXNR) return -ENOTTY;
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;
	
	if (! capable (CAP_SYS_ADMIN))
		return -EPERM;
		
	switch(cmd){
		case CLUMSY_IOCSMODE:
			if(arg != 1 && arg != 2)
				return -EINVAL;
			mode = arg;
			break;
		case CLUMSY_IOCSNUM:
			if(arg < 0)
				return -EINVAL;
			n = arg;
			break;
				
		}
}

module_init(clumsy_init_module);
module_exit(clumsy_cleanup_module);
