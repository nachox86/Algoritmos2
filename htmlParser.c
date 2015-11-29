#include "htmlParser.h"

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

int parseStringToHtml(htmlFile *output, t_keyword comment) {

    if (strcmp(KW_TITLE, comment.tag) == 0) {
        writeHtmlTitle(output, comment.name);
    } else if (strcmp(KW_SUBTITLE, comment.tag) == 0) {
        writeHtmlSubtitle(output, comment.name);
    } else if (strcmp(KW_FUNCTION, comment.tag) == 0) {
        writeHtmlFunction(output, comment.name);
    } else if (strcmp(KW_DESCRIPTION, comment.tag) == 0) {
        writeHtmlDescription(output, comment.name);
    } else if (strcmp(KW_AUTHOR, comment.tag) == 0) {
        writeHtmlAuthor(output, comment.name);
    } else if (strcmp(KW_DATE, comment.tag) == 0) {
        writeHtmlDate(output, comment.name);
    } else if (strcmp(KW_VERSION, comment.tag) == 0) {
        writeHtmlVersion(output, comment.name);
    } else if (strcmp(KW_RETURN, comment.tag) == 0) {
        writeHtmlReturn(output, comment.name);
    } else if (strcmp(KW_PRE, comment.tag) == 0) {
        writeHtmlPreconditions(output, comment.name);
    } else if (strcmp(KW_POST, comment.tag) == 0) {
        writeHtmlPostconditions(output, comment.name);
    } else {
        return ERROR;
    }

    return SUCCESS;
}
