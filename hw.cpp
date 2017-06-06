#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Node* NodePtr;//��屸��ü�� ������ ����

//color�� ���������� ����
const int red = 1;
const int black = 0;

struct Node{
  int val;
  int color;
  NodePtr left, right, parent;
};//����� ����ü ����

NodePtr NodePtr_alloc(int newval) {
  NodePtr self = (NodePtr)malloc(sizeof(struct Node));
  self->val = newval;
  self->left = NULL;
  self->right = NULL;
  self->parent = NULL;
  self->color = NULL;
  return self;
}//����� ���� �� �� �Ҵ�(�ڷᰪ �� left,right �ּҰ�)


NodePtr gp(NodePtr n)//gp NodePtr ����
{
    return n->parent->parent;
}
 
NodePtr uncle(NodePtr n)//uncle NodePtr ����
{
    if (n->parent == gp(n)->left)
        return gp(n)->right;
    else
        return gp(n)->left;
}
NodePtr sibling(NodePtr n)// sibling NodePtr ����
{
 if ((n == NULL) || (n->parent == NULL))
  return NULL; 
 if (n == n->parent->left)
  return n->parent->right;
 else
  return n->parent->left;
}

typedef struct rb* rbPtr;

struct rb {
  NodePtr root;
  NodePtr NIL;
  int count;
  int black_count;
};//rb ����ü ����


rbPtr rb_alloc()
{
   rbPtr rb = (rbPtr)malloc(sizeof(struct rb));
   rb->root = NULL;
   rb->count = 0;
   rb->black_count = 0;
   rb->NIL = NULL;
   return rb;
}
void create_nilnode(rbPtr r)
{
    r->NIL  = (NodePtr)malloc(sizeof(struct Node));
    (r->NIL)->color = black;
    r->root = r->NIL;
}

//insert �Լ� ����
void insert_case1(rbPtr self,NodePtr n);
void insert_case2(rbPtr self,NodePtr n);
void insert_case3(rbPtr self,NodePtr n);
void insert_case4(rbPtr self,NodePtr n);
void insert_case5(rbPtr self,NodePtr n);

