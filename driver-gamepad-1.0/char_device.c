/*
 * This is a linux Kernel Module for the course TDT4232, SPring 
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#include <linux/version.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/sched.h>
#include <asm/siginfo.h>
#include <linux/pid_namespace.h>
#include <linux/pid.h>

#include <asm/uaccess.h>
#include <linux/slab.h>

//#include "query_ioctl.h"
#include "efm32gg.h"


#define IRQ_N_GPIO_EVEN 17
#define IRQ_N_GPIO_ODD  18
#define IRQ_NAME_GPIO_EVEN "gamepad_IRQ_gpio_even"
#define IRQ_NAME_GPIO_ODD  "gamaeoad_IRQ_gpio_odd"

#define GPIO_MEM_REGION_LENGTH 0x11c
#define CMU_MEM_REGION_LENGTH 0x44

static dev_t first; 		//Global variable for the first device number
static struct cdev char_device;	//Variable for character device structure
static struct class *device_class; 	//Global variable for the device class
static struct task_struct *pid_type  = NULL;


static int gamepad_led_status = 0;
static int gamepad_button_status = 0;

static int gamepad_open(struct inode *i, struct file *f)
{
	rcu_read_lock();
	printk(KERN_INFO "Gamepad open\n;");
	if(pid_type != NULL){
		rcu_read_unlock();
		return -EBUSY;
	}

	pid_type = pid_task(find_vpid(current->pid), PIDTYPE_PID); 
	if(pid_type == NULL){
		printk("no such pid\n");
		rcu_read_unlock();
		return -1;
	}
	rcu_read_unlock();

	return 0;
}
static int gamepad_release(struct inode *i, struct file *f)
{
	pid_type = NULL;
	printk( KERN_INFO "Gamepad release\n;");
	//TO DO: stop interrupts
	return 0;
}

static int send_signal(void){
	struct siginfo signal_info;

	//Setting up signal info
	memset(&signal_info, 0, sizeof(struct siginfo));
	signal_info.si_signo = SIGIO;
	signal_info.si_code = SI_QUEUE;
	signal_info.si_int = 1234;

	//Do the actuall sending of signal
	if(pid_type == NULL)
		return -1;
	if(send_sig_info(SIGIO, &signal_info, pid_type) < 0){
		printk("Sending signal failed\n");
		return -1;
	}
	return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int gamepad_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long gamepad_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	query_gamepad_t gamepad_data;
	printk(KERN_INFO "ENTER Gamepad ioctl\n;");

	switch (cmd)
	{
		break;
		case READ_GAMEPAD:
		{
			gamepad_data.led = gamepad_led_status;
			gamepad_data.buttons = gamepad_button_status;
			if(copy_to_user((query_gamepad_t*)arg, &gamepad_data, sizeof(query_gamepad_t)))
			{
				return -EACCES;
			}
		}

		break;
		case WRITE2GAMEPAD:
		{
			printk(KERN_INFO "Gamepad ioctl write\n;");
			if(copy_from_user(&gamepad_data, (query_gamepad_t*)arg,sizeof(query_gamepad_t))){
				return -EACCES;
			}
			gamepad_led_status = gamepad_data.led;
			gamepad_button_status = gamepad_data.buttons;
			//send_signal();
		}
		break;
	}
	return 0;
}

static irqreturn_t gamepad_interrupt_handler(int irq, void *dumme, stryct pt_regs *regs){
	gamepad_button_status = *GPIO_PC_DIN;
	send_signal();
	*GPIO_IFC = 0xff; //clear interrupt
}


static int setup_hardware_access_and_interrupt(void){
	//request access to memery region
	struct resource *r = request_mem_region(GPIO_PC_BASE,GPIO_MEM_REGION_LENGTH,) == NULL,"gamepad_device"
	if(r == NULL)
		return -1;
	struct resource *r = request_mem_region(GPIO_PA_BASE,GPIO_MEM_REGION_LENGTH,) == NULL,"gamepad_device"
	if(r == NULL)
		return -1;
	struct resource *r = request_mem_region(CMU_BASE2,CMU_MEM_REGION_LENGTH,) == NULL,"gamepad_device"
	if(r == NULL)
		return -1;
	//request IRQ (interrupt handling) 
	int status = request_irq(IRQ_N_GPIO_ODD, gamepad_interrupt_handler,0,IRQ_NAME_GPIO_ODD, NULL);
	if(status != 0)
		return status;
	status = request_irq(IRQ_N_GPIO_EVEN, gamepad_interrupt_handler,0,IRQ_NAME_GPIO_EVEN, NULL);
	if(status != 0)
		return status;

	//Init GPIO 
	*GPIO_PA_CTRL = 2;  				/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555; 			/* set pins A8-15 as output */

	*GPIO_PC_MODEL = 0x33333333;			//Set pins as input
	*GPIO_PC_DOUT  = 0xff;

	/* GPIO interrupts */
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff;

	return 0;
}

static void clean_hardware_access_and_interrupt(void){
	//Clear GPIO interrupts
	*GPIO_EXTIPSELL = 0x0;
	*GPIO_EXTIFALL = 0x0;
	*GPIO_EXTIRISE = 0x0;
	*GPIO_IEN = 0x0;
	//Turn off GPIO
	*GPIO_PA_CTRL = 0;  				/* set low drive*/
	//Free IRQ
	free(IRQ_N_GPIO_EVEN, NULL);
	free(IRQ_N_GPIO_ODD, NULL);
	//Release memory region
	release_mem_region(CMU_BASE2,CMU_MEM_REGION_LENGTH);
	release_mem_region(GPIO_PA_BASE,GPIO_MEM_REGION_LENGTH);
	release_mem_region(GPIO_PC_BASE,GPIO_MEM_REGION_LENGTH);

}

static struct file_operations gamepad_fileOps = 
{
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = gamepad_ioctl
	#else
    .unlocked_ioctl = gamepad_ioctl
	#endif
};

static int __init gamepad_init(void)
{
	//allocate chardevice region, device class, char device structure 
	if (alloc_chrdev_region(&first,0,1,"gamepad") < 0){
		return -1;
	}
	if ((device_class = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		unregister_chrdev_region(first,1);
		return -1;
	}
	if (device_create(device_class, NULL, first, NULL, "gamepad_device") == NULL )
	{
		class_destroy(device_class);
		unregister_chrdev_region(first,1);
		return -1;
	}
	cdev_init(&char_device,&gamepad_fileOps);
	if (cdev_add(&char_device,first,1) == -1)
	{
		device_destroy(device_class,first);
		class_destroy(device_class);
		unregister_chrdev_region(first,1);
		return -1;
	}
	//TODO: Ask for access to I/O Ports

	printk(KERN_INFO "<Major, Minor>: <%d,%d>\n",MAJOR(first),MINOR(first));
	printk(KERN_INFO "Hello World, here is your module speaking\n");
	return 0;
}


static void __exit gamepad_cleanup(void)
{
	//if(current_pid != 0)	//Cannot unload when there is a process using the module
	//	return -EBUSY;
	kfree(pid_type);
	cdev_del(&char_device);
	device_destroy(device_class,first);
	class_destroy(device_class);
	unregister_chrdev_region(first,1);
	printk("Short life for a small module...\n");
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Gamepad module for the course TDT4258, spring 2014");
MODULE_LICENSE("GPL");
