#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define KEY 16

int main(int argc, char** argv) {
    
    FILE * file = fopen("test.txt", "w");

    if (file == NULL) {
        printf("erreur dans l'ouverture du fichier");
        return -1;
    }

    for (int i = 1523998909 - 60 * 60 * 3; i <= 1523998909 + 60 * 60 * 3; ++i) {
        srand(i);
        char key[KEY];
        for (int j = 0; j < KEY; ++j)
        {
            key[j] = rand() % 256;
            fprintf(file,"%.2x", (unsigned char)key[j]);
            printf("%.2x", (unsigned char)key[j]);
        }
        fprintf(file,"\n");
        printf("\n");
    }

    fclose(file);
	
    /*struct tm t;
    t.tm_year = 2018 - 1900;
    t.tm_mon = 4 - 1;    
    t.tm_mday = 17;         
    t.tm_hour = 22;       
    t.tm_min = 1;       
    t.tm_sec = 49;       
    t.tm_isdst = -1;

    long long unix_time = mktime(&t);

    if (unix_time != -1) {
        printf("Le temps est: %lld\n",unix_time);
    }
    else {
        printf("Erreur lors de la conversion.\n");
    }*/

	/*int i;
	char key[KEY];
	printf("%lld\n", (long long)time(NULL));
	srand(1);

	for (i = 0; i < KEY; i++)
	{
		key[i] = rand() % 256;
		printf("%.2x", (unsigned char)key[i]);
	}
	printf("\n");

	return 0;*/
}
// e32c483bfd923848805356082eb9172e
// d06bf9d0dab8e8ef880660d2af65aa82
// 95fa2030e73ed3f8