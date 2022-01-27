#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct patricia{
        char*bits;
        int bitnumber;
        int data;
        struct patricia*left;
        struct patricia*right;
}patricia;

patricia*bit(patricia*next,int bitnum,char*k);
patricia*search(patricia*t,char*k);
void searchbit(patricia*t,char*k);
patricia*insert(patricia*t,char*bits,int data);
void printall(patricia*root,int bitnum);
patricia*deletebit(patricia*root,char*k);


int main(){

        patricia*root=NULL;
        char operation[7];
        int number;
        scanf("%d",&number);
        do{
                scanf("%s",operation);
                if(strcmp(operation,"insert")==0){
                        int data;
                        char*bitnumber=(char*)malloc(sizeof(char)*number+1);
                        scanf("%s",bitnumber);
                        scanf("%d",&data);
                        root=insert(root,bitnumber,data);
                }else if(strcmp(operation,"search")==0){
                        char*bitnumber=(char*)malloc(sizeof(char)*number+1);
                        scanf("%s",bitnumber);
                        searchbit(root,bitnumber);
                }else if(strcmp(operation,"delete")==0){
                        char*bitnumber=(char*)malloc(sizeof(char)*number+1);
                        scanf("%s",bitnumber);
                        root=deletebit(root,bitnumber);
                }
        }while(strcmp(operation,"quit")!=0);
        return 0;
}


void searchbit(patricia*t,char*k)
{
        printf("search -> ");
        if(t==NULL){
                printf("not found\n");
                return;
        }
        patricia*temp=search(t,k);
        if(strcmp(temp->bits,k)==0){
                printf("%d\n",temp->data);
        }else{
                printf("not found\n");
        }
}
//pg567(textbook)
patricia*search(patricia*t,char*k)
{
        if(t==NULL){
                return NULL;
        }
        patricia*current=t;
        patricia*next=t->left;

        while(next->bitnumber>current->bitnumber){
                current=next;
                next=bit(next,next->bitnumber,k);
        }
        return next;
}

patricia*bit(patricia*next,int bitnum,char*k)
{
        bitnum=bitnum-1;
        char bit2[2];
        bit2[0]=k[bitnum];
        bit2[1]='\0';

        if(strcmp(bit2,"1")==0){
                //printf("right\n");
                return next->right;
        }else if(strcmp(bit2,"0")==0){
                //printf("left\n");
                return next->left;
        }
        return NULL;
}

//pg569(textbook)
patricia*insert(patricia*t,char*bits,int data)
{
        printf("insert -> ");
        if(t==NULL){
                t=(patricia*)malloc(sizeof(patricia));
                t->bitnumber=0;
                t->data=data;
                t->bits=bits;
                t->left=t;
                t->right=NULL;
                printf("%d\n",data);
                return t;
        }

        patricia*lastnode;
        lastnode=search(t,bits);
        if(strcmp(lastnode->bits,bits)==0){
                printf("conflict\n");
                return t;
        }
        /*if(lastnode->data==data){
                printf("conflict\n");
                return t;
        }*/

        int i;
        for(i=0;bits[i]==lastnode->bits[i];i++);
        //printf("i=%d\n",i);

        patricia*current=t->left;
        patricia*parent=t;
        while(current->bitnumber>parent->bitnumber&&current->bitnumber<i+1){
                parent=current;
                current=bit(current,current->bitnumber,bits);
        }
        patricia*newnode=(patricia*)malloc(sizeof(patricia));
        newnode->bitnumber=i+1;
        newnode->bits=bits;
        newnode->data=data;
        char bit2[2];
        bit2[0]=newnode->bits[i];
        bit2[1]='\0';

        if(strcmp(bit2,"1")==0){
                newnode->left=current;
                newnode->right=newnode;
        }else{
                newnode->left=newnode;
                newnode->right=current;
        }

        if(current==parent->left){
                parent->left=newnode;
        }else{
                parent->right=newnode;
        }
        printf("%d\n",newnode->data);
        return t;
}

void printall(patricia*root,int bitnum)
{
        if(root->bitnumber<=bitnum){
                return;
        }
        printf("%s ",root->bits);
        printf("%d ",root->bitnumber);
        printall(root->left,root->bitnumber);
        printall(root->right,root->bitnumber);

}


patricia*deletebit(patricia*root,char*k)
{
        printf("delete -> ");
        //null root
        if(root==NULL){
                printf("not found\n");
                return root;
        }
        //search deletenode(next,temp),parent,child(current)
        patricia*next=root->left;
        patricia*current=root;
        patricia*parent=current;
        int flag=0; //parent use
        if(strcmp(next->bits,k)==0){
                        flag=1;//find the parent
                }
        while(next->bitnumber>current->bitnumber){
                current=next;
                next=bit(next,next->bitnumber,k);
                if(strcmp(next->bits,k)==0&&flag==0){
                        parent=current;
                        flag=1;
                }
        }
        patricia*temp=next;//delete node

        if(strcmp(temp->bits,k)!=0){
                printf("not found\n");
                return root;
        }

        //root node only
        if(temp->right==NULL&&temp->left==root){ //
                 printf("%d\n",temp->data);//
                root=NULL;
                return root;
        }
        patricia*child=NULL;

        //one self pointer
        if(temp->left==temp||temp->right==temp){
                //root node
                printf("%d\n",temp->data);
                if(temp->right==NULL){
                        child=temp->left;
                        root=parent;
                        root->left=child;
                        root->right=NULL;
                        root->bitnumber=0;

                }else{
                        //find child
                        if(temp->left==temp){
                                child=temp->right;
                        }else if(temp->right==temp){
                                child=temp->left;
                        }
                        //change parent pointer
                        if(parent->left==temp){
                                parent->left=child;
                        }else if(parent->right==temp){
                                parent->right=child;
                        }
                }
        //no self pointer
        }else{
                child=current;//back pointer of delete node
                printf("%d\n",temp->data);
                //root node
                if(temp->right==NULL){
                        root=child;
                        root->right=NULL;
                        root->left=temp->left;
                        root->bitnumber=0;
                        return root;
                }
                //similiary to search
                //find the back pointer of the back pointer of delete node(temp)
                //find back pointer from delete node(temp)

                patricia*current2=temp;//back pointer of child
                patricia*next2=bit(temp,temp->bitnumber,child->bits);
                patricia*parent2=current2;
                int flag2=0;
                if(strcmp(next2->bits,child->bits)==0){
                                flag2=1;
                        }
                while(next2->bitnumber>current2->bitnumber){
                        current2=next2;
                        next2=bit(next2,next2->bitnumber,k);
                        if(strcmp(next2->bits,child->bits)==0&&flag2==0){
                                parent2=current2;
                                flag2=1;
                        }
                }
                //change pointer (lec51-patricia (9th))
                temp->bits=child->bits;
                temp->data=child->data;
                if(current2->left==child){
                        current2->left=temp;
                }else if(current2->right==child){
                        current2->right=temp;
                }

                if(child->left==temp){
                        if(parent2->left==child){
                                parent2->left=child->right;
                        }else if(parent2->right==child){
                                parent2->right=child->right;
                        }
                }else if(child->right==temp){
                        if(parent2->left==child){
                                parent2->left=child->left;
                        }else if(parent2->right==child){
                                parent2->right=child->left;
                        }
                }
                //end of change pointer (lec51-patricia (9th))
        }
        return root;
}
