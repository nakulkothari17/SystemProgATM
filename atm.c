#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "header.h"

int count = 0;
//struct
struct user_details
{
	long int card_no;
	int pin;
	char type;
	double bal;
};

struct user_details UD;
//struct UD arr[200];

/*For cleaing the stdin
void clean_stdin(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}*/

void display_details()
{
	FILE *ptr;
	//reading
	ptr = fopen("acc_details.txt","r");
	if(ptr == NULL)
	{
		printf("\nERROR File not opened\n");
		return ;
	}

	char data[300];

	while(fgets(data, sizeof(data), ptr))
	{
		char *point;
		point = strtok(data," ");
		char *tmp;

		UD.card_no=strtol(point,&tmp,10);
		//strcpy(UD.card_no, point);
		printf("card_no : %ld\n",UD.card_no);

		point = strtok(NULL," ");
		UD.pin=atoi(point);
		//strcpy(UD.pin, point);
		printf("pin : %d\n",UD.pin);

		point = strtok(NULL," ");
		UD.type=*point;
		//strcpy(UD.type, point);
		printf("type : %c\n",UD.type);

		point = strtok(NULL," ");
		UD.bal=strtod(point,&tmp);
		//strcpy(UD.bal, point);
		printf("bal : %0.2f\n",UD.bal);

		count++;
	}
	fclose(ptr);
}



void user_input()
{
	int i,choice,count=0,flag = 0,found = 0;
	double amount;
	char card_no[16],pin[10],type;
	char pin_tmp[10],type_tmp,amount_tmp[20];

	
	FILE *ptr;
	//reading
	ptr = fopen("acc_details.txt","r");
	if(ptr == NULL)
	{
		printf("\nERROR File not opened\n");
		return ;
	}

	char data[300];

	signal(SIGALRM,alarm_card_error);
	printf("Enter your card number \n");
	alarm(3);
	scanf("%s",card_no);
	signal(SIGALRM,SIG_IGN);
	//strcat(card_no,"\n");
	while(fscanf(ptr,"%s",data)!=EOF)
	{
		//printf("%s\n",data);
		count++;
		int size = strlen(data);
		//printf("%c ",data[i]);
		if (data[size-1] == '\n')
		{
			data[size-1] = 0;
		}

		//for varifing
		if(count == 1)
		{
			
			if (strcmp(card_no,data) == 0)
			{
				printf("Card Number is valid\n");
				flag = 1;
				found = 1;
			}
		}
		//storeing
		else if(count == 2 && flag == 1)
		{
			strcpy(pin_tmp,data);
		}
		else if(count == 3 && flag == 1)
		{
			type_tmp = *data;
		}
		else if(count == 4)
		{

			count = 0;
			if (flag == 1)
			{
				strcpy(amount_tmp,data);
				break;
			}
			flag = 0;
		}
	}
	if (found == 0)
	{
		printf("ERROR card number not verified\n");
		return ;
	}
	//printf("%s %c %s \n",pin_tmp, type_tmp ,amount_tmp );

	printf("Enter your pin \n");
	scanf("%s",pin);
	if (strcmp(pin,pin_tmp) != 0)
	{
		printf("ERROR\n");
		return ;
	}

	//clean_stdin();
	getchar();
	printf("What accout you hold?\n Enter S for Savings \n Enter C for Current\n");
	scanf("%c",&type);
	if (type != type_tmp)
	{
		printf("ERROR\n");
		return ;
	}

	printf("Press 1 for Balance inquiry \nPress 2 for withdrawal\n");
	scanf("%d",&choice);

	if(choice == 1)
	{
		printf("you have balance of %s\n",amount_tmp);
	}
	else if (choice == 2)
	{
		
		double bal;
		char *tmp;
		char str[100];
		memset(str,0,strlen(str));
		//strcat(str,card_no);
		sprintf(str,"%s %s %c %s",card_no,pin_tmp,type_tmp,amount_tmp);
		//printf("%s---",str);
		bal=strtod(amount_tmp,&tmp);
		withdrawal(bal,str);
	}

}

int main()
{
	display_details();
	user_input();
}