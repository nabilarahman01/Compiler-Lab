#include<stdio.h>

int main()
{
    FILE *p1,*p2;
    char c1,c2 = '\0';
    int count=0;

    p1 = fopen("input.c", "r");
    p2 = fopen("output.txt","w");

    if(!p1)
        printf("File can't be opened!");

    else
    {
        while((c1 = fgetc(p1)) != EOF)
        {
            if(c1 == '/' && (c2 == '\n' || c2 == '\t')){
                c2=c1;
                //c1=fgetc(p1);
                continue;
            }
             else if(c1 == '/' && c2 == '/')
             {
                while((c1 = fgetc(p1))!= '\n' && c1 != EOF )
                {
                    continue;
                }
             }
             else if(c1 == '*' && c2 == '/')
             {
                while((c1 =fgetc(p1)) != '/' && c2 != '*')
                {
                    c2=c1;
                }
             }
             else if(c1 == ' ' || c1 == '\t')
             {
                 if(!count)
                 {
                     fputc(' ',p2);
                     count=1;
                 }
             }
             else if(c1 != '\n')
             {
                 fputc(c1, p2);
                 count=0;
             }
             c2 = c1;
        }
    }

    fclose(p1);
    fclose(p2);

    p1= fopen("input.c", "r");
    printf("input: \n");

    while((c1 = fgetc(p1)) != EOF)
    {
        printf("%c", c1);
    }

    fclose(p1);

    p2 = fopen("output.txt", "r");
    printf("\nOutput: \n");

    while((c1 = fgetc(p2)) != EOF)
    {
        printf("%c", c1);
    }

    fclose(p2);


    return 0;
}

