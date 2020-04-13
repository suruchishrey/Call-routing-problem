
/*
Assumption- The tree is predefined.I have chosen B+Tree to implement the functions.It is given below.

                         4        7
                      /     \         \
                     /       \         \
                    /         \         \
                   /           \         \
                2   3          5   6      8   9
              /    |   \      /  |  \    /  |  \
             /     |    \    /   |   \  /   |   \
            1 ->   2 ->  3->4 -> 5 -> 6->7->8 -> 9->NULL
    NULL  <-  <-     <-   <-  <-   <-  <- <-  <-  

The central server consisits of 4 as its id.Similarly the nodes (2,3),(5,6),(8,9) are having node id as 2,5,8 respectively.The leaf nodes are the 
base stations having id as 1,2,3,4,5,6,7,8,9 respectively.They are connected as a doubly linked list.They have a set of mobilephone objects.
Every upper level node consists of the data of the nodes below it for e.g., node (2,3) consists of mobilephones present in nodes leaf nodes 1,2,3.
*/
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>

#define LEVEL 3
#define ORDER 3                                                             //define NO_OF_CHILD = 3
#define MOBILES 10

typedef struct MobilePhone_tag{
    char mob_no[11];
}MobilePhone;

typedef struct MobilePhoneSet_tag{
    MobilePhone mobilephones[MOBILES*10];
}MobilePhoneSet;

typedef struct Exchangelist_tag{
    int Exchanges[LEVEL*2];
    int n;
}Exchangelist;

typedef int Exchange;                                               //for residentSet()

typedef struct BPlusTreeNode_tag{
    int*data_id;                                                   //basestation_id or exchange_id respectively for leaf nodes and index nodes
    //int key;
    MobilePhone* mobilephones;
    int num_phones;
    //BaseStation station;
    struct BPlusTreeNode_tag* left;
    struct BPlusTreeNode_tag* right;
    struct  BPlusTreeNode_tag** child_ptr;
    bool leaf;
    int n;
    //int level;
    //char*name;
}TreeNode;

MobilePhone* MakeMobilePhone(char*mobno)
{
    MobilePhone*new=(MobilePhone*)malloc(sizeof(MobilePhone));
    strcpy(new->mob_no,mobno);

    return new;
}

TreeNode* MakeNode(int mode)                                                    //argument 0 for leaf node and 1 for index node
{
    TreeNode*newptr=(TreeNode*)malloc(sizeof(TreeNode));
    newptr->child_ptr=(TreeNode**)malloc(ORDER*sizeof(TreeNode));
    if(mode==0)
    {
        newptr->leaf=true;
        //newptr->level=0;
        newptr->left=NULL;
        newptr->right=NULL;
        newptr->data_id=(int*)malloc(1*sizeof(int));
        newptr->mobilephones=(MobilePhone*)malloc(MOBILES*sizeof(MobilePhone));
        newptr->num_phones=0;
    }
    else if(mode==1)
    {
        newptr->leaf=false;
        newptr->data_id=(int*)malloc((ORDER-1)*sizeof(int));
        newptr->mobilephones=NULL;
        newptr->num_phones=0;
    }
    newptr->left=NULL;
    newptr->right=NULL;
    newptr->n=0;
    int i;
    for (i = 0; i < ORDER; i++)  
    {  
        newptr->child_ptr[i] = NULL;  
    }  
    return newptr;
}

