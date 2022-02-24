/*
This software is under PPSplit Public License 1.1
Copyright (c) 2000-2004 Mypapit. All rights reserved.


This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim 
that you wrote the original software. If you use this software in a product, 
an acknowledgment in the product documentation would be appreciated but is not 
required.

2. Altered source versions must be plainly marked as such, and must not be 
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.

4. Redistributions of source code must with or without modifications retain the 
above copyright notice, this list of conditions and the following disclaimer.

5. Redistributions in binary form with or without modifications must reproduce the 
above copyright notice, this list of conditions and the following disclaimer in the 
documentation and other materials provided with the distribution. 


THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS

INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For latest version of this software, please visit :

http://ppsplit.sourceforge.net
http://papit.cjb.net

authors email : papit58@yahoo.com, papit01@lycos.com

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorok.h"

#define CHARBUF 384960
#define BMPBUF  768900

int decode(char *buffer, int len, const char *bitmap);

int main(int argc, char *argv[])
{

    PAPITBMP *bmpInfo;
    int len,szmesej;
    char *buffer;
    unsigned char temp[BMPBUF];
    char ayat[CHARBUF];
    char number[4];
    int *szdata;
    const char header[]={0x02};
    FILE *fin;
    
    
    fprintf(stderr,"unsorok 0.05 - steganografi tatkala bosan.\nCopyright (c) papit 2004, website : http://papit.cjb.net\n\n");
    fprintf(stderr,"AMARAN : Perisian prototaip, jangan sorok bukan-bukan !\n\n");

    buffer=(char *) malloc(sizeof(PAPITBMP));
  
    if (argc<2){
        printf("usage : unsorok filename\n");
        return 0xff;
    }
    
    fin = fopen(argv[1],"r+b");
    if (!fin){
        printf("ralat : file tidak dapat dibuka\n");
        return 0xf8;
    }

    fread(buffer,sizeof(char),2,fin);
    if (buffer[0]!='B' && buffer[1]!='M'){
        printf("ralat : ini bukan file bitmap!\n");
        return 0xf0;
    }
    
    len=fread(buffer,sizeof(PAPITBMP),1,fin);
    
    if (!len){
        printf("ralat : masalah baca file bitmap\n");
        return 0xff;
    }
    
    bmpInfo=(PAPITBMP *) buffer;
    printf("size : %d\n",bmpInfo->size);
    printf("Width : %d\t",bmpInfo->width);
    printf("Height : %d\n",bmpInfo->height);
    printf("Bits : %hd\n",bmpInfo->bpp);
    printf("Data offset : %d (hex: %xh)\n",bmpInfo->offset,bmpInfo->offset);
    printf("\nmaklumat bitmap dipaparkan, tekan [enter]...\n");
    
    getc(stdin);
    
 
    
    fseek(fin,bmpInfo->offset,SEEK_SET);
    fread(temp,sizeof(char),9,fin);
    decode(ayat,9,temp);
    
    
    if (  header[0]!=ayat[0]  ){
        printf("ralat : bukan file \'sorok\', id : %hd (%hx Hex)\n", ayat[0], ayat[0]);
        return 0xf6;
    }
    
    fread(temp,1,4*9,fin);
    decode(number,4*9,temp);
    szdata= (int *) &number[0]; /*saiz data*/
    szmesej = *szdata;
            
    len=fread(temp,sizeof(char),szmesej*9,fin);
    decode(ayat,szmesej,temp);
    printf("saiz ayat : %d\n",szmesej);
    printf("ayat adalah :\n\t%s\n",ayat);
       
    fclose(fin);
    return 0;
}



int decode(char *buffer, int len, const char *bitmap)
{
    unsigned char huruf;
    int i,j;
     
     j=0;
    /*decode*/
    for (i=0;i<len;i++) {
        huruf=0x00; /*reset huruf*/
        huruf|=((bitmap[j+0]&0x01)<<7);
        huruf|=((bitmap[j+1]&0x01)<<6);
        huruf|=((bitmap[j+2]&0x01)<<5);
        huruf|=((bitmap[j+3]&0x01)<<4);
        huruf|=((bitmap[j+4]&0x01)<<3);
        huruf|=((bitmap[j+5]&0x01)<<2);
        huruf|=((bitmap[j+6]&0x01)<<1);
        huruf|=((bitmap[j+7]&0x01)<<0);
        
        j+=9;
        
        buffer[i]=huruf;
    }
    
    buffer[len]='\0';


    return 0;
}

