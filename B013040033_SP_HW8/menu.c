/*
 *	menu :
 *	A generalised menu function
 *	The argument is base of an array of Menu_item, which
 *	must be suitably NULL-teminated
 */

#include "sms.h"

void menu(Menu_item *menu) {

	while(1)
	{
		printf("1) Listen number of current collections\n\n");
		printf("2) Summarise statistics\n\n");
		printf("3) Re-start statistics\n\n");
		printf("4) Zap stale clients and free memory\n\n");
		printf("Please choose (1 - 4):");
		int choose_num=0;
		scanf("%d",&choose_num);

		switch(choose_num)
		{
			case 1:list_conn();				/* list number of current connections 	*/
				break;
			case 2:list_stats();  			/* Summarise statistics					*/
				break;
			case 3:zero_stats();			/* Call disconnect when needed			*/
				break;
			case 4:zap_stale();				/* Call disconnect when needed			*/
				break;
			default:DIE("choose number error");
		}
	}


}
