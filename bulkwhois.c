#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct input
{
    char *url;
    struct input *next;
}input;


// function prototypes
char* concat(const char *s1, const char *s2);
char *strdup(const char *str);
void unloadlist(input *list);


int main(int argc, char *argv[]) {

    // test for correct number of args
    if(argc < 2){
        printf("usage: ./bulkwhois <textfile>\n");
        printf("ensure textfile is plain text with one url per line and no empty lines after last entry\n");
        return 1;
    }
    // open text file containing URLS for reading.
    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL){
        printf("error opening file: %s\n", argv[1]);
        return 1;
    }

    // create a linked list of the URLs to be parsed by whois
    input *node = malloc(sizeof *node);
    memset(node, 0, sizeof *node);
    input *list;
    list = node;
    char *currword = malloc(10000);
    while(fscanf(input_file, "%s", currword) != EOF){
        node->url = strdup(currword);
        input *current = malloc(sizeof *current);
        memset(current, 0, sizeof *current);
        node->next = current;
        node = current;
    }
    free(currword);
    fclose(input_file);
// execute the Whois command on each element of the list of URLs
    input *url_list = list;
    printf("Bulk_Whois v0.1 - Generating text files\n");
    for(int i = 0; i < 40; i++){
        printf("-");
    }
    printf("\n");
    while(url_list->url != NULL) {
        char *whoisarg = concat("whois ", url_list->url);
        char *outputfile = concat(url_list->url, ".txt");
        FILE *whois = popen(whoisarg, "r");
        FILE *output = fopen(outputfile, "w");
        for (int c = getc(whois); c != EOF; c = fgetc(whois)) {
            fputc(c, output);
        }
        printf("processed: %s\n", url_list->url);
        url_list = url_list->next;
        pclose(whois);
        fclose(output);
        free(whoisarg);
        free(outputfile);
    }
    for(int i = 0; i < 40; i++){
        printf("-");
    }
    printf("\nBulk_Whois completed.\nSee current directory for output files.\n");
    for(int i = 0; i < 40; i++){
        printf("-");
    }
    printf("\n");
    unloadlist(list);

    return 0;
}
/* END MAIN FUNCTION */
// function to concatenate two strings
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    if(result == NULL){
        printf("error creating concatenated string\n");
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
// function to duplicate strings
char *strdup(const char *str){
    char *newstr = (char *) malloc(strlen(str) + 1);
    if(newstr){
        strcpy( newstr, str);
    }
    return newstr;
}
// function to free memory from linked list
void unloadlist(input *list){
    while(list->url != NULL) {
        input *tmp = list->next;
        free(list->url);
        free(list);
        list = tmp;
    }
    free(list);
}
