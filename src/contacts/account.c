#include "account.h"
#include <malloc.h>


/*********************************PRIVATE**************************************/
void _setMember(dict *dt, char *name, char *phone) {
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
    destroySds(&(act->password));
    destroyDict(&(act->contacts));
    traverseDict(act->groups, destroyDict);
    destroyDict(&(act->groups));
}

void setAccountPassword(account *act, char *password) {
    setSds(act->password, password);
}

void setAccountContacts(account *act, char *name, char *phone) {
    _setMember(act->contacts, name, phone);
}

void setAccountGroupsMember(account *act, char *group, char *name,
                            char *phone) {
    dict *dt = getDictVal(act->groups, group);
    if (dt) {
        _setMember(dt, name, phone);
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
    destroyDict(&dt);
}

// dirty, don't touch
void getAccountAll(account *act, sds *s) {
    sdsCatStr(s, "{\"contacts\":{");
    uint32_t size = act->contacts->size;
    list *li = NULL;
    listNode *ln = NULL;
    for (uint32_t i = 0; i < size; i++) {
        li = act->contacts->table[i];
        ln = li->head;
        while (ln) {
            if (ln->key) {
                sdsCatStr(s, "\"");
                sdsCatStr(s, ln->key->str);
                sdsCatStr(s, "\":");
                sdsCatStr(s, getSdsStr(ln->key));
                sdsCatStr(s, "\",");
            }
        }
    }
    sdsReduceStr(s, ",");
    sdsCatStr(s, "},\"groups\":{");
    size = act->groups->size;
    li = NULL;
    ln = NULL;
    dict *dt = NULL;
    for (uint32_t i = 0; i < size; i++) {
        li = act->groups->table[i];
        ln = li->head;
        while (ln) {
            if (ln->key) {
                sdsCatStr(s, "\"");
                sdsCatSds(s, ln->key);
                sdsCatStr(s, "\":{");
                dt = ln->val;
                for (uint32_t j = 0; j < dt->size; j++) {
                    list *liPtr = dt->table[j];
                    listNode *lnPtr = liPtr->head;
                    while (lnPtr) {
                        if (lnPtr->key) {
                            sdsCatStr(s, "\"");
                            sdsCatStr(s, lnPtr->key->str);
                            sdsCatStr(s, "\":");
                            sdsCatStr(s, getSdsStr(lnPtr->key));
                            sdsCatStr(s, "\",");
                        }
                    }
                }
                sdsReduceStr(s, ",");
                sdsCatStr(s, "},");
            }
        }
    }
    sdsReduceStr(s, ",");
    sdsCatStr(s, "}}");
}

int checkAccountPassword(account *act, char *password) {
    return !sdsCompareStr(act->password, password);
}