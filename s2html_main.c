#include <stdio.h>
#include <string.h>
#include "s2html_event.h"
#include "s2html_conv.h"
#include "s2html_conv.c"
#include "s2html_event.c"


int main(int argc, char *argv[])
{
    FILE *sfp, *dfp;
    pevent_t *event;
    char dest_file[100];
    const char *source_file;
    int show_line_numbers = 0;

    /* Validate argument count */
    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "Usage: %s <source.c> [output.html] [-n]\n", argv[0]);
        return 1;
    }

    source_file = argv[1];

    /* Parse optional output filename and -n flag */
    if (argc == 3)
    {
        if (strcmp(argv[2], "-n") == 0)
        {
            show_line_numbers = 1;
        }
        else
        {
            snprintf(dest_file, sizeof(dest_file), "%s", argv[2]);
        }
    }
    else if (argc == 4)
    {
        if (strcmp(argv[3], "-n") != 0)
        {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[3]);
            return 1;
        }
        show_line_numbers = 1;
        snprintf(dest_file, sizeof(dest_file), "%s", argv[2]);
    }

    /* Generate default output filename when not specified */
    if (argc == 2 || (argc == 3 && strcmp(argv[2], "-n") == 0))
    {
        int written = snprintf(dest_file, sizeof(dest_file), "%s.html", source_file);
        if (written < 0 || written >= sizeof(dest_file))
        {
            fprintf(stderr, "Error: Output filename too long\n");
            return 1;
        }
    }

    /* Prevent direct source/destination path collision */
    if (strcmp(source_file, dest_file) == 0)
    {
        fprintf(stderr, "Error: Source and destination must differ\n");
        return 1;
    }

    /* Open source file */
    sfp = fopen(source_file, "r");
    if (sfp == NULL)
    {
        perror(source_file);
        return 2;
    }

    /* Open destination file */
    dfp = fopen(dest_file, "w");
    if (dfp == NULL)
    {
        perror(dest_file);
        fclose(sfp);
        return 3;
    }

    /* Configure line numbering */
    set_line_numbering(show_line_numbers);

    /* Generate HTML */
    html_begin(dfp, HTML_OPEN);

    do
    {
        event = get_parser_event(sfp);
        if (event == NULL)
        {
            fprintf(stderr, "Error: Parser returned NULL\n");
            fclose(sfp);
            fclose(dfp);
            return 4;
        }
        source_to_html(dfp, event);

    } while (event->type != PEVENT_EOF);

    html_end(dfp, HTML_CLOSE);

    if (fclose(sfp) != 0)
    {
        perror("Error closing source file");
        fclose(dfp);
        return 5;
    }

    if (fclose(dfp) != 0)
    {
        perror("Error closing output file");
        return 5;
    }

    printf("Output file %s generated\n", dest_file);

    return 0;
}
