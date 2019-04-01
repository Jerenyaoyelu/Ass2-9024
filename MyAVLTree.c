#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<stdbool.h>
#include<string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v ){
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree(){
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}
// declaration
int InsertNode(AVLTree *T, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);

int convert(char *string){
	int num = 0;
	int pre = 1;
	if(string[0]=='-'){
		pre = -1;
	}
	for(int i = 0; i<strlen(string);i++){
		if(string[i]!='-'){
			int temp = string[i] - '0';
			num = num*10+temp;
		}
	}
	num = num*pre;
	return num;
}

int *getKV(char *data_string){
	char parenthses = '\0';
	char integer[255]="";
	int len;
	int j = 0;
	int * values = (int *)malloc(255 * sizeof(int));
	for(int i = 0; i < strlen(data_string);i++){
		if(data_string[i] == '(' || data_string[i] == ')'){
			if(parenthses == '\0'){
				parenthses = data_string[i];
			}else if(data_string[i] == parenthses){
				printf("Error syntax!!\n");
				exit(0);
			}else{
				int num;
				num = convert(integer);
				//no problem!!! how stupid am I !!!
				values[j+1] = num;
				// printf("%d\n",values[j]);
				j++;
				strcpy(integer,"");
				parenthses = '\0';
			}
		}else if(data_string[i] == ','){
			int num;
			num  = convert(integer);
			//no problem!!!
			values[j+1] = num;
			// printf("%d\n",values[j]);
			j++;
			strcpy(integer,"");
		}else if(data_string[i] != ' '){
			len = strlen(integer);
			integer[len] = data_string[i];
			integer[len+1] = '\0';
		}
	}
	// insert the szie into the head of values
	values[0] = j;
	return values;
}

