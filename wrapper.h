
#define WRAPPER_SOROK_WRITE 32788
#define WRAPPER_SOROK_BUFFER 1179728

#define WRAPPER_FILE_NOTEXIST 0xf000;
#define WRAPPER_FILE_NOTBMP   0xf001;
#define WRAPPER_FILE_BMPPROB 0xf002;
#define WRAPPER_FILE_NOTSOROK 0xf003;


int wrapper_sorok(const char *filename,char *byteArray, int length);
int wrapper_unsorok(const char *filename, char *byteArray, int *length);
char *wrapper_error(int error);
