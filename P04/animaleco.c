#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>     /* Needed for function module_param */
#include <linux/kernel.h>          /* Needed for priority messages     */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhumitha Tolakanahalli Pradeep");
MODULE_DESCRIPTION("Kernel Module sorts string array and processes it into two linked lists. Input parameters are Animal Types and Count");

static char * animalType[50];
static int animalCount = 0;
module_param_array(animalType,charp,NULL,0644);          /* User can Read & Write, Group and Others can Read */
module_param(animalCount,int, 0644);           /* User can Read & Write, Group and Others can Read */

static char * animals[10] = { "cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat"};

void animaleco_callback(unsigned long data)
{
	printk(KERN_INFO "animaleco callback funciton \n");
}

int __init animaleco_init(void)
{

	printk(KERN_INFO "Initializing animaleco kernel module...\n");

	/* Sort array alphanetically */
	for ( i = 0; i < n; i++)
	{
		for (j = i+1; j < n; j++)
		{
			if (strcmp(animals[i], animals[j]) > 0) 
			{
            			char* temp = animals[i];
				animals[i] = animals[j];
				animals[j] = temp;
         		}
         	}
	}

	/* Remove duplicates in the Animal Array */
	int i = 0;int j = 0;int k = 0;int n = 10;
	for( i = 0; i < n; i++ )
	{
		for( j = i+1; j < n;)
		{
			if( animals[j]==animals[i] )
			{
				printk(KERN_INFO "[%s] is repeared \n",animals[i]); 
				for( k = j; k < n; k++ )

				{
					animals[k] = animals[k+1];
				}
				n--;
			}
			else
				j++;
		}
	}


	for( i = 0; i< n; i++ )
		printk( KERN_INFO"[%s] ", *(animals+i));
}

void __exit animaleco_exit(void)
{
	printk("Exiting animaleco kernel module...\n");
}

module_init(animaleco_init);
module_exit(animaleco_exit);
