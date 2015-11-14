#include<stdio.h>
#include"char2bool.h"

const int block_size=7;
int main(){
    char b[block_size];
    char d[block_size];
    char c[block_size*8];
    int i;
    for(i=0;i<block_size;i++){
        b[i]='a';
        printf("%c ",b[i]);
    }
    printf("\n");

    bool2char(b,c,block_size,block_size);
    for(i=0;i<block_size*8;i++){
        printf("%c ",c[i]);
    }
    printf("\n");

    char2bool(c,d,block_size);
    for(i=0;i<block_size;i++){
        printf("%c ",d[i]);
    }
    printf("\n");
}