//Predefined Tree
TreeNode* initialize()
{
    TreeNode*root;
    root=MakeNode(1);
    int i;
    root->data_id[0]=4;
    root->data_id[1]=7;
    root->n=2;
    root->child_ptr[0]=MakeNode(1);
    root->child_ptr[0]->data_id[0]=2;
    root->child_ptr[0]->data_id[1]=3;
    root->child_ptr[0]->n=2;
    root->child_ptr[1]=MakeNode(1);
    root->child_ptr[1]->data_id[0]=5;
    root->child_ptr[1]->data_id[1]=6;
    root->child_ptr[1]->n=2;
    root->child_ptr[2]=MakeNode(1);
    root->child_ptr[2]->data_id[0]=8;
    root->child_ptr[2]->data_id[1]=9;
    root->child_ptr[2]->n=2;
    TreeNode*x;
    x=root;
    x=x->child_ptr[0];
    for(i=0;i<ORDER;++i)
    {
        x->child_ptr[i]=MakeNode(0);
        x->child_ptr[i]->data_id[0]=i+1;
        x->child_ptr[i]->n++;
        //printf("\nid= %d",x->child_ptr[i]->data_id[0]);

    }
    
    x=root;
    x=x->child_ptr[1];
    for(i=0;i<ORDER;++i)
    {
        x->child_ptr[i]=MakeNode(0);
        x->child_ptr[i]->data_id[0]=i+4;
        x->child_ptr[i]->n++;
        //printf("\nid= %d",x->child_ptr[i]->data_id[0]);
    }
    x=root;
    //printf("\nx->child_ptr[0]->child_ptr[2]->data_id[0]=%d",root->child_ptr[1]->child_ptr[2]->data_id[0]);
    x=x->child_ptr[2];
    for(i=0;i<ORDER;++i)
    {
        x->child_ptr[i]=MakeNode(0);
        x->child_ptr[i]->data_id[0]=i+7;
        x->child_ptr[i]->n++;
        //printf("\nid= %d",x->child_ptr[i]->data_id[0]);
    }
    x=root;
    //setting the doubly linked list
    for(i=0;i<ORDER;++i)
    {
        x=root;
        x=x->child_ptr[i];
        for(int j=0;j<ORDER;++j)
        {
            if(j==0)
            {
                x->child_ptr[j]->left=NULL;
            }
            else{
                x->child_ptr[j]->left=x->child_ptr[j-1];
            }
            if(j==ORDER-1)
            {
                x->child_ptr[j]->right=NULL;
            }
            else{
                x->child_ptr[j]->right=x->child_ptr[j+1];
            }
            
        }
    }
    x=root;
    for(i=0;i<ORDER;++i)
    {
        if(i==2)
        {
            x->child_ptr[i]->child_ptr[2]->right=NULL;
        }
        else{
            x->child_ptr[i]->child_ptr[2]->right=x->child_ptr[i+1]->child_ptr[0];
        }
        if(i==0)
        {
            x->child_ptr[i]->child_ptr[0]->left=NULL;
        }
        else{
            x->child_ptr[i]->child_ptr[0]->left=x->child_ptr[i-1]->child_ptr[2];
        }
    }
    x=root;
    x=x->child_ptr[2]->child_ptr[2];

    //printf("\nx->child_ptr[0]->child_ptr[2]->data_id[0]=%d",root->child_ptr[0]->child_ptr[2]->data_id[0]);
    //printf("\nx->child_ptr[0]->child_ptr[2]->data_id[0]=%d",root->child_ptr[1]->child_ptr[2]->data_id[0]);
    //printf("\nx->child_ptr[0]->child_ptr[2]->data_id[0]=%d",root->child_ptr[2]->child_ptr[2]->data_id[0]);
    //printf("\nHERE");
    return root;
}

bool IsEmpty(TreeNode*node)
{
    bool retval=false;
    if(node->leaf==true && node->n==0)
    {
        retval=true;
    }
    return retval;
}

void reverse(int arr[],int n)
{
    int i=0,temp,j;
    for(i=0,j=n-1;i<n/2;++i,--j)
    {
        temp=arr[i];
        arr[i]=arr[j];
        arr[j]=temp;
    }
}

int SearchMobilephone(MobilePhone*arr, char* x,int n) 
    { 
        /*int l = 0 ; 
        int r = n - 1; 
        int res=-1;
        while (l <= r)  
        { 
            int m = (l + (r - l)) / 2; 
  
         
        if (strcmp(arr[m].mob_no,x)==0) 
        {
            res=0;
            printf("\nin binary search found=%s to search=%s",arr[m].mob_no,x);
        } 
              
  
            // Check if x is present at mid 
            if (res == 0) 
            {
                printf("\nRETURNED value=%d",m);
                     return m;
        
            }
            
  
            // If x greater, ignore left half 
            if (x > (arr[m].mob_no)) 
                l = m + 1; 
  
            // If x is smaller, ignore right half 
            else
                r = m - 1; 
        } 
  
        return -1; */
        
        int i=0,retval=-1,flag=0;
        char temp[11];
        for(i=0;i<n && flag==0;++i)
        {
            strcpy(temp,arr[i].mob_no);
            if (strcmp(temp,x)==0) 
            {
                flag=1;
                retval=i;
            }
        }
        return retval;
    } 

