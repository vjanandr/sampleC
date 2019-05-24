#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *left;
    struct node *right;
};

struct node*
create_node (int data)
{
    struct node* n = malloc(sizeof(struct node));
    if (n) {
        n->data = data;
        n->left = n->right = NULL;
    }
    return n;
}
int searchindex(int in[], int data, int end)
{
    int i = 0;
    while  (i <= end) {
        if (in[i] == data) 
            return i;
        i++;
    }
}

void printInorder (struct node *n)
{
    if (!n) 
        return;
    printInorder(n->left);
    printf("%c  ",n->data);
    printInorder(n->right);
}
struct node* buildtree (int in[], int pre[], int start, int end)
{
    static int preindex = 0;
    if (start > end) {
        return NULL;
    }

    struct node *n = create_node(pre[preindex++]);
    if (start == end) {
        // this the leaf node
        return n;
    }
    int nodeindex = searchindex(in, n->data, end);
    n->left = buildtree(in, pre, start, nodeindex-1);
    n->right = buildtree(in, pre, nodeindex+1, end);
    return n;
}
int main ()
{
    int in[] = {'D','B','E','A','F','C'};
    int pre[] = {'A','B','D','E','C','F'};
    int len = sizeof(in)/sizeof(in[0]);
    struct node *tree = buildtree(in, pre, 0, len-1);
    printInorder(tree);
}
