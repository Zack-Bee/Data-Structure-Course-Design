#include "account.h"

account *newAccount() {
    account *act = NULL;
    act = malloc(sizeof(account));
    act->password = newSds();
    act->contacts = newDict();
    act->groups = newDict();
}

void destroyAccount(account *act) {
    destroySds(act->password);
    destroyDict(act->contacts);
    traverseDict(act->groups, destroyDict);
}

void setAccountPassword(account *act, char *password) {
    setSds(act->password, password);    
}

void setAccountContacts(account *act, char *name, char *phone) {
    
}