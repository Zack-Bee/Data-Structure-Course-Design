#include "account.h"


/*********************************PRIVATE**************************************/
_setMember(dict dt, char *name, char *phone) {
    sds *s = newCopySds(phone);
    setDictEntry(dt, name, phone);
}

/**********************************PUBLIC**************************************/
account *newAccount() {
    account *act = NULL;
    act = malloc(sizeof(account));
    act->password = newSds();
    act->contacts = newDict();
    act->groups = newDict();
    act->destroy = destroyAccount;
}

void destroyAccount(account *act) {
    destroySds(act->password);
    destroyDict(act->contacts);
    traverseDict(act->groups, destroyDict);
    destroyDict(act->groups);
}

void setAccountPassword(account *act, char *password) {
    setSds(act->password, password);
}

void setAccountContacts(account *act, char *name, char *phone) {
    _setMember(act->contacts, name, phone);
}

void setAccountGroupsMember(account *act, char *group, char *name, char *phone) {
    dict *dt = getDictVal(act->groups, group);
    if (dt) {
        _setMember()
    } else {
        dt = newDict();
        setDictEntry(act->groups, group, dt);
        _setMember(dt, name, phone);
    }
}

void delAccountContacts(account *act, char *name) {
    delDictEntry(act->contacts, name);
}

void delAccountGroupsMember(account *act, char *group, char *name) {
    dict *dt = getDictVal(act->groups, group);
    if (dt) {
        delDictEntry(dt, name);
    }
}

void delAccountGroups(account *act, char *group) {
    dict *dt = getDictVal(act->groups, group);
    destroyDict(dt);
}


#define ACCOUNT_TEST
#ifdef ACCOUNT_TEST

#endif