typedef struct bstree
{
  char key[8];
  int value;
  struct bstree *left;
  struct bstree *right;
  struct bstree *parent;
} bstree;

bstree *bstree_create (char *key, int value);
int bstree_add (bstree *tree, char *key, int value);
bstree *bstree_lookup (bstree *tree, char *key);
bstree *bstree_min (bstree *tree);
bstree *bstree_max (bstree *tree);
static void _bstree_delete1 (bstree *deltree, char *key);
static void _bstree_delete2 (bstree *deltree, char *key);
static void _bstree_delete3 (bstree *deltree, char *key);
bstree *bstree_delete (bstree *tree, char *key);
void bstree_free (bstree *tree);
void print_tree_as_list (bstree *root);
void showLine (char *c, int p, int s);
void print_tree (bstree *tree, int p, int s);
int bstree_heigth (const bstree *p);