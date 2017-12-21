.RECIPEPREFIX = >
test : test.c
> gcc test.c; ./a.out; rm ./a.out;

listTest : src/contacts/list.c src/contacts/list.h
> gcc src/contacts/list.c -o listTest; ./listTest; rm ./listTest;

sdsTest : src/contacts/sds.c src/contacts/sds.h
> gcc src/contacts/sds.c -o sdsTest; ./sdsTest; rm ./sdsTest;

dictTest : src/contacts/dict.c src/contacts/dict.h
> gcc src/contacts/dict.c -o dictTest; ./dictTest; rm ./dictTest;

contactsServer : src/contacts/server.c
> gcc src/contacts/server.c -o server