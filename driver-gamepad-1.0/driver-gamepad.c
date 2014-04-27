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

#include <linux/interrupt.h>
#include "efm32gg.h"


#define IRQ_N_GPIO_EVEN 17
#define IRQ_N_GPIO_ODD  18
#define IRQ_NAME_GPIO_EVEN "gamepad_IRQ_gpio_even"
#define IRQ_NAME_GPIO_ODD  "gamepad_IRQ_gpio_odd"

#define GPIO_MEM_REGION_LENGTH 0x11c
#define GPIO_EXT_MEM_REGION_LENGTH 0xc
#define CMU_MEM_REGION_LENGTH 0x44

///////////
//Storage variables
//////////
static dev_t first; 		//Global variable for the first device number
static struct cdev char_device;	//Variable for character device structure
static struct class *device_class; 	//Global variable for the device class

static struct task_struct *pid_type  = 0;   //information about current active program
static struct siginfo signal_info;                 //signal info

static char gamepad_button_status = 0;      //storage for gamepad data


/////////////
//Prototyes
/////////////
void setup_GPIO();
static int send_signal(void);
static irq_handler_t  gamepad_interrupt_handler(int irq, void *dumme, struct pt_regs *regs);
static int requestIRQ(void);
static void freeIRQ(void);
static void setup_signal();


//////////
//Char drive interaction functions (open, release, read)
//
//////////////

//only one application at a time can access the gamepad
static int gamepad_open(struct inode *i, struct file *f)
{
	printk("Gamepad open\n");
	pid_type = pid_task(find_vpid(current->pid), PIDTYPE_PID); 
	if(pid_type == NULL){                                       
	    printk("opening file failed \n");
		return -1;
	}
	return 0;
}
static int gamepad_release(struct inode *i, struct file *f)
{
	kfree(pid_type);
	pid_type = 0;
	return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  if(copy_to_user(buf, &gamepad_button_status,1) != 0){
    printk("read error \n");
    return -EFAULT;
  }
  return 1;
}

//////////
//Char drive setup and cleaning
//
//////////////

static struct file_operations gamepad_fileOps = 
{
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = my_read
};

static void __exit gamepad_cleanup(void)
{
	//clean up pointers
	kfree(pid_type);
	//Stop interrupts
	freeIRQ();
	//free memory region
	release_mem_region(GPIO_PC_BASE,(resource_size_t)GPIO_MEM_REGION_LENGTH);
	
	//remove device from filesystem
	cdev_del(&char_device);
	device_destroy(device_class,first);
	class_destroy(device_class);
	unregister_chrdev_region(first,1);
}

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
	//request memory access
	struct resource *r_GPIO_PC;
	r_GPIO_PC = request_mem_region(GPIO_PC_BASE,(resource_size_t)GPIO_MEM_REGION_LENGTH,"gamepad");
	if(r_GPIO_PC == NULL){
		printk("GPIO_PC mem request failed\n");
		return -1;
	}
	
    //request interrupts
	if(requestIRQ() != 0){
		printk("Failed to request IRQ\n");
		return -1;		
	}
	
	printk("Device creation finnished\n");

    //Config GPIO
    setup_GPIO();   

    //Setup internal variables
	pid_type = NULL;
	setup_signal();
	
    printk("<Major, Minor>: <%d,%d>\n",MAJOR(first),MINOR(first));
	printk("Gamepad module is set up and  ready to go!!!!\n");
	return 0;
}

static int requestIRQ(void){
	int status;	
	status = request_irq(IRQ_N_GPIO_ODD,(irq_handler_t) gamepad_interrupt_handler,0,IRQ_NAME_GPIO_ODD, NULL);
	if(status != 0){
		printk("IRQ GPIO ODD request failed");
		return status;
	}
	status = request_irq(IRQ_N_GPIO_EVEN,(irq_handler_t) gamepad_interrupt_handler,0,IRQ_NAME_GPIO_EVEN, NULL);
	if(status != 0){
		free_irq(IRQ_N_GPIO_ODD, NULL);
		printk("IRQ GPIO EVEN request failed");	
		return status;
	}

	return 0;
}

static void freeIRQ(void){
	free_irq(IRQ_N_GPIO_EVEN, NULL);
	free_irq(IRQ_N_GPIO_ODD, NULL);

}




void setup_GPIO(){
    //Init GPIO 	
	*GPIO_PC_MODEL = 0x33333333;			//Set pins as input
	*GPIO_PC_DOUT  = 0xff;

	/* GPIO interrupts */
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff;
}

///////////
// Signal and intterrupt handling
// Receive HW intterrupts and notify user space
///////////////
static irq_handler_t  gamepad_interrupt_handler(int irq, void *dumme, struct pt_regs *regs){
	gamepad_button_status = (char)*GPIO_PC_DIN;
	send_signal();
	*GPIO_IFC = 0xff; //clear interrupt
	return (irq_handler_t) IRQ_HANDLED;
}

static void setup_signal(){
	//Setting up signal info
	memset(&signal_info, 0, sizeof(struct siginfo));
	signal_info.si_signo = SIGIO;
	signal_info.si_code = SI_QUEUE;
	signal_info.si_int = 1234;
}

static int send_signal(void){

	//Do the actuall sending of signal
	if(pid_type == 0){
		return -1;
	}
	if(send_sig_info(SIGIO, &signal_info, pid_type) < 0){
		return -1;
	}
	return 0;
}


///////
//
////////
module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Gamepad module for the course TDT4258, spring 2014");
MODULE_LICENSE("GPL");
