/************************************************
function encode  (papit, 28 April 2004)

input :
buffer = ayat/data yang nak diencodekan
len = panjang data yang nak diencode
bitmap = data bitmap yang hendak di steganokan

output :
bitmap = data bitmap yang dah disteganokan
**************************************************/

#include "sorok.h"

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

    return 1;
}
