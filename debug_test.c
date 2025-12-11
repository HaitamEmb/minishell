#include "minishell.h"
#include <stdio.h>

int main(void)
{
    t_data data;
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    
    init_data(&data, envp);
    data.user_input = ft_strdup("ls");
    
    printf("Input: %s\n", data.user_input);
    
    if (create_token(data.user_input, &data) != SUCCESS)
    {
        printf("Token creation failed\n");
        return 1;
    }
    
    printf("Tokens created\n");
    t_token *t = data.token;
    while (t)
    {
        printf("Token: type=%d str=%s\n", t->type, t->str ? t->str : "NULL");
        t = t->next;
    }
    
    return 0;
}
