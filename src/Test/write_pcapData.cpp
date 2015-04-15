#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// g++  -o write_pcapData.o write_pcapData.cpp
//D59B24371B66E7
/*write from sql into file, then test ac is right or not*/
void  test_write_tcp_content(int dst_port, char *content)
{
    //char * temp_content = (char *) malloc (sizeof(char)*length);
    //  strcpy(temp_content,content);
    char fileName[10];
    sprintf(fileName,"%d",dst_port);

    FILE * pFile;
    pFile = fopen ( fileName , "ab" );//ab binary

    int i=0;
    while(content[i])
    {
        fwrite (&content[i] , 1, 1 , pFile );
        i++;
    }

    // fwrite (temp_content , length, 1 , pFile );

    fclose (pFile);

}

int main()
{
    int dst_port=8080;
    char * content="D59B24371B66E7";
    test_write_tcp_content(dst_port, content);
}