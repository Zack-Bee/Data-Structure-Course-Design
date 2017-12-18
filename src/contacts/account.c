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
}