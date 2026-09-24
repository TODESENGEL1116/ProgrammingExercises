#include<stdio.h>


int main(){
	
	int c,prev_space;
	prev_space=0;
	
	printf("Enter:\n");
	
	while((c=getchar())!=EOF){
		if(c==' '){
			if(!prev_space){
				putchar(c);
				prev_space=1;
			}
			
		}
		else{
			putchar(c);
			prev_space=0;
		}
		
	}
	return 0;
}