bool IsMember(MobilePhone m,TreeNode*node)                              //returns true if m is present in the set
{
    
    bool retval=false;
    int index,flag=0;
    TreeNode*x=node;

    if(x->leaf==true)
    {
        index=SearchMobilephone(x->mobilephones,m.mob_no,x->num_phones);
        if(index!=-1)
        {
            retval=true;
        }
    }
    return retval;
}

void assign(MobilePhone*a,MobilePhone* b)
{
    strcpy(a->mob_no,b->mob_no);
}

int findPhone(MobilePhone m,TreeNode*root)                                      //returns the basestation(id) which is containing the given mobilephone
{
    int i,retval=-1;
        if(root->leaf==true && IsMember(m,root))
        {
            retval= root->data_id[0];
        }
        else if(root->leaf==false){
            for(i=0;i<=root->n && retval==-1;++i)
            retval=findPhone(m,root->child_ptr[i]);
        }
            return retval;
        
}

TreeNode* SetMembers(int base_stn_id,TreeNode*root)                                //returns basestation node(leaf node) corresponding to given id
{
    TreeNode*x=root,*retval=NULL;
    int i,flag=0;
    while(x->leaf==false)
        {
            flag=0;
            for(i=0;i<x->n && !flag;++i)
            { 
                if (base_stn_id < x->data_id[i])  
                {  
                    flag=1;  
                }  
            }
            if(flag)i--;
            x = x->child_ptr[i];
        }
        
        if(x->leaf==true){
            retval=x;
        }
        return retval;
}

MobilePhoneSet residentSet(Exchange e,int choice,TreeNode*root)                              //choice = 0 for basestation and choice = 1 for ith level exchanges where i>0
{
    TreeNode*set;
    MobilePhoneSet ret_set;
    int i;
    if(choice==0)                                                                          //if choice=0,i.e.,base station so setMember is called for given exchange e
    {
        set=SetMembers(e,root);
        for(i=0;i<set->num_phones;++i)
        {
            assign(&(ret_set.mobilephones[i]),&(set->mobilephones[i]));
        }
    }
    else{                                                                                   //if choice=1,then setMember() is called for its children and theor data is collectively stored in mobiephoneset object
        set=SetMembers(e-1,root);
        for(i=0;i<set->num_phones;++i)
        {
            assign(&(ret_set.mobilephones[i]),&(set->mobilephones[i]));
        }
        set=SetMembers(e,root);
        for(;i<set->num_phones;++i)
        {
            assign(&(ret_set.mobilephones[i]),&(set->mobilephones[i]));
        }
        set=SetMembers(e+1,root);
        for(;i<set->num_phones;++i)
        {
            assign(&(ret_set.mobilephones[i]),&(set->mobilephones[i]));
        }
    }
    return ret_set;
}

bool Insert(TreeNode*root,MobilePhone m,int base_stn_id)
{
    TreeNode* x=root;
    int i=0,flag=0;
    bool retval=true;
    if(root==NULL)
    {
        root=initialize();
        x=root;
    }
    else
    {
        //printf("\nx->child_ptr[0]->child_ptr[2]->data_id[0]=%d",x->child_ptr[0]->child_ptr[2]->data_id[0]);

        /*Go to the base station node(leaf node)by searching , then insert the mobile phone object into the set present in that leaf node.*/
        while(x->leaf==false)
        {
            //printf("\nHERE!!!");
            //printf("\nid= %d",x->data_id[i]);
            flag=0;
            for(i=0;i<x->n && !flag;++i)
            {
                if(base_stn_id<x->data_id[i])
                {
                    flag=1;
                } 
            }
            
            if(flag)i--;
            x = x->child_ptr[i]; 
        }
        
        if(x->leaf==true)
        {
            if(x->num_phones<MOBILES)
            {
                assign(&(x->mobilephones[x->num_phones]),&m);                    //INSERTION into the apt set
                x->num_phones++;                                
            }
            else{
                retval=false;
            }
        }
    }
    return retval;
}

