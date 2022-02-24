
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "sorok.h"
#include "wrapper.h"

#define BMPBUF  4608

int wrapper_sorok(const char *filename,char *byteArray, int length)
{
	FILE *fp;
	PAPITBMP *bmpInfo;
	char buffer[BMPBUF];
	char *szdata, *temp;
	char header[]={0x02};
	int len;

	fp = fopen(filename,"r+b");
	
	if (!fp){
		return WRAPPER_FILE_NOTEXIST;
	}

	fread(buffer,sizeof(char),2,fp);
    if (buffer[0]!='B' && buffer[1]!='M'){
        printf("ralat : ini bukan file bitmap!\n");
        return WRAPPER_FILE_NOTBMP;
    }
    

	len= (int) fread(buffer,sizeof(PAPITBMP),1,fp);
    if (!len){
        
        return WRAPPER_FILE_BMPPROB;
    }

	bmpInfo=(PAPITBMP *) buffer;
	/*letak ayat dlm bitmap*/
    fseek(fp,bmpInfo->offset,SEEK_SET);

	temp = (char *) malloc(WRAPPER_SOROK_BUFFER);
	szdata = (char *) malloc(40);
    
	len= (int) fread(temp,sizeof(char),(length+4+1)*9,fp);
    
    if (len<9)        
        return WRAPPER_FILE_BMPPROB;


	szdata = (char *) &length; /*assign saiz mesej utk di tuliskan*/
    encode(header,1,temp); /*tambah 0x02 header*/
    encode(szdata,4,&temp[9]); /* letak mesej size */
    encode(byteArray,length,&temp[45]); /* baru letak mesej*/

    fseek(fp,bmpInfo->offset,SEEK_SET);
    fwrite(temp,sizeof(char),(length+4+1)*9,fp);

    /*ok pack up semua....*/
	fflush(fp);
    fclose(fp);
    free(temp);
	free(szdata);

	return 0;
}
int wrapper_unsorok(const char *filename, char *byteArray, int *length)
{
	static char number[5];
	FILE *fp;
	PAPITBMP *bmpInfo;
	char buffer[BMPBUF];
	char *temp;
	int *szdata;
	char header[]={0x02};
	int len;
	


	temp = (char *) malloc(WRAPPER_SOROK_BUFFER);
	



	fp = fopen(filename,"rb");
	if (!fp){
		return WRAPPER_FILE_NOTEXIST;
	}
	
	fread(buffer,sizeof(char),2,fp);
    if (buffer[0]!='B' && buffer[1]!='M'){
        printf("ralat : ini bukan file bitmap!\n");
        return WRAPPER_FILE_NOTBMP;
    }
    

	len= (int) fread(buffer,sizeof(PAPITBMP),1,fp);
    if (!len){
        
        return WRAPPER_FILE_BMPPROB;
    }

	bmpInfo=(PAPITBMP *) buffer;
	/*letak ayat dlm bitmap*/
    fseek(fp,bmpInfo->offset,SEEK_SET);
	
	fread(temp,sizeof(char),9,fp);
    decode(byteArray,9,temp);
    
    
    if (  header[0]!=byteArray[0]  )
		return WRAPPER_FILE_NOTSOROK;

	fread(temp,1,4*9,fp);
    decode(number,4*9,temp);
    szdata= (int *) &number[0]; /*saiz data*/
    len = *szdata;
	

	fread(temp,sizeof(char),len*9,fp);
    decode(byteArray,len,temp);

	free(temp);
	//free(number);

	/*ok bois, klinap lekas...*/
	fclose(fp);
	

	*length=len; /*return values*/
    

	return 0;
}

char *wrapper_error(int error)
{
	char *errorstr[]={
		"Masalah membaca fail",
		"Bukan fail bitmap (*.bmp)",
		"Masalah bacaan fail bitmap",
		"Bukan fail bmp \'sorok\'"
	};

	error-=0xf000;
	
	return errorstr[error];
}