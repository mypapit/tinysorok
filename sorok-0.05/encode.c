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

Changelog :

0.05
- auto-detect size mesej dalam bitmap

0.01 
- first release
- takder encryption
- unsorok perlukan size data sebelum extract mesej
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorok.h"
#define BMPBUF  4608

#define CHARBUF 1024
#define BMPBUF  4608


int encode(const char *buffer,int len,char *bitmap);

int main(int argc, char *argv[])
{

    PAPITBMP *bmpInfo;
    int len,szmesej;
    char *buffer;
    unsigned char temp[BMPBUF];
    static char header[]={0x02}; /*standard 1 byte header */
    char *szdata; /* 4 byte len - compatibility purporse*/
   
    FILE *fin;

    buffer=(char *) malloc(sizeof(PAPITBMP));
    
    fprintf(stderr,"sorok 0.05 - steganografi tatkala bosan.\nCopyright (c) papit 2004, website : http://papit.cjb.net\n\n");
    /*fprintf(stderr,"This program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");*/
    fprintf(stderr,"AMARAN : Perisian prototaip, jangan sorok bukan-bukan !\n\n");
    
    if (argc<3){
        printf("usage : sorok filename \"ayat rahsia\" \n");
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
        printf("ralat : masalah membaca file bitmap\n");
        return 0xf1;
    }

    /*dapatkan maklumat bitmap*/
    bmpInfo=(PAPITBMP *) buffer;
    printf("Size : %d\n",bmpInfo->size);
    printf("Width : %d\t",bmpInfo->width);
    printf("Height : %d\n",bmpInfo->height);
    printf("Bits : %hd\n",bmpInfo->bpp);
    printf("Data offset : %d (hex: %xh)\n",bmpInfo->offset,bmpInfo->offset);

    /*pause sejenak..*/
    printf("\nmaklumat bitmap dipaparkan, tekan [enter]...\n");
    getc(stdin);
    
    szmesej = strlen(argv[2]);

    /*letak ayat dlm bitmap*/
    fseek(fin,bmpInfo->offset,SEEK_SET);
    len=fread(temp,sizeof(char),(szmesej+4+1)*9,fin);
    
    if (len<9){
        printf("ralat : masalah membaca file bitmap\n");
        return 0xf2;
    }
    
    szdata = (char *) &szmesej; /*assign saiz mesej utk di tuliskan*/
    encode(header,1,temp); /*tambah 0x02 header*/
    encode(szdata,4,&temp[9]); /* letak mesej size */
    encode(argv[2],szmesej,&temp[45]); /* baru letak mesej*/
    
    /*
    * overlap data bitmap dgn bitmap yang dah dipinda
    */
    fseek(fin,bmpInfo->offset,SEEK_SET);
    fwrite(temp,sizeof(char),(szmesej+4+1)*9,fin);

    /*ok pack up semua....*/
    fclose(fin);
    free(buffer);
    printf("Saiz mesej : %d\n",szmesej);
    printf("Selesai...\n");
    return 0;
}


/************************************************
function encode  (papit, 28 April 2004)

input :
buffer = ayat/data yang nak diencodekan
len = panjang data yang nak diencode
bitmap = data bitmap yang hendak di steganokan

output :
bitmap = data bitmap yang dah disteganokan
**************************************************/
int encode(const char *buffer,int len,char *bitmap)
{

    
    int i,j;

    
    /*
     * clearkan LSB bagi data bitmap, bagi mudahkan operasi boolean
     * Quick hack by papit : 28 April 2004
     *
     */
    for (i=0;i<len*9;i++)
        bitmap[i]&=bitmap[i] & 0xfe;

    /*
     * Jadikan LSB bagi data bitmap sama dgn bit-bit buffer
     * ikut susunan menaik...
     * LSB byte[9] bagi data bitmap tak digunakan (April 2004)
    */
    j=0;
    for(i=0;i<len;i++){    
        bitmap[j+0]|=  ( (*buffer>>7) & 0x01 );
        bitmap[j+1]|=  ( (*buffer>>6) & 0x01 );
        bitmap[j+2]|=  ( (*buffer>>5) & 0x01 ); 
        bitmap[j+3]|=  ( (*buffer>>4) & 0x01 );
        bitmap[j+4]|=  ( (*buffer>>3) & 0x01 );
        bitmap[j+5]|=  ( (*buffer>>2) & 0x01 );
        bitmap[j+6]|=  ( (*buffer>>1) & 0x01 );
        bitmap[j+7]|=  ( (*buffer>>0) & 0x01 );
        j+=9;
        buffer++;
    }

    return 1;
}
