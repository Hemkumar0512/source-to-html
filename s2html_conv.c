
#include <stdio.h>
#include "s2html_event.h"
#include "s2html_conv.h"

static int line_numbering_enabled = 0;
static int line_number = 1;
static int at_line_start = 1;

/* print str to fp, converting HTML special characters to entities */
static void write_escaped(FILE *fp, const char *str)
{
	while (*str)
	{
		if (line_numbering_enabled && at_line_start)
		{
			fprintf(fp, "<span style=\"color:gray;\">%4d </span>", line_number++);
			at_line_start = 0;
		}
		switch (*str)
		{
			case '<': fputs("&lt;", fp);  break;
			case '>': fputs("&gt;", fp);  break;
			case '&': fputs("&amp;", fp); break;
			default : fputc(*str, fp);    break;
		}
		if (*str == '\n')
		{
			at_line_start = 1;
		}
		str++;
	}
}

void set_line_numbering(int enabled)
{
    line_numbering_enabled = enabled;
    line_number = 1;
    at_line_start = 1;
}

/* start_or_end_conv function definitation */
void html_begin(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML begining tags into destination file */
	fprintf(dest_fp, "<!DOCTYPE html>\n");
	fprintf(dest_fp, "<html lang=\"en-US\">\n");
	fprintf(dest_fp, "<head>\n");
	fprintf(dest_fp, "<title>%s</title>\n", "sode2html");
	fprintf(dest_fp, "<meta charset=\"UTF-8\">\n");
	fprintf(dest_fp, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(dest_fp, "</head>\n");
	fprintf(dest_fp, "<body style=\"background-color:lightgrey;\">\n");
	fprintf(dest_fp, "<pre>\n");
}
void html_end(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML closing tags into destination file */
	fprintf(dest_fp, "</pre>\n");
	fprintf(dest_fp, "</body>\n");
	fprintf(dest_fp, "</html>\n");
}



/* sourc_to_html function definitation */
void source_to_html(FILE* fp, pevent_t *event)
{
#ifdef DEBUG
	printf("%s", event->data);
#endif
	switch(event->type)
	{
		case PEVENT_PREPROCESSOR_DIRECTIVE:
			fprintf(fp, "<span class=\"preprocess_dir\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		case PEVENT_MULTI_LINE_COMMENT:
		case PEVENT_SINGLE_LINE_COMMENT:
			fprintf(fp, "<span class=\"comment\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		case PEVENT_STRING:
			fprintf(fp, "<span class=\"string\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		case PEVENT_HEADER_FILE:
			fprintf(fp, "<span class=\"header_file\">");
			if(event->property == USER_HEADER_FILE)
			{
				write_escaped(fp, event->data);
			}
			else
			{
				fprintf(fp, "&lt;");
				write_escaped(fp, event->data);
				fprintf(fp, "&gt;");
			}
			fprintf(fp, "</span>");
			break;
		case PEVENT_REGULAR_EXP:
		case PEVENT_EOF :
			write_escaped(fp, event->data);
			break;
		case PEVENT_NUMERIC_CONSTANT:
			fprintf(fp, "<span class=\"numeric_constant\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		case PEVENT_RESERVE_KEYWORD:
			if(event->property == RES_KEYWORD_DATA)
				fprintf(fp, "<span class=\"reserved_key1\">");
			else
				fprintf(fp, "<span class=\"reserved_key2\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		case PEVENT_ASCII_CHAR:
			fprintf(fp, "<span class=\"ascii_char\">");
			write_escaped(fp, event->data);
			fprintf(fp, "</span>");
			break;
		default :
			printf("Unknow event\n");
			break;
	}
}