bool Delete(TreeNode*root,MobilePhone m,int base_stn_id)
{
    int x;
    TreeNode*temp,*y;
    temp=root;
    int i,index;
    bool retval=true;
 
    x=findPhone(m,temp);                                    //first find the base station id of the given mobile phone by searching

    if(x!=-1)
    {
            y=SetMembers(x,temp);                          //get the set present in the node(leaf node) of corresponding base station id
            if(y!=NULL){
                if(IsMember(m,y))                           //checks if given mobilephone is the member of the got base station
                {
                                    i=0;
                                    index=SearchMobilephone(y->mobilephones,m.mob_no,y->num_phones);              //find the given phone's index in the set in the base staion then make it null
                                    while(y->mobilephones[index].mob_no[i] != '\0') {
                                        y->mobilephones[index].mob_no[i] = '\0';
                                        i++;
                                    }
                                    y->num_phones--;
                }
                else{
                    //printf("\n!!!Not present")
                    retval=false;
                }
            }
            else{
                    retval=false;
                }
    }
    else{
                    retval=false;
                        
    }
    return retval;
}

/*This function finds the lowest router between two base stations by storing path from base1 to root and from base2 to root and then finding the common ancestor*/
TreeNode* lowestRouter(int base1,int base2,TreeNode*root)
{
    int path1[LEVEL],path2[LEVEL],j=0,flag,i;
    TreeNode*x=root,*retval;
    if(base1==base2)
    {
        retval=SetMembers(base1,x);
    }
    else{
        while(x->leaf==false)                                                   //storing in path1 path from root to base1
            {
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    
                    if(base1<x->data_id[i])
                    {
                        flag=1;
                    }  
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                //printf("\nid= %d",x->data_id[0]);

                int k=0;
               /* while(base1>x->data_id[k] && k<x->n)
                {
                    k++;
                }*/
                path1[j++]=x->data_id[k];
                //printf("\npath1[j]=%d",path1[j-1]);
                x = x->child_ptr[i]; 
            }
            if(x->leaf==true)
            {
                path1[j++]=x->data_id[0];
            }
        j=0;
        x=root;
        while(x->leaf==false)                                                       //storing in path2 path from root to base2
            {
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    if(base2<x->data_id[i])
                    {
                        flag=1;
                    } 
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                int k=0;
                /*while(base2>x->data_id[k] && k<x->n)
                {
                    k++;
                }*/
                path2[j++]=x->data_id[k];
                //printf("\npath2[j]=%d",path2[j-1]);
                x = x->child_ptr[i]; 
            }
            if(x->leaf==true)
            {
                path2[j++]=x->data_id[0];
            }
            flag=0;
        
        /*Finding the point of difference between two paths , the last common node is the ancestor i.e.,the node before first difference*/
        for(i=0;i<j && !flag;++i)       
        {
            if(path1[i]!=path2[i])
            {
                flag=1;
            }
        }
        i--;
        int req_node_index=path1[i-1];

        x=root;
        while(x->data_id[0]!=req_node_index)                                                       //storing in path2 path from root to base2
            {
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    if(req_node_index<x->data_id[i])
                    {
                        flag=1;
                    } 
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                x = x->child_ptr[i]; 
            }
            printf("\nlowest router index of node= %d",req_node_index);
            retval=x;
    }
    return retval;
}

Exchangelist routeCall(MobilePhone a,MobilePhone b,TreeNode*root)
{
    int a_bs,b_bs,i=0,flag,j=0,path[LEVEL*2];
    TreeNode*x=root;
    /*find the base station id of th given phones*/
    a_bs=findPhone(a,x);                                                
    b_bs=findPhone(b,x);
    TreeNode*ances;
    Exchangelist route;

    /*If base station is not there i.e.,phone is not present in the database so mobile phone is not reachable*/
    if(a_bs==-1 || b_bs==-1)
    {
        printf("\nMobilephone not reachable... Try again later!!!");
    }
    else{
        if(b_bs==a_bs)
        {
            route.Exchanges[i]=a_bs;
        }
        else{

            ances=lowestRouter(a_bs,b_bs,x);
            x=ances;
            while(x->leaf==false)
            {
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    if(a_bs<x->data_id[i])
                    {
                        flag=1;
                    } 
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                path[j++]=x->data_id[0];
                x = x->child_ptr[i]; 
            }
            if(x->leaf==true)
            {
                path[j++]=x->data_id[0];
            }
            reverse(path,j);
            x=ances;
            j--;
            while(x->leaf==false)
            {
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    if(b_bs<x->data_id[i])
                    {
                        flag=1;
                    } 
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                path[j++]=x->data_id[0];
                x = x->child_ptr[i]; 
            }
            if(x->leaf==true)
            {
                path[j++]=x->data_id[0];
            }
            route.n=j;
            for(i=0;i<j;++i)
            {
                route.Exchanges[i]=path[i];
            }
        }

    }

    return route;
}

