#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/neutrino.h>


int main()
{
    const char *smsg = "Это буфер вывода";
    char rmsg[200];
    int coid;

    coid = ConnectAttach (0, 1175581, 1, 0, 0);
    if (coid == -1)
    {
        fprintf (stderr, "Ошибка ConnectAttach к 0/7782439/1!\n");
        perror(NULL);
        exit (EXIT_FAILURE);
    }

    if (MsgSend (coid, smsg,
                 strlen (smsg) + 1,
                 rmsg, sizeof (rmsg)) == -1)
    {
        fprintf (stderr, "Ошибка MsgSend\n");
        perror(NULL);
        exit (EXIT_FAILURE);
    }

    if (strlen (rmsg) > 0)
        printf ("Процесс с ID 7782439 возвратил \"%s\"\n", rmsg);

    return 0;
}
