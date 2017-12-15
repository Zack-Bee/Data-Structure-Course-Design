.RECIPEPREFIX = >
test : test.c
> gcc test.c; ./a.out; rm ./a.out;

linkedlistTest : linkedlist.c linkedlist.h
> gcc linkedlist.c -o linkedlistTest; ./linkedlistTest; rm ./linkedlistTest;

sdsTest : sds.c sds.h
> gcc sds.c -o sdsTest; ./sdsTest; rm ./sdsTest;

dictTest : dict.c dict.h
> gcc dict.c -o dictTest; ./dictTest; rm ./dictTest;