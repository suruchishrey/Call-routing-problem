#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>

#define AMOUNT 10
#define ORDER 5

typedef struct object_tag{
    char mobno[11];
}mobilephone;

typedef struct Myset_tag{
    mobilephone set[AMOUNT];
    int n;
}mobilephone_set;

typedef struct TreeNode_tag{
    mobilephone_set* setptr;
    int id[ORDER];                                                                      //should be ORDER-1
    bool leaf;
    int n;
    struct TreeNode_tag*child_ptr[ORDER];
    struct TreeNode_tag*left;
    struct TreeNode_tag*right;
}TreeNode;

typedef struct Tree_tag{
    TreeNode*root;
}Tree;

TreeNode*MakeNode(int a)                                                             //a=1 for index and a=0 for leaf
{
    TreeNode*newptr=(TreeNode*)malloc(sizeof(TreeNode));
    if(a==0)
    {
        newptr->leaf=true;
    }
    else{
        newptr->leaf=false;
    }
    
    newptr->n=0;
    newptr->left=NULL;
    newptr->right=NULL;
    int i;
    for(i=0;i<ORDER;++i)
    {
        newptr->child_ptr[i]=NULL;
    }
    newptr->setptr=NULL;
    printf("HEREEE\n");
    return newptr;
}

bool IsEmpty(mobilephone_set*p)
{
    if(p->n==0)
    {
        return true;
    }
    return false;
}

bool IsMember(mobilephone_set*p,mobilephone o)
{
    int i=0;
    for(i=0;i<p->n;++i)
    {
        if(strcmp(p->set[i].mobno,o.mobno)==0)
        {
            return true;
        }
    }
    return false;
}

void insert_mem(mobilephone o,mobilephone_set*p)
{
    if(p==NULL)
    {
        p=(mobilephone_set*)malloc(sizeof(mobilephone_set));
        p->n=0;
    }
    if(p->n<AMOUNT)
    {
        strcpy(p->set[p->n].mobno,o.mobno);
        p->n++;
    }
    
}

void delete_mem(mobilephone o,mobilephone_set*p)
{
    int j=0,i;
    if(p->n>0)
    {
        for(i=0;i<p->n;++i)
        {
            if(strcmp(p->set[i].mobno,o.mobno)==0)
            {
                for(j=0;p->set[i].mobno[j]!='\0';++j)
                {
                    p->set[i].mobno[j]='\0';
                }
                p->n--;
            }
        }
    }
}

TreeNode* initialize()
{
    TreeNode*new=MakeNode(0);
    printf("HEREEE\n");
}

bool overflows(TreeNode*p)
{
    if(p->n>(ORDER-1))
    {
        printf("\nIn Overflows n=%d\n",p->n);
        return true;
    }
    return false;
}

TreeNode* splitnode(TreeNode*p)                                                               //returns address of new node formed after splitting p
{
    TreeNode*np1,*np2,*retval;
    int i,j=0;
    if(p->leaf==true)
    {
        np1=MakeNode(0);
        for(i=(ORDER-1)/2;i<ORDER;++i)
        {
            np1->id[j++]=p->id[i];
            printf("\nhere in splitnode frst node->id[i]= %d and n=%d",np1->id[j-1],np1->n);
            p->id[i]=-1;
            p->n--;
            printf("\nfirst node =%d",p->id[j-1]);
            np1->n++;
        }
        //p->n--;
        p->right=np1;
        np1->left=p;
        
    }
    else
    {
        j=0;
        np1=MakeNode(1);
        for(i=(ORDER-1)/2+1;i<ORDER;++i)
        {
            np1->id[j++]=p->id[i];
            printf("\nhere in ELSE splitnode frst node->id[i]= %d and n=%d",np1->id[j-1],np1->n);
            np1->child_ptr[j-1]=p->child_ptr[i];
            p->id[i]=-1;
            p->n--;
            np1->n++;
        }
        np1->child_ptr[np1->n]=p->child_ptr[i];
    }
    retval=np1;
    return retval;
}

void sort(int *p, int n)  
{  
    int i, j, temp;  
    for (i = 0; i < n; i++)  
    {  
        for (j = i; j <= n-1; j++)  
        {  
            if (p[i] > p[j])  
            {  
                temp = p[i];  
                p[i] = p[j];  
                p[j] = temp;  
            }  
        }  
    }  
} 

