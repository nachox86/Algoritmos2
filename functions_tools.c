#define RES_OK 0
#define RES_ERROR 1
#define RES_HELP 2
#define RES_NOT_ENOUGH_ARGS 3
#define RES_WRONG_ARGS 4
#define RES_CORRECT_ARGS 5

#define HELP1 -h
#define HELP2 --help
#define INPUT_FILE -i
#define LOG_FILE -l
#define OUPUT_FILE -o

#define TWO 2
#define SEVEN 7

void showHelp()
{
    print("Este programa permite obtener en formato html, la documentación provista de un archivo de texto.\n");
    print("Uso:\n");
    print("C:\>Documentador.exe -i archivo_a_documentar.c -l archivo_log.txt -o archivo_documentado.html\n");
    print("C:\>Documentador.exe -h\n");
    print("C:\>Documentador.exe --help\n");
    print("Descripción de los parámetros:\n");
    print("-i: indica el nombre del archivo de entrada del que se toma la documentación.\n");
    print("-l: indica el nombre del archivo de loggeo de eventos.\n");
    print("-o: indica el nombre del archivo de salida de la documentación en formato html.\n");
    print("-h o --help: muestra esta ayuda.\n");
}

int validateInput(int carg, char *varg[])
{
    if(carg!=TWO && carg!= SEVEN)
    {
        showHelp();
        return RES_NOT_ENOUGH_ARGS;
    }
    else
    {
        if(carg==TWO)
        {
            if(strcmp(varg[1],HELP1)==0||strcmp(varg[1],HELP2)==0)
            {
                showHelp();
                return RES_HELP;
            }
            else
                return RES_WRONG_ARGS;
        }
        else if(carg==SEVEN)
        {
            if((strcmp(varg[1],INPUT_FILE)==0&&strcmp(varg[3],LOG_FILE)==0&&strcmp(varg[5],OUTPUT_FILE)==0)||
               (strcmp(varg[1],INPUT_FILE)==0&&strcmp(varg[3],OUTPUT_FILE)==0&&strcmp(varg[5],LOG_FILE)==0)||
               (strcmp(varg[1],LOG_FILE)==0&&strcmp(varg[3],INPUT_FILE)==0&&strcmp(varg[5],OUTPUT_FILE)==0)||
               (strcmp(varg[1],LOG_FILE)==0&&strcmp(varg[3],OUTPUT_FILE)==0&&strcmp(varg[5],INPUT_FILE)==0)||
               (strcmp(varg[1],OUTPUT_FILE)==0&&strcmp(varg[3],INPUT_FILE)==0&&strcmp(varg[5],LOG_FILE)==0)||
               (strcmp(varg[1],OUTPUT_FILE)==0&&strcmp(varg[3],LOG_FILE)==0&&strcmp(varg[5],INPUT_FILE)==0)||)
               {
                   return RES_CORRECT_ARGS;
               }
               else
                return RES_WRONG_ARGS;
        }

    }
}