void printExchangeList(Exchangelist*list)
{
    int i;
    printf("\nTotal elements of exchange list= %d\n",list->n);
    for(i=0;i<list->n;++i)
    {
        printf(" %d -> ",list->Exchanges[i]);
    }
    printf("end\n");
}

void movePhone(MobilePhone a,int base,TreeNode*root)
{
    TreeNode*x;
    int location,flag,i;
    x=root;
    location=findPhone(a,x);
    if(location!=-1)
    while(x->leaf==false)
            {
                flag=0;
                for(i=0;i<x->n && !flag;++i)
                {
                    if(location<x->data_id[i])
                    {
                        flag=1;
                    } 
                }
                if(flag)i--;
                //printf("\nHERE!!!");
                //printf("\nid= %d",x->data_id[i]);
                x = x->child_ptr[i]; 
            }
            if(x->leaf==true)
            {
                MobilePhone*temp=MakeMobilePhone(a.mob_no);
                Delete(root,a,location);
                while(x->data_id[0]!=base && x!=NULL)
                {
                    if(location<base)
                    {
                        x=x->right;
                    }
                    else{
                        x=x->left;
                    }
                }
                if(x->num_phones!=MOBILES)
                {
                    assign(&(x->mobilephones[x->num_phones]),temp);
                    x->num_phones++;
                }
            }
}

void traverse(TreeNode *p)  
{  
    //TreeNode*p=root;
    int i;  
    //printf("\nTree is as follows:\n");
    printf("\n"); 
    for (i = 0; i < p->n; i++)  
    {  
        if (p->leaf == false)  
        {  
            traverse(p->child_ptr[i]);  
        }  
        printf(" %d ", p->data_id[i]); 
        if(p->num_phones!=0)
        {
            printf(" Number of phones in this base station = %d",p->num_phones);
        }
        /*else if(p->num_phones==0){
            printf(" Level %d node",i);
        }*/
        for(int j=0;j<p->num_phones;++j)
        {
            printf(" Phone %d = %s ",j+1,p->mobilephones[j].mob_no);
        } 
    }   
    if (p->leaf == false)  
    {  
        traverse(p->child_ptr[i]);  
    }  
    printf("\n");  
}

void MakeDatabase(TreeNode*root)
{
    MobilePhone *m1,*m2,*m3,*m4,*m5,*m6,*m7,*m8,*m9;
    m1=MakeMobilePhone("9876543210");
    m2=MakeMobilePhone("9424567059");
    m3=MakeMobilePhone("9234275059");
    m4=MakeMobilePhone("9424273454");
    m5=MakeMobilePhone("9821293210");
    m6=MakeMobilePhone("9871234510");
    m7=MakeMobilePhone("6546543210");
    m8=MakeMobilePhone("9098543210");
    m9=MakeMobilePhone("9236543298");
    
    Insert(root,*m1,1);
    Insert(root,*m2,2);
    Insert(root,*m3,3);
    Insert(root,*m4,4);
    Insert(root,*m5,5);
    Insert(root,*m6,6);
    Insert(root,*m7,7);
    Insert(root,*m8,8);
    Insert(root,*m9,9);    
}
int main()
{
    /*MobilePhone *m1,*m2;
    /*m1=MakeMobilePhone("9425259180");
    m2=MakeMobilePhone("9424275059");
    TreeNode*root=initialize();
    traverse(root);
    Insert(root,*m1,1);
    /*TreeNode*x;
    Insert(root,*m2,5);
    x=lowestRouter(1,6,root);*/
    MobilePhone*m3,*m1,*m2;
    m3=MakeMobilePhone("9234275059");
    m1=MakeMobilePhone("9876543210");
    m2=MakeMobilePhone("9236543298");
    TreeNode*root=initialize();
    MakeDatabase(root);
    traverse(root);
    //printf("\nDeleting m3 phone\n");
    //Delete(root,*m3,3);
    //traverse(root);
    TreeNode*x=lowestRouter(6,7,root);
    /*Exchangelist route;
    Exchangelist*route_ptr=&route;
    route=routeCall(*m1,*m2,root);
    printExchangeList(route_ptr);
    printf("\n\n\nNOW MOVING PHONE");
    movePhone(*m1,8,root);*/
    traverse(root);
    return 0;
}