#include "kernel/types.h"
#include "user/user.h"

typedef enum {wrong_char, success_parse, toomany_char} cmd_parse;

// funciton define
cmd_parse parse_cmd(int argc, char** argv);

int main(int argc, char** argv){
    if(argc == 1){
        printf("Please enter the parameters!");
        exit(-1);
    }
    else{
        cmd_parse parse_result;
        parse_result = parse_cmd(argc, argv);
        if(parse_result == toomany_char){
            printf("Too many args! \n");
            exit(-1);
        }
        else if(parse_result == wrong_char){
            printf("Cannot input alphabet, number only \n");
            exit(-1);
        }
        else{
            int duration = atoi(argv[1]);
            sleep(duration);
            exit(0);
        }
        
    }
}

cmd_parse parse_cmd(int argc, char** argv){
    if(argc > 2){
        return toomany_char;
    }
    else {
        // check each if it's number
        int i = 0;
        while (argv[1][i] != '\0')
        {
            if(!('0' <= argv[1][i] && argv[1][i] <= '9')){
                return wrong_char;
            }
            i++;
        }
        
    }
    return success_parse;
}