// Compare two nodes
// returns 1 when the about-to-insert node is smaller; 
// returns 0 when they are equal; returns -1 when he about-to-insert node is bigger.
int comparasion(AVLTreeNode *node,int k, int v){
	if (node->key > k){
		return 1;
	}else if(node->key == k){
		if(node->value > v){
			return 1;
		}else if(node->value == v){
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

// check if the node is in balance
bool isBalance(AVLTreeNode *newNode){
	if((newNode->height - newNode->left->height) > 2 || (newNode->height - newNode->right->height) > 2){
		return false;
	}else{
		return true;
	}
}

// increase height
void increaseH(AVLTreeNode *N){
	AVLTreeNode *current = N;
	while(current->parent!=NULL){
		current->height++;
		current = current->parent;
	}
}

//rotation
//O(log(n))
void rotation(AVLTree *unbalancedT, AVLTreeNode *insertedN){
	if(unbalancedT->size > 1){
		// find x,y,z
		AVLTreeNode *x=NULL, *y=NULL, *z = NULL;
		AVLTreeNode *a, *b, *c;
		AVLTreeNode *current = insertedN;
		while(1){
			if(isBalance(current->parent)){
				current = current->parent;
			}else{
				break;
			}
		}
		z = current;
		if(z->left->height >= z->right->height){
			y = z->left;
		}else{
			y = z->right;
		}
		if(y->left->height >= y->right->height){
			x = y->left;
		}else{
			x = y->right;
		}
		// assign a,b,c
		if(comparasion(x,y->key,y->value)>=0){
			c = x;
			a = y;
			if(comparasion(y,z->key,z->value)>=0){
				a = z;
				b = y;
			}else{
				if(comparasion(x,z->key,z->value)>=0){
					b = z;
				}else{
					b = x;
					c = z;
				}
			}
		}else{
			a = x;
			c = y;
			if(comparasion(y,z->key,z->value)>=0){
				if(comparasion(x,z->key,z->value)>=0){
					a = z;
					b = x;
				}else{
					b = z;
				}
			}else{
				b = y;
				c = z;
			}
		}
		//restructring the tree
		b->parent = z->parent;
		if(b->left != NULL){
			a->right = b->left;
		}
		b->left = a;
		if(b->right != NULL){
			c->left = b->right;
		}
		b->right = c;
	}
}

// put your time complexity analysis of CreateAVLTree() here
AVLTree *CreateAVLTree(const char *filename){
 // create an empty tree
 AVLTree *tree;
 tree = newAVLTree();
 int t;
 char data_string[255];
 char data_pieces[255];
 if(strncmp(filename,"stdin",5)==0){
	 while (1){
		// assgin the input string to the data_string line by line
		gets(data_pieces);
		// if it is a empty line, then the input is done, ending up reading from the standard input
		if(strlen(data_pieces)==0){
			break;
		}
		strcat(data_string,data_pieces);
	 }
 }else{
		FILE *fp;
		fp = fopen(filename,"r");
		if (fp == NULL){
			printf("Error occurs when opening file!");
			return NULL;
		}
		//iteratively read tuples until the end of the file
		while(1){
			//reads string and stops when encounter white-space or new line
			fscanf(fp,"%s",data_pieces);
			//end of the file
			if(feof(fp)){
				break;
			}
			strcat(data_string,data_pieces);
		}
	}
	int *pp;
	pp = getKV(data_string);
	//pp[0] is the size of the array!!
	for (int k = 1; k <=pp[0] ; k++ ) {
		//got the (key,value)!!
		t = InsertNode(tree,*(pp+k),*(pp+k+1));
		//make sure the loop goes by step 2
		k++;
	}
	printf("size:%d\n",tree->size);
	return tree;
}

// //put your time complexity analysis for CloneAVLTree() here
// AVLTree *CloneAVLTree(AVLTree *T)
// {
//  // put your code here
//   AVLTreeNode *current = T->root;
// 	AVLTreeNode *crtCln;
// 	AVLTree *subL, *subR, *clone;
// 	subL = newAVLTree();
// 	subR = newAVLTree();
// 	clone = newAVLTree();
// 	clone->size = T->size;
// 	clone->root = T->root;
// 	crtCln = clone->root;
	
// }
 
// // put your time complexity for ALVTreesUNion() here
// AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
// {
// 	//put your code here
// }
 
// // put your time complexity for ALVTreesIntersection() here
// AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
// {
// 	 //put your code here
// }

// O(log(n)) 
// return 0 if item exists in the tree, otherwise return 1.  
int InsertNode(AVLTree *T, int k, int v){
	AVLTreeNode *newNode;
	newNode = newAVLTreeNode(k,v);
	// empty tree
	if(T->root == NULL){
		T->root = newNode;
		T->size++;
		return 1;
	}
	AVLTreeNode *current = T->root;
	while(1){
		if(comparasion(current,k,v)>0){
			if(current->left == NULL){
				current->left = newNode;
				break;
			}else{
				current = current->left;
			}
		}else if(comparasion(current,k,v)==0){
			// printf("existing node!\n");
			return 0;
		}else{
			if(current->right == NULL){
				current->right = newNode;
				break;
			}else{
				current = current->right;
			}
		}
	}
	T->size++;
	newNode->parent = current;
	increaseH(current);
	// problem:
	// rotation(T,newNode);
	return 1;
}

// put your time complexity for DeleteNode() here
// int DeleteNode(AVLTree *T, int k, int v)
// {
//  // put your code here
// }

// O(log(n))
// The tree is empty, so return NULL; return a node if it finds the item, otherwise, return NULL.
AVLTreeNode *Search(AVLTree *T, int k, int v){
	if(T->root == NULL){
		// The tree is empty, so return NULL.
		return NULL;
	}else{
		AVLTreeNode *crnt = T->root;
		// return a node if it finds the item, otherwise, return NULL.
		while(crnt != NULL){
			if(comparasion(crnt,k,v) == 0){
				return crnt;
			}else if(comparasion(crnt,k,v) > 0){
				crnt = crnt->left;
			}else{
				crnt = crnt->right;
			}
		}
		return NULL;
	}
}
// O(n)
void FreeAVLTree(AVLTree *T){
	// put your code here
	AVLTree *subLeft,*subRight;
	subLeft = newAVLTree();
	subRight = newAVLTree();
	if(T->root->left != NULL){
		subLeft->root = T->root->left;
		T->root->left->parent = NULL;
		T->root->left = NULL;
		FreeAVLTree(subLeft);
	}
	if(T->root->right != NULL){
		subRight->root = T->root->right;
		T->root->right->parent = NULL;
		T->root->right = NULL;
		FreeAVLTree(subRight);
	}
	T->size = 0;
	free(T->root);
}

// O(n)
void PrintAVLTree(AVLTree *T){
 // put your code here
	AVLTreeNode *current = T->root;
	AVLTree *subL, *subR;
	subL = newAVLTree();
	subR = newAVLTree();
	if(current->left != NULL){
		subL ->root = current->left;
		PrintAVLTree(subL);
	}
	printf("(%d,%d),%d\n",current->key,current->value,current->height);
	if(current->right != NULL){
		subR ->root = current->right;
		PrintAVLTree(subR);
	}
}

//sample main for testing 
int main(){ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("File1.txt");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
//  tree2=CreateAVLTree("file1.txt"); 
//  PrintAVLTree(tree2);
//  tree3=CloneAVLTree(tree2);
//  PrintAVLTree(tree3);
//  FreeAVLTree(tree2);
//  FreeAVLTree(tree3);
//  //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  node1 = Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
//   for (i=17; i>0; i--)
//   {
//     j=DeleteNode(tree4, i, i);
// 	if (j==0) 
// 	  printf("Key %d does not exist\n",i);  
//     PrintAVLTree(tree4);
//   }
//  FreeAVLTree(tree4);
//  //Create tree5
//  tree5=newAVLTree();
//  j=InsertNode(tree5, 6, 25);
//  j=InsertNode(tree5, 6, 10);
//  j=InsertNode(tree5, 6, 12);
//  j=InsertNode(tree5, 6, 20);
//  j=InsertNode(tree5, 9, 25);
//  j=InsertNode(tree5, 10, 25);
//  PrintAVLTree(tree5);
//  //Create tree6
//  tree6=newAVLTree();
//  j=InsertNode(tree6, 6, 25);
//  j=InsertNode(tree6, 5, 10);
//  j=InsertNode(tree6, 6, 12);
//  j=InsertNode(tree6, 6, 20);
//  j=InsertNode(tree6, 8, 35);
//  j=InsertNode(tree6, 10, 25);
//  PrintAVLTree(tree6);
//  tree7=AVLTreesIntersection(tree5, tree6);
//  tree8=AVLTreesUnion(tree5,tree6);
//  PrintAVLTree(tree7);
//  PrintAVLTree(tree8);
 return 0; 
}
