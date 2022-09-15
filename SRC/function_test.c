#include <stdio.h>
#include <string.h>
int string_in_string(char check_for[], char search_in[]);

int main() {


    char long_word[] = {""};
    char short_word[] = {""};
    
    int test = strncasecmp(short_word, long_word, strlen(short_word));
    printf("strncasecmp returns: %d \n", test);

    int test_2 = string_in_string(short_word, long_word);
    printf("string_in_string: %d \n", test_2);
    
    if (test_2 == 1) {
        printf("match! \n");
    }
    else {
        printf("no match \n");
    }
}



int string_in_string(char check_for[], char search_in[]) {
    // first loop looks for a matching character, if this does not 
    //exist return 0, if it does continue to next loop
    int search_index = 0;
    while (search_in[search_index] != '\0') {
        if (strncasecmp(check_for, &search_in[search_index], 1) == 0) {
      //  if (check_for[0] == search_in[search_index]) {
            // do string compare on the remainder of search_in
            printf("starting character passed to :%c\n", search_in[search_index]);
            int check = strncasecmp(check_for, 
            &search_in[search_index], strlen(check_for));
            printf("strncasecmp in function output: %d \n", check);
            if (check == 0) {
                return 1;
            }
        }
        search_index++;
       
    }
    return 0;
}
