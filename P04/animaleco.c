#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/list_sort.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhumitha Tolakanahalli Pradeep");
MODULE_DESCRIPTION("Kernel Module sorts string array and processes it into two linked lists. Input parameters are Animal Types and Count");

static char * animalType= NULL;
static int animalCount = 0;
//module_param_array(animallist,charp,NULL,0644);          /* User can Read & Write, Group and Others can Read */
module_param(animalType,charp,0644);		/* User can Read & Write, Group and Others can Read */
module_param(animalCount,int, 0644);           /* User can Read & Write, Group and Others can Read */
static char * animals[50] = {"cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat","cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat","cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat","cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat","cat","dog","cat","dog","rhino","ant","cat","rat","rhino","bat"};

/* Node for Linked List of Ecosystem */
struct animal_ecosystem
{
	char * animal_type;
	int animal_count;
	struct list_head eco_list;
};

/* Node for Filtered Linked List */
struct filtered_ecosystem
{
	char * animal_type;
	int animal_count;
	struct list_head fil_list;
};

/*Initialize the List Heads */
LIST_HEAD(ecosystem);
LIST_HEAD(filecosystem);

/*
void animaleco_callback(unsigned long data)
{
	printk(KERN_INFO "animaleco callback funciton \n");
}*/

/* Kernel Init function */
static int __init animaleco_init(void)
{
	
	struct animal_ecosystem *aeptr1, *aeptr2;
	struct filtered_ecosystem *feptr1, *feptr2;
	struct list_head *pos;
	struct timespec ts;
	unsigned long begint_ns = 0;
	unsigned long endt_ns = 0;
	int i = 0;int j = 0;int k = 0; int cnt = 0;
	int n = 50; //Number of Nodes of Eco-system Linked list
	getnstimeofday(&ts);
	begint_ns = ts.tv_sec + (ts.tv_nsec*1000000000);
	printk(KERN_INFO "Initializing animaleco kernel module...\n");

	/* Sort array alphabetically */
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
	for( i = 0; i < n; i++ )
	{
		cnt = 1;
		for( j = i+1; j < n;)
		{
			if( animals[j]==animals[i] )
			{
			 	cnt++;		//Incrementing animal count
				printk(KERN_INFO "[%s] : Duplicate Entry Found \n",animals[i]); 
				for( k = j; k < n; k++ )

				{
					animals[k] = animals[k+1];
				}
				n--;
			}
			else
				j++;
		}
 		aeptr1 = (struct animal_ecosystem *)kmalloc(sizeof(struct animal_ecosystem),GFP_KERNEL);
		/* Check for NULL Pointer */
		if( aeptr1==NULL )
		{
			printk(KERN_ERR "Error in kmalloc on Node for Animl Ecosystem");
			return -ENOMEM;
			
		}
		aeptr1->animal_type=animals[i];
		aeptr1->animal_count=cnt;
		
		/* Added newly allocated pointer to animal_ecosystem Linked List */
		list_add(&aeptr1->eco_list,&ecosystem);

	}


	/* Print the Linked List of all animals in the ecosystem */
	printk( KERN_INFO "List of animals in the Ecosystem of Size [%d]\n",n);
	list_for_each(pos,&ecosystem)
	{
		aeptr2 = list_entry(pos, struct animal_ecosystem, eco_list);
		printk("Animal : [%s] | Number : [%d]", aeptr2->animal_type, aeptr2->animal_count);
	
	}
	
	/* Filtered Linked List */
	
	/* Filter Option 1 : No input : Copy animals of the entire ecosystem into filtered list */
	if ((animalType==NULL) && (animalCount==0))
	{
		printk(KERN_INFO "No Filter Criteria input. List of animal ecosystem consisting of %d animals \n",n);
		/* Copy animal nodes from animal_ecosytem Linked List to filtered_ecosystem Linked List */
		list_for_each_entry(aeptr2,&ecosystem,eco_list)
		{
			/* Dynamically allocate memory for filtered Linked List */
			feptr1 = (struct filtered_ecosystem *)kmalloc(sizeof(struct filtered_ecosystem), GFP_KERNEL);
			/*Check for NULL Pointer */
			if( feptr1==NULL )
			{
				printk(KERN_ERR "Allocation of memory for Filtered List failed \n");
				return -ENOMEM;
			}
			feptr1->animal_type = aeptr2->animal_type;
			feptr1->animal_count = aeptr2->animal_count;

			/* Add the newly created Node to linked list filecosystem */
			list_add(&feptr1->fil_list,&filecosystem);
		}

		/* Print all the animals in the filtered ecosystem */
		printk(KERN_INFO " List of animals in the Ecosystem of Size [%d] \n",n);
		list_for_each(pos,&filecosystem)
		{
			feptr2 = list_entry(pos, struct filtered_ecosystem, fil_list);
			printk(KERN_INFO "Animal : [%s] Count : [%d]",feptr2->animal_type,feptr2->animal_count);
		}
		
		printk("ECOSYSTEM SIZE : [%d] MEMORY ALLOCATED FOR FILTERED LIST : [%zu] \n",n,(n*sizeof(struct filtered_ecosystem)));
	
	}

	/* Filter Criteria 2 : Animal Type id specified */
	else if ( animalType!=NULL && animalCount==0 )
	{
		n=0;
		printk(KERN_INFO "FILTER BASED ON ANIMAL TYPE : %s\n",animalType);
		list_for_each_entry(aeptr2,&ecosystem,eco_list)
		{
			/* Check if animaltype is found in the ecosystem */
			if(strcmp(animalType,aeptr2->animal_type)==0)
			{
				/* Dynamically allocate memory */
				feptr2 = (struct filtered_ecosystem *)kmalloc(sizeof(struct filtered_ecosystem),GFP_KERNEL);
				if(!feptr2) 
				{
					printk (KERN_ERR "Failed to dymaically allocate node for FIltered List");
					return -ENOMEM;
				}
				
				feptr2->animal_type = aeptr2->animal_type;
				feptr2->animal_count = aeptr2->animal_count;
				/* Add Nodes to FIltered List */
				list_add(&feptr2->fil_list,&filecosystem);
				n++;
				printk(KERN_INFO "[%d] OF ANIMAL TYPE [%s] FOUND \n",feptr2->animal_count,feptr2->animal_type);
			}
		}
		printk("ECOSYSTEM SIZE : [%d] MEMORY ALLOCATED FOR FILTERED LIST : [%zu] \n",n,(n*sizeof(struct filtered_ecosystem)));

	}

	/* Filter Criteria 3 : Animal Count specified */
	else if ( animalType==NULL )
	{
		n=0;
		printk(KERN_INFO "FILTER BASED ON ANIMAL COUNT : %d\n",animalCount);
		list_for_each_entry(aeptr2,&ecosystem,eco_list)
		{
			/* Check if Animal Count > input */
			if( aeptr2->animal_count>animalCount ) 
			{
				/* Dynamically allocate memory */
				feptr2 = (struct filtered_ecosystem *)kmalloc(sizeof(struct filtered_ecosystem),GFP_KERNEL);
				if(!feptr2) 
				{
					printk (KERN_ERR "Failed to dymaically allocate node for FIltered List");
					return -ENOMEM;
				}
				
				feptr2->animal_type = aeptr2->animal_type;
				feptr2->animal_count = aeptr2->animal_count;
				/* Add Nodes to FIltered List */
				list_add(&feptr2->fil_list,&filecosystem);
				n++;
				printk(KERN_INFO "ANIMAL TYPE [%s] GREATER THAN %d FOUND \n",feptr2->animal_type,animalCount);
			}
		}
			 printk("ECOSYSTEM SIZE : [%d] MEMORY ALLOCATED FOR FILTERED LIST : [%zu] \n",n,(n*sizeof(struct filtered_ecosystem)));


	}

	/* Filter Criterion 4 : Animal Type and Count Specified */
	else
	{	
		n=0;
		printk(KERN_INFO "FILTER BASED ON ANIMAL COUNT > %d & ANIMAL TYPE : %s\n",animalCount,animalType);
		list_for_each_entry(aeptr2,&ecosystem,eco_list)
		{
			/* Check if animaltype is found in the ecosystem */
			if( (aeptr2->animal_count>animalCount) && (!strcmp(animalType,aeptr2->animal_type)))
			{
				/* Dynamically allocate memory */
				feptr2 = (struct filtered_ecosystem *)kmalloc(sizeof(struct filtered_ecosystem),GFP_KERNEL);
				if(!feptr2) 
				{
					printk (KERN_ERR "Failed to dymaically allocate node for FIltered List");
					return -ENOMEM;
				}
				
				feptr2->animal_type = aeptr2->animal_type;
				feptr2->animal_count = aeptr2->animal_count;
				/* Add Nodes to FIltered List */
				list_add(&feptr2->fil_list,&filecosystem);
				n++;
				printk(KERN_INFO "ANIMAL TYPE [%s] WITH COUNT GREATER THAN %d FOUND \n",feptr2->animal_type,animalCount);
			}
		}
			 printk("ECOSYSTEM SIZE : [%d] MEMORY ALLOCATED FOR FILTERED LIST : [%zu] \n",n,(n*sizeof(struct filtered_ecosystem)));
	}

	getnstimeofday(&ts);
	endt_ns = ts.tv_sec + (ts.tv_nsec*1000000000);
	printk("Time taken to insert Kernel Module : [%lu ns] \n",(endt_ns-begint_ns));
       return 0;
       
}