void left_rotate(rbPtr self, NodePtr x) {//left rotation ����
   NodePtr y;
   y = x->right;
   x->right = y->left;
   if (y->left != NULL) {
      y->left->parent = x;
   }
   y->parent = x->parent;
   if (x->parent == NULL) {
      self->root = y;
	  y->parent = self->NIL;
   }
   else if (x == x->parent->left) {
      x->parent->left = y;
   }
   else {
      x->parent->right = y;
   }
   y->left = x;
   x->parent = y;
}

 void right_rotate(rbPtr self, NodePtr x) {//right rotation ����
    NodePtr y;
    y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
       y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
       self->root = y;
    }
    else if (x == x->parent->left) {
       x->parent->left = y;
    }
    else {
       x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

void insert_case1(rbPtr self,NodePtr n)//��尡 ó�� ���� �Ǿ�����
{
    if (n->parent == NULL)//��Ʈ����� ���
        n->color = black;
    else
        insert_case2(self,n);//�ش����� ������ case 2��
}
void insert_case2(rbPtr self,NodePtr n)//�θ����� color�� �����϶�
{
    if (n->parent->color == black)
        return; //�θ����� color�� black�̶�� rb-tree�� ������ ��ȿ�ϱ� ������ fixup�Լ� ����
    else
        insert_case3(self,n);//�ش����� ������ case 3��
}
void insert_case3(rbPtr self,NodePtr n)//���̳�尡 �����ϰ�, �� ����� color�� red�� ���
{
    if (uncle(n) != NULL && uncle(n)->color == red)
    {
        n->parent->color = black;
        uncle(n)->color = black;
        gp(n)->color = red;
 
        insert_case1(self,gp(n));//�Ҿƹ��� ����� ���� red�̰� black�� �ڽĳ��� ������ ������ ��尡 ó�� ���ԵǾ������� �˰��� ���� ����
    }
    else
        insert_case4(self,n);//�ش����� ������ case4��
}

void insert_case4(rbPtr self,NodePtr n)//�θ����� color�� red�̰� ���̳���� color�� black�� ���    
{
    if (n == n->parent->right && n->parent == gp(n)->left)
    {
        left_rotate(self,n->parent);
        n = n->left;
    }
    else if (n == n->parent->left && n->parent == gp(n)->right)
    {
        right_rotate(self,n->parent);
        n = n->right;
    }
    insert_case5(self,n);//�ش����� ������ case5��
}
 

void insert_case5(rbPtr self,NodePtr n)//���� �θ���, �Ҿƹ��� ������ ��� ���� ������ ���
{
    n->parent->color = black;
    gp(n)->color = red;
 
    if (n == n->parent->left && n->parent == gp(n)->left)
    {
        right_rotate(self,gp(n));
    }
    else
    {
        left_rotate(self,gp(n));
    }
}


void rb_insert(rbPtr self,NodePtr tree,NodePtr n){//insert operation
	
  if (self->root == NULL){//Ʈ���� �ƹ� ��尡 ������
    self->root = n;
	self->root->color = black;
	n->parent = self->NIL;
	self->count++;
	//self->NIL->color = black;
}
  else if (n->val <= tree->val) {//��Ʈ��尡 �� ū���
    if (tree->left == NULL){
      tree->left = n;
	  n->parent = tree;
	  n->color = red;
	  n->left = self->NIL;
	  n->right = self->NIL;
	  self->count++;
	}//left�� ����ִ� ���.
    else
      rb_insert(self,tree->left,n);//recursion�� ���� ���� ����Ʈ�� ����.
  }

  else{//��Ʈ��庸�� ū ���
    if (tree->right == NULL){//right�� �������� �� ��ġ�� �Ҵ�
      tree->right = n;
	  n->parent = tree;
	  n->color = red;
	  n->left = self->NIL;
	  n->right = self->NIL;
	  self->count++;
	}
    else
      rb_insert(self,tree->right,n);//recursion�� ���� ���� ����Ʈ�� ����
  }
  
  insert_case1(self,n);//insert_fixup�� ���� case1 �Լ� ȣ��
}

void transplant(rbPtr self, NodePtr t, NodePtr c){
	if(t->parent = NULL){
		self->root = c;
	}
	else if(t == t->parent->left){
		t->parent->left = c;
	}
	else
		t->parent->right = c;
	if(c!= NULL)
		c->parent = t->parent;
}


void rb_delete_fixup(rbPtr r, NodePtr x)
{
    NodePtr s = NULL; // sibling NodePtr.


	while( (x != r->root) && ( x->color == black))
    {
		if( x == (x->parent)->left)
        {
			s   = (x->parent)->right;

            // case 1 : x's sibling s is red
			if( s->color == red)
            {
                s->color      = black;
                (x->parent)->color = red;
                left_rotate( r, x->parent);
                // update x's sibling
                s   = (x->parent)->right;
            }
            // case 2 : x's sibling s is black, and both of s's children are black.
            if( (s->left)->color == black && (s->right)->color == black)
            {
                s->color  = red;
                x       = x->parent;
            }
            // case 3 : x's sibling s is black, s's left child is red, another is black.
            else if( (s->left)->color == red && (s->right)->color == black)
            {
                s->color          = red;
                (s->left)->color  = black;
                right_rotate( r, s);
                // update x's sibling
                s   = (x->parent)->right;
            }
            // case 4 : x's sibling s is black, s's right child is red.
            if( (s->right)->color == red)
            {
                s->color          = (x->parent)->color;
                (s->right)->color = black;
                (x->parent)->color     = black;
                left_rotate( r, x->parent);
            
                x   = r->root;
            }
			return;
        }
        else
        {
            s   = (x->parent)->left;

            // case 1 : x's sibling s is red
           if( s->color == red)
            {
                s->color      = black;
                (x->parent)->color = red;
                right_rotate( r, x->parent);
                // update x's sibling
                s   = (x->parent)->left;
            }
            // case 2 : x's sibling s is black, and both of s's children are black.
            if( (s->left)->color == black && (s->right)->color == black)//����
            {
                s->color  = red;
                x       = x->parent;
            }
            // case 3 : x's sibling s is black, s's right child is red, another is black.
            else if( (s->right)->color == red && (s->left)->color == black)
            {
                s->color          = red;
                (s->right)->color = black;
                left_rotate( r, s);
                // update x's sibling
                s   = (x->parent)->left;
            }
            // case 4 : x's sibling s is black, s's left child is red.
            if( (s->left)->color == red)
            {
                s->color          = (x->parent)->color;
                (s->left)->color  = black;
                (x->parent)->color     = black;
                right_rotate( r, x->parent);
            
                x   = r->root;
            }
        }
    }
    x->color  = black;
}

void rb_delete(rbPtr self, NodePtr tree, NodePtr value){
	NodePtr x = value;
	if(value->left == NULL && value->right == NULL){//Ÿ�ٳ���� �ڽĳ�尡 ���°��
		if(value == value->parent->left)
			value->parent->left = NULL;
		else if(value == value->parent->right)
			value->parent->right = NULL;
		self->count--;
	}

	else if(value->left != NULL && value->right == NULL){//�ڽĳ�尡 ���ʿ��� �ִ°��
		if(value->parent->left == value){//Ÿ�ٳ�尡 �θ����� left NodePtr�ΰ��
			value->parent->left = value->left;
			value->left->parent = value->parent;
			self->count--;
		}
		else if(value->parent->right == value){//Ÿ�ٳ�尡 �θ����� right NodePtr�ΰ��
			value->parent->right = value->left;
			value->left->parent = value->parent;
			self->count--;
		}
	}
	else if(value->right != NULL && value->left == NULL){//�ڽĳ�尡 �����ʿ��� �ִ°��
		if(value->parent->left == value){//Ÿ�ٳ�尡 �θ����� left NodePtr�ΰ��
			value->parent->left = value->right;
			value->right->parent = value->parent;
			self->count--;
		}
		else if(value->parent->right == value) {//Ÿ�ٳ�尡 �θ����� right NodePtr�ΰ��
			value->parent->right = value->right;
			value->right->parent = value->parent;
			self->count--;
		}
	}
	else if(value->left != NULL && value->right != NULL){//Ÿ�ٳ�尡 �ΰ��� �ڽĳ�带 �����°��
		NodePtr mNodePtr = value->right;
		NodePtr mpNodePtr = value;

		while(mNodePtr->left != NULL)
		{
			mpNodePtr = mNodePtr;
			mNodePtr = mNodePtr->left;
		}

		value->val = mNodePtr->val;

		if(mpNodePtr->left == mNodePtr)
		{
			mpNodePtr->left = mNodePtr->right;
			mNodePtr->parent->left= NULL;
			self->count--;
		}
		else if(mpNodePtr->right == mNodePtr)
		{
			mpNodePtr->right = mNodePtr->right;
			mNodePtr->parent->right= NULL;
			self->count--;
		}
	}

	/*if(x->color == black){
	rb_delete_fixup(self,x);
	}*/
}

NodePtr rb_search(rbPtr self, NodePtr tree, int value){
	if(value == tree->val)
		return tree;
	else if(value > tree->val){
		rb_search(self,tree->right, value);
	}
	else if(value < tree->val)
		rb_search(self,tree->left, value);
	else
		return 0;
}

void rb_print(rbPtr self, NodePtr tree, int level) {
  if (tree->right != NULL)
    rb_print(self,tree->right, level + 1);
  for(int i = 0; i < level; i++)
    printf("    ");
  printf("%d\n", tree->val);
  if (tree->left != NULL)
    rb_print(self, tree->left, level + 1);
}
//stack�� �̿��� inorder travel
struct Stack{
	NodePtr arr[100];
	int top;
};

void stackinit(Stack * pstack){
	pstack->top = 0;
}
int is_empty(Stack * pstack){
	return pstack->top==0;
}	
void push(Stack * pstack, NodePtr val){
	pstack->arr[pstack->top++] = val;
}
NodePtr pop(Stack * pstack){
	return pstack->arr[--pstack->top];
}

void rb_inorder_iter(NodePtr tree) {
	Stack stk;
	stackinit(&stk);
	while(!is_empty(&stk) || tree !=NULL)
	{
		if(tree != NULL){
			push(&stk,tree);
			tree = tree->left;
		}
		else
		{
			tree = pop(&stk);
			printf("%d ",tree->val);
			tree = tree->right;
		}
	}
}
int count_black(NodePtr tree){
	Stack stk;
	stackinit(&stk);
	int num=0;
	while(!is_empty(&stk) || tree!=NULL){
		if(tree != NULL && tree->color==black){
			push(&stk,tree);
			tree = tree->left;
		}
		else
		{
			tree = pop(&stk);
			num++;
			tree = tree->right;
		}
	}
	return num;
}

void main() {
  rbPtr rb = rb_alloc();//Ʈ�� ����
  //rb->black_count = count_black(rb->root);
  //create_nilnode(rb);
  rb_insert(rb,rb->root,NodePtr_alloc(2));
  rb_insert(rb,rb->root,NodePtr_alloc(3));
  rb_insert(rb,rb->root,NodePtr_alloc(4));
  rb_insert(rb,rb->root,NodePtr_alloc(5));
  rb_insert(rb,rb->root,NodePtr_alloc(6));
  rb_insert(rb,rb->root,NodePtr_alloc(7));
  //rb->black_count = count_black(rb->root);
  rb_print(rb,rb->root,0);
  rb_inorder_iter(rb->root);
  printf("\n����� ��:");
  printf("%d ",rb->root->color);
  printf("%d ",rb->root->left->color);
  printf("%d ",rb->root->right->color);
  printf("%d ",rb->root->right->left->color);
  printf("%d ",rb->root->right->right->color);
  printf("%d\n",rb->root->right->right->right->color);
  printf("\n����:1, ��: 0�Դϴ�\n");
  printf("�� ����� ����: %d",rb->count);
  printf("\n\\\\\\\\\\\\\\\\\\\\\\\n");
  
  Node * n = rb_search(rb,rb->root,4);
  rb_delete(rb ,rb->root,n);
  rb_print(rb,rb->root,0);
  printf("%d\n",rb->count);
  rb_inorder_iter(rb->root);

}
