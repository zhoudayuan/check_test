#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

int
main(int argc, char **argv)
{
   int opt;
   int digit_optind = 0;
   int option_index = 0;
   
   // -c==cmd 
   // -t==type
   // -m==model
   // -n==number
   // -l==len
   // -k==clock
   
    
   
   char *optstring = "c:t:m:n:l:k";
   static struct option long_options[] = {
       {"reqarg", required_argument, NULL, 'r'},
       {"noarg",  no_argument,       NULL, 'n'},
       {"optarg", optional_argument, NULL, 'o'},
       {0, 0, 0, 0}
   };

   while ( (opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
   {
        printf("opt = %c\n", opt);
        printf("optarg = %s\n", optarg);
        printf("optind = %d\n", optind);
        printf("argv[optind - 1] = %s\n",  argv[optind - 1]);
        printf("option_index = %d\n\n", option_index);
   }

   return 0;
}