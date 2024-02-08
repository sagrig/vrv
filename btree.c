#include <stddef.h>
#include <stdint.h>

#include "btree.h"

void add_btn(struct btr *root, struct btn *new)
{
     struct btn **next = NULL;
     uint8_t      val  = new->bn_val;

     if (!root)
	  return;

     next = (val & 1)?
	  (&root->br_ri):
	  (&root->br_le);

     do {
	  if (!(*next)) {
	       *next = new;
	       break;
	  }
	  val >>= 1;

	  next  = (val & 1)?
	       (&(*next)->bn_ri):
	       (&(*next)->bn_le);
     } while (1);
}

struct btn *find_btn(const struct btr *root, const uint8_t val)
{
     struct btn *next = NULL;
     uint8_t     tmp  = val;

     if (!root)
	  return NULL;

     next = (tmp & 1)?
	  (root->br_ri):
	  (root->br_le);

     while (next) {
	  if (next->bn_val == val)
	       return next;
	  tmp >>= 1;

	  next  = (tmp & 1)?
	       (next->bn_ri):
	       (next->bn_le);
     }
     return NULL;
}