TreeNode* Insert(Tree*tree,int key,int h)                               //initially h=1 as argument
{
    TreeNode*x=tree->root,*y=NULL,*returned=NULL;
    Tree t;Tree*tptr=&t;
    tptr->root=x;
    int i,flag;
    if(x==NULL)
    {
        tree->root=initialize();
        x=tree->root;
        printf("\nHEREEE in NULL");
    }
    if(x->leaf==false)
    {
        flag=0;
        printf("\nhere in while leaf==false node->n= %d",x->n);
            for(i=0;i<x->n && !flag;++i)
            {
                if(key<x->id[i])
                {
                    flag=1;
                } 
            }
            
            if(flag)i--;
              
            //h++;
            printf("\nhere in while leaf==false childnode->n= %d",x->child_ptr[i]->n);
            tptr->root=x->child_ptr[i];
            returned=Insert(tptr,key,++h);
            if(returned!=NULL){printf("\nReturned is not null");
            printf("\nAfter returned node->n = %d and returned node->n= %d",x->n,returned->n);}
    }
    if(x->leaf==true)
    {
        x->id[x->n] = key;                                                  //INSERTION
        x->n++;                                             
        sort(x->id, x->n);
        printf("\ninsertde value= %d", x->id[x->n-1]); 
        printf("HEREEE im imsert\n");
    }
    else if(returned!=NULL){
        printf("\nelse part");
        if(x->child_ptr[0]->leaf==true)
        {
            x->id[x->n]=returned->id[0];
            printf("\nelse part x->id[x->n=%d",x->id[x->n]);
        }
        else{
            x->id[x->n]=x->child_ptr[x->n-1]->id[x->n-1];
            x->child_ptr[x->n-1]->id[x->n-1]=-1;
            //TreeNode*newroot=MakeNode(1);
        }
        x->n++;
        //if(x->n<ORDER)
        x->child_ptr[x->n]=returned;
    }

    if(overflows(x))
    {
        y=splitnode(x);
        
        if(x->leaf==true && h==1)
        {
            printf("HEREEE im imsert in not overflow\n");
            TreeNode*newroot=MakeNode(1);
            newroot->child_ptr[newroot->n]=x;
            newroot->child_ptr[(newroot->n)+1]=y;
            newroot->id[newroot->n]=y->id[0];
            newroot->n++;
            y=newroot;
            tree->root=newroot;
            tree->root->leaf=false;
            //printf(" %d ",y->id[0]);
        }
        else if(x->leaf==false){
            printf("HEREEE im imsert in leaf=false not overflow\n");
            TreeNode*newroot=MakeNode(1);
            newroot->child_ptr[newroot->n]=x;
            newroot->child_ptr[(newroot->n)+1]=y;
            newroot->id[newroot->n]=x->id[x->n-1];
            newroot->n++;
            x->id[x->n-1]=-1;
            x->n--;
            tree->root=newroot;
            tree->root->leaf=false;
        }
    }
    else{
        y=NULL;
    }
    return y;
}

void traverse(TreeNode *p)  
{  
    printf("\n");  
    int i;  
    for (i = 0; i < p->n; i++)  
    {  
        //printf(" %d ",p->n);
        if (p->leaf == false)  
        {  
            traverse(p->child_ptr[i]);  
        }  
        printf(" %d",p->id[i]);  
    }   
    if (p->leaf == false)  
    {  
        traverse(p->child_ptr[i]);  
    }  
    printf("\n");  
}

void MakeTree(Tree*t)
{
    Insert(t,1,1);
    Insert(t,2,1);
    Insert(t,3,1);
    Insert(t,4,1);
    Insert(t,5,1);
    traverse(t->root);
   // if(x!=NULL)t=x;
    Insert(t,6,1);
    //if(x!=NULL)root=x;
    traverse(t->root);
    Insert(t,7,1);
    traverse(t->root);
    Insert(t,8,1);
    traverse(t->root);
    Insert(t,9,1);
    traverse(t->root);
    Insert(t,10,1);
    traverse(t->root);
    Insert(t,11,1);
    traverse(t->root);
    Insert(t,12,1);
    traverse(t->root);
    Insert(t,13,1);
    traverse(t->root);
    printf("\nroot->data=%d n=%d",t->root->id[0],t->root->n);

}

int main()
{
    TreeNode*x;
    TreeNode*root;
    root=MakeNode(0);
    Tree T;
    Tree*t=&T;
    t->root=root;
    MakeTree(t);
    return 0;
}