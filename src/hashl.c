#include "hashl.h"
#include "globals.h"

/**
 * @brief Add structure to the hashtable
 *        Key is the sequence number
 */
void add_entry(router_entry *r){
    HASH_ADD_INT( globals.rentry, given_pattern, r);
}

/**
 * @brief Find a structure based on the key
 *        which is a sequence number
 */
router_entry *find_entry(uint32_t given_pattern){
    router_entry *r;
    HASH_FIND_INT( globals.rentry, &given_pattern, r);
    return r;
}