void __exit animaleco_exit(void)
{
	/* For calculating time to exit kernel moduele */
	struct timespec ts;
	struct list_head *pos1,*pos2,*tmp;
	struct animal_ecosystem * temp1;
	struct filtered_ecosystem * temp2;
	unsigned long eco_bytes = 0, fil_bytes = 0;
	unsigned long begint_ns = 0, endt_ns = 0;
	printk("Exiting animaleco kernel module...\n");

	getnstimeofday(&ts);
	begint_ns = (ts.tv_sec*1000000000) + (ts.tv_nsec);
		/* For Ecosystem List */
	list_for_each_safe(pos1, tmp, &ecosystem) 
   	{
        	if(pos1 != NULL)
        	{
   			temp1 = list_entry(pos1,struct animal_ecosystem,eco_list);
			list_del(pos1);
	    		kfree(temp1);
	    		eco_bytes++;
       		 }
	}
	/* For Filtered Ecosystem list */
	list_for_each_safe(pos2, tmp, &filecosystem) 
    	{
       		if(pos2 != NULL)
        	{
            		temp2 = list_entry(pos2,struct filtered_ecosystem,fil_list);
			list_del(pos2);
            		kfree(temp2);
	   		fil_bytes++;
        	}
    	}
	getnstimeofday(&ts);
	endt_ns = (ts.tv_sec*1000000000) + (ts.tv_nsec);

	
    	printk("Ecosystem Linked List Memory Freed : [%lu B]",sizeof(struct animal_ecosystem)*eco_bytes);
	printk( "Filtered Ecosystem Linked List Memory Freed : [%lu B] \n Time taken to exit : %lu ns \n",sizeof(struct filtered_ecosystem)*fil_bytes,(endt_ns-begint_ns));
	printk("....");
}


module_init(animaleco_init);
module_exit(animaleco_exit);
