// a test of sscanf

#include <stdio.h>

int count_spaces_till_5(char string[], int index);

int main() {

    char string[] = {"juno 1234.345 23 poison fire hugo 12 34 dragon none me 1 2 fighting poison "};
    
    int i = 0;
    while (string[i] != '\0') {
        char name[1000];
        double height;
        double weight;
        char type1[1000];
        char type2[1000];
          
        sscanf(&string[i], "%s %lf %lf %s %s ", name, &height, &weight, type1, type2);
        printf("name: %s\n", name);
        printf("height: %lf\n", height);
        printf("weight: %lf\n", weight);
        printf("type1: %s\n", type1);
        printf("type2: %s\n", type2); 
        
        i = count_spaces_till_5(string, i);       
    }
        

}
// function will increment along a string until it counts 5 white spaces.
// returns an int of the amount incremented by. for use in divinging load file txt
int count_spaces_till_5(char string[], int index) {
    int counter = 0;
    while (counter < 5) {
        if (string[index] == ' ') {
            counter++;
        }
        index++;
    }
    return index;
}
