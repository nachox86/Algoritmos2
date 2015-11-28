#include "htmlParser.h"
#include "straight_list.h"
#include "tda_documentador.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initHtmlFile(char *path) {
    /* Little hack, hope you guys don't mind... */
    FILE *src = fopen("./misc/base.html", "rb");
    FILE *dst = fopen(path, "wb");
    int i;

    if ( (!src) || (!dst))
        return ERROR;

    for (i = getc(src); i != EOF; i = getc(src)) {
        putc(i, dst);
    }
    fclose(dst);
    fclose(src);
    return SUCCESS;
}

int createHtmlFile(htmlFile *output, char *path) {

    output = malloc(sizeof(htmlFile));
    output->init = initHtmlFile(path);
    output->file = fopen(path, "a+");

    if (output->init && output->file)
        return SUCCESS;

    return ERROR;
}

int closeHtmlFile(htmlFile *output) {
	/* Before closing the file, we need to add some more code.. closing tags */
	if (output->init) {
		fprintf(output->file, "</div>\n</body>\n</html>");
		fclose(output->file);
	    free(output);
	    return SUCCESS;
   	}

	return ERROR;
}

int writeHtmlTitle(htmlFile *output, char *title) {
    if (output->init) {
        fprintf(output->file, "<h1>%s</h1>\n", title);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlSubtitle(htmlFile *output, char *subtitle) {
    if (output->init) {
        fprintf(output->file, "<h2>%s</h2>\n", subtitle);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlFunction(htmlFile *output, char *name) {
    if (output->init) {
        fprintf(output->file, "<h3>Funci&oacute;n: <a name=\"%s\">%s</a></h3>\n", name, name);
        return  SUCCESS;
    }
    return ERROR;
}

int writeHtmlDescription(htmlFile *output, char *description) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Descripci&oacute;n</dt>\n<dd>%s</dd>\n</dl>", description);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlAuthor(htmlFile *output, char *author) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Autor</dt>\n<dd>%s</dd>\n</dl>", author);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlDate(htmlFile *output, char *date) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Fecha</dt>\n<dd>%s</dd>\n</dl>", date);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlVersion(htmlFile *output, char *version) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Version</dt>\n<dd>%s</dd>\n</dl>", version);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlParam(htmlFile *output, char *paramName, char *paramInfo) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Par&aacute;metro: %s</dt>\n<dd>%s</dd>\n</dl>", paramName, paramInfo);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlReturn(htmlFile *output, char *returnInfo) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Return</dt>\n<dd>%s</dd>\n</dl>", returnInfo);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlPreconditions(htmlFile *output, char *conditions) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>Pre­Condici&oacute;n</dt>\n<dd>%s</dd>\n</dl>", conditions);
        return SUCCESS;
    }
    return ERROR;
}

int writeHtmlPostconditions(htmlFile *output, char *conditions) {
    if (output->init) {
        fprintf(output->file, "<dl>\n<dt>PostCondici&oacute;n</dt>\n<dd>%s</dd>\n</dl>", conditions);
        return SUCCESS;
    }
    return ERROR;
}

int parseStringToHtml(htmlFile *output, straight_list_t* lista) {

    void* data_aux2;
    t_keyword* data_aux;
    if(!straight_list_is_empty(lista)) return RES_ERROR;

    data_aux2 = malloc(sizeof(t_keyword));
    if(!data_aux2) return RES_ERROR;

    straight_list_move(lista,straight_list_first);

    do{

        straight_list_get(lista,data_aux2);
        printf("estoy en parseStringToHtml\n");

        data_aux = (t_keyword*)data_aux2;

        if (strcmp(KW_TITLE, data_aux->tag) == 0) {
            writeHtmlTitle(output, data_aux->value);
        } else if (strcmp(KW_SUBTITLE, data_aux->tag) == 0) {
            writeHtmlSubtitle(output, data_aux->value);
        } else if (strcmp(KW_FUNCTION, data_aux->tag) == 0) {
            writeHtmlFunction(output, data_aux->value);
        } else if (strcmp(KW_DESCRIPTION, data_aux->tag) == 0) {
            writeHtmlDescription(output, data_aux->value);
        } else if (strcmp(KW_AUTHOR, data_aux->tag) == 0) {
            writeHtmlAuthor(output, data_aux->value);
        } else if (strcmp(KW_DATE, data_aux->tag) == 0) {
            writeHtmlDate(output, data_aux->value);
        } else if (strcmp(KW_VERSION, data_aux->tag) == 0) {
            writeHtmlVersion(output, data_aux->value);
        } else if (strcmp(KW_RETURN, data_aux->tag) == 0) {
            writeHtmlReturn(output, data_aux->value);
        } else if (strcmp(KW_PRE, data_aux->tag) == 0) {
            writeHtmlPreconditions(output, data_aux->value);
        } else if (strcmp(KW_POST, data_aux->tag) == 0) {
            writeHtmlPostconditions(output, data_aux->value);
        }
        free(data_aux->name);
        free(data_aux->tag);
        free(data_aux->value);
    }while(!straight_list_move(lista,straight_list_next));

    free(data_aux2);
    return RES_OK;
}
