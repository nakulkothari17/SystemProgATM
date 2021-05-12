#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


void replace(char *str, char *newWord)
{
    char tempstr[300];
    int temp = 0, j = 0; 
    memset(tempstr, 0, 100);
    for(int i = 0; i < strlen(str); i++){
       if(str[i] == ' '){
           temp++;
       }
       if(temp == 3){
            tempstr[j++] = ' ';
            strcat(tempstr, newWord);
            break;
        }
        tempstr[j] = str[i];
        j++;
    }
    strcpy(str, tempstr);
    memset(tempstr, 0, 100);
}

void alarm_card_error()
{
	printf("you have not entered your card \n");
	exit(0);
}

void alarm_handeler()
{
	printf("visit nearest branch as amount is deducted but you have not taken it\n");
	printf("Sorry for inconvenience \n");
	exit(0);
}

void withdrawal( double amount_tmp,char *str)
{
	double wd_amt,new_bal;
	char temp_amt[40];
	printf("Enter the amount you want to withdraw.\n");
	scanf("%lf",&wd_amt);
	

	printf("checking your balance");
	if(wd_amt<amount_tmp)
	{
		printf("processiong transaction\n");
	}
	else
	{
		printf("Insufficient withdrawal amount\n");
		return ;
	}

	new_bal=amount_tmp-wd_amt;
	sprintf(temp_amt,"%f",new_bal);

	FILE *f1,*f2;
	f1 = fopen("acc_details.txt","r");
	f2 = fopen("replace.txt","w");

	char data[300];
	while(fgets(data, sizeof(data), f1))
	{
		if(data[strlen(data)-1]=='\n')
		{
			data[strlen(data)-1]=0;
		}
		if(strcmp(data,str)==0)
		{
			//printf("%s",data);
			replace(data,temp_amt);
		}
		strcat(data,"\n");
		fputs(data,f2);
	}
	fclose(f1);
	fclose(f2);

	int del=remove("acc_details.txt");
	if(!del)
	{
		rename("replace.txt","acc_details.txt");
		signal(SIGALRM,alarm_handeler);

		printf("press d to collect your cash %f\n",wd_amt);
		alarm(10);
		char c ;
		while(1)
		{
			c= getchar();
			if (c=='d')
			{
				signal(SIGALRM,SIG_IGN);
				printf("Have a good day\n");
				break;
			}
		}	
		//printf("press d to collect your cash\n");
	}
	else
	{
		printf("file not updated");
	}

	//printf("please take your %f\n",wd_amt);
	//printf("%f",amount_tmp);
}