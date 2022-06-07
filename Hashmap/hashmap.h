#define TABLE_SIZE 20000

typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

typedef struct {
    entry_t **entries;
} ht_t;


void ht_free(ht_t *hashtable);
void ht_dump(ht_t *hashtable);
void ht_del(ht_t *hashtable, const char *key);
char *ht_get(ht_t *hashtable, const char *key);
void ht_set(ht_t *hashtable, const char *key, const char *value);
ht_t *ht_create(void);
entry_t *ht_pair(const char *key, const char *value);
unsigned int hash(const char *key);
char *ht_getFirstNull(ht_t *hashtable);
unsigned int ht_getSlotOfKey(ht_t *hashtable, const char *key);
char *ht_getValueFromSlot(ht_t *hashtable, unsigned int slot);
char *ht_getKeyFromSlot(ht_t *hashtable, unsigned int slot);