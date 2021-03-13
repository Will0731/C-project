#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//password

typedef struct user{
	int no;
	char id[10];
	int bal;
	
}user;

void beting(int bal[],int be){
	size_t i,no=1;
	for(i=0;i<2;i++){
	bal[i]=bal[i]-be;
	printf("player%d is %d\n",no,bal[i]);
	no++;
	}
}

void print_bal(int bal[]){
	size_t i,no=1;
	for(i=0;i<2;i++){
		printf("player%d:%d\n",no,bal[i]);
		no++;
	}
}

void newuser(FILE*f){
	int r,i;
	fseek(f,0,SEEK_END);
	r=ftell(f)/sizeof(user);
	rewind(f);
	user userin={r+1,"",1000};
	unsigned int no=0,num=0;
	puts("Enter new user name");
	scanf("%9s",&userin.id);
	fseek(f,0,SEEK_END);
	fwrite(&userin,sizeof(user),1,f);
	printf("%d.%9s\t%d\n",userin.no,userin.id,userin.bal);
}

int input_user(FILE*f, int n){ 

   user player={0,"",0};

              
   fseek( f, ( n - 1 ) * sizeof( user ), SEEK_SET );                                              

   
   fread( &player, sizeof(user), 1, f );

   
   if ( player.no == 0 ) {
      printf( "Account #%d has no information.\n", n );
   } 
   else { 
      printf("%d.%9s\t%d\n", player.no,player.id, player.bal);
      return player.bal;
   } 
}

void output_user(FILE*f,int n,int bala){
	user player={0,"",0};
	fseek( f, ( n - 1 ) * sizeof( user ), SEEK_SET );                                              
	fread( &player, sizeof(user), 1, f );
	if ( player.no == 0 ) {
    	printf( "Account #%d has no information.\n", n );
	} 
	else { 
    	player.bal=player.bal*0+bala;
    	fseek( f, ( n - 1 ) * sizeof( user ), SEEK_SET );
    	fwrite( &player, sizeof(user), 1, f );
	}
}
int main(int argc, const char * argv[]) {
    int keyin,answer,min=1,max,range,count=1,bet,record,i,option,no1,no2;
    int balance[2]={0};
    FILE*fptr;
    fptr=fopen("user.txt","r+b");
	while(1){
		printf("1.Create a new user?Y=1,N=2\n");
		scanf("%d",&option);
		if(option==1)
			newuser(fptr);
		else if (option==2)
			break;
	}
	puts("Enter your user number");
	scanf("%d",&no1);
	balance[0]=input_user(fptr,no1);
	puts("Enter your user number");
	scanf("%d",&no2);
	balance[1]=input_user(fptr,no2);
	printf("Please enter the range from 1 to ");
    scanf("%d",&range);
    puts("How much money do you want to bet");
    scanf("%d",&bet);
    for(i=0;i<2;i++){
    	while(balance[i]-2*bet<0){
    		puts("not enough money");
			scanf("%d",&bet);
		}
	}
    beting(balance,bet);
    do{
		srand((unsigned)time(NULL));
    	answer=rand()%range+1;
	}while(answer==1 || answer==range);
    printf("Please enter the number from 1 to %d\n",range);
    max=range;
	do {
		if(count%2==1)
			printf("player%d\n",no1);
		else
			printf("player%d\n",no2);
		scanf("%d",&keyin);
        if (answer!=keyin) {
        	if(keyin>max || keyin<min){
        		puts("Out of range,Please re-enter");
			}
			else{
				if (keyin<answer) {
	                printf("%d to %d\n",keyin,max);
	                min=keyin;
	                count++;
	            }
	            else{
	                printf("%d to %d\n",min,keyin);
	                max=keyin;
					count++;
	            }
	    	}
        }
        else{
        	if(count%2==1){
        		printf("Boom!Winner is player%d\n",no2);
        		if(count<=6){
        			balance[1]=balance[1]+3*bet;
        			balance[0]=balance[0]-bet;
        		}
        		else{
					balance[1]=balance[1]+2*bet;
				}
        	}
        	else{
        		printf("Boom!Winner is player%d\n",no1);
        		if(count<=6){
        			balance[0]=balance[0]+3*bet;
        			balance[1]=balance[1]-bet;
        		}
				else{
					balance[0]=balance[0]+2*bet;
				}
        	}
			break;
		}
    } while (1);
    print_bal(balance);
    output_user(fptr,no1,balance[0]);
    user player={0,"",0};
	fseek(fptr,(no2-1)*sizeof(user),SEEK_SET);                                              
	fread(&player,sizeof(user),1,fptr);
	if (player.no == 0){
    	printf( "Account #%d has no information.\n", no2 );
	} 
	else { 
    	player.bal=player.bal*0+balance[1];
    	fseek( fptr, (no2-1)*sizeof(user),SEEK_SET);
    	fwrite(&player,sizeof(user), 1, fptr );
	}
	//outpur_user(fptr,no2,balance[1]);
    fclose(fptr);
    return 0;
}
