#include <stdio.h>
int main (int argc, char *argv[])
{
    int index = 2;
    int i,j,k;
    if (argc < 3) {
        return 0;
    }
    char *match_pattern = argv[1];
    while (index < argc) {
        char *str = argv[index];
        for (j=0; str[j] != '\0';j++) { 
            for (i=0, k=j; match_pattern[i] != '\0' || str[k] != '\0'; 
                 i++,k++) {
                if (match_pattern[i] != str[k])
                    break;
            }
            if (match_pattern[i] == '\0') {
                printf("%s\n",argv[index]);
                break;
            }
        }
        index++;
    }
}
