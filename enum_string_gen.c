#include <stdio.h>

#define FOREACH_FRUIT(FRUIT) \
    FRUIT(apple) \
    FRUIT(orange) \
    FRUIT(banana)

#define GENERATE_ENUM(ENUM) ENUM,

#define GENERATE_STRING(STRING) #STRING,


enum FRUIT_ENUM {
    FOREACH_FRUIT(GENERATE_ENUM)
};


//#define GENERATE_STRING(STRING) #STRING,

static const char *FRUIT_STRING[] = {
    FOREACH_FRUIT(GENERATE_STRING)
};

char *get_string(FRUIT_ENUM val) {
    return (#val)

int main ()
{
    printf("enum apple as a string: %s\n",FRUIT_STRING[1]);
}
