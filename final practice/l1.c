#include<stdio.h>

int main()
{
    FILE *p1, *p2;
    char c;

    p1= fopen("l1.c", "r");
    p2= fopen("par1.txt", "w");

    if(!p1)
    {
        printf("file nai");
    }
    else
    {
        while((c=fgetc(p1))!=EOF)
        {
            if((c=='(') || (c==')'))
            {
                fputc(c, p2);
            }
        }
    }

    fclose(p1);

    while((c=fgetc(p2))!=EOF){
        printf("%c",c);
    }
    fclose(p2);

    return 0;
}
