#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char str[100];
int i=0;
int len;
int Y()
{
    if(str[i] == 'r')
    {
        i++;
    }

}
int X()
{
    if(str[i] == 'q')
    {
        i += 1;
    }
    else if(str[i]='p')
    {
        i += 1;
        Y();

    }
}


int S()
{
    if(i==0 && str[i] == 'p')
    {
        i+=1;

        X();
        if(str[i] == 'r')
        {
            return 1;
        }
        else
            return 0;
    }
    else if(i==0)
    {

        Y();
        if(str[i] == 's' && str[i+1] == 't')
        {
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}




int main()
{
    gets(str);
    len = strlen(str);

    if(S())
    {
        printf("String is valid.\n");
    }
    else
    {
        printf("String is invalid.\n");
    }
}
