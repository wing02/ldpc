#include"char2bool.h"
void bool2char(char *b,char* c,int block_size,int length){//length:data length 
    int i,j;
    for(i=0;i<block_size;i++){
        if(i>=length){
            for(j=0;j<8;j++){
                c[8*i+j]=0; 
            }
            continue;    
        }
        for(j=0;j<8;j++){
            char flag=1<<(7-j);
            if((flag & b[i] )== flag){
                c[8*i+j]=1; 
            }else{
                c[8*i+j]=0;
            }
        }
    }
}



void char2bool(char* c,char * b,int block_size){
    int i,j;
    memset(b,0,sizeof(char)*block_size);
    for(i=0;i<block_size;i++){
        for(j=0;j<8;j++){
            if(c[8*i+j]==1){
                b[i]=b[i]+(1<<(7-j));
            }
        }
    }
}

