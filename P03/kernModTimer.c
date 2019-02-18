#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>     /* Needed for function module_param */
#include <linux/kernel.h>          /* Needed for priority messages     */
#include <linux/timer.h>	   /* Needed for Kernel Timer          */
#include <linux/delay.h>

static char* ownerName = "Madhumitha";
static unsigned short timerWakeUp = 500;     /* Timer wakeup period */

module_param(ownerName,charp,0644);          /* User can Read & Write, Group and Others can Read */
module_param(timerWakeUp,ushort, 0644);      /* User can Read & Write, Group and Others can Read */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhumitha Tolakanahalli Pradeep");
MODULE_DESCRIPTION("Kernel Timer Module with confugurable wake-up period (default 500ms). Owner Name and Time-Out values are input as parameters");

struct timer_list kernModTimer;
static unsigned long timerCount = 0;

void kernMod_callback(unsigned long data)
{
	timerCount++;		   	    /* Increment count each time the callback function is entered */
	printk(KERN_INFO "| Kernal Timer Fire Count : [%lu] \t | Kernel Owner : [%s] |\n",timerCount,ownerName);
	mod_timer(&kernModTimer, jiffies+msecs_to_jiffies(timerWakeUp));
	return;
}

int __init kernModTimer_init(void)
{
	printk(KERN_INFO "Initializing Kernel Timer Module...\n");
	setup_timer(&kernModTimer,kernMod_callback,0);
	add_timer(&kernModTimer);
	mod_timer(&kernModTimer, jiffies+msecs_to_jiffies(timerWakeUp));
	return 0;
}


void __exit kernModTimer_exit(void)
{               
       	printk("Exiting Kernel Timer Module...\n");	
	del_timer(&kernModTimer);
}


module_init(kernModTimer_init);
module_exit(kernModTimer_exit);

