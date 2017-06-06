#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Node* NodePtr;//노드구조체의 포인터 선언

//color를 전역변수로 선언
const int red = 1;
const int black = 0;

struct Node{
  int val;
  int color;
  NodePtr left, right, parent;
};//노드의 구조체 선언

NodePtr NodePtr_alloc(int newval) {
  NodePtr self = (NodePtr)malloc(sizeof(struct Node));
  self->val = newval;
  self->left = NULL;
  self->right = NULL;
  self->parent = NULL;
  self->color = NULL;
  return self;
}//노드의 생성 및 값 할당(자료값 및 left,right 주소값)


NodePtr gp(NodePtr n)//gp NodePtr 정의
{
    return n->parent->parent;
}
 
NodePtr uncle(NodePtr n)//uncle NodePtr 정의
{
    if (n->parent == gp(n)->left)
        return gp(n)->right;
    else
        return gp(n)->left;
}
NodePtr sibling(NodePtr n)// sibling NodePtr 정의
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
};//rb 구조체 선언


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

//insert 함수 선언
void insert_case1(rbPtr self,NodePtr n);
void insert_case2(rbPtr self,NodePtr n);
void insert_case3(rbPtr self,NodePtr n);
void insert_case4(rbPtr self,NodePtr n);
void insert_case5(rbPtr self,NodePtr n);

void left_rotate(rbPtr self, NodePtr x) {//left rotation 구현
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

 void right_rotate(rbPtr self, NodePtr x) {//right rotation 구현
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

void insert_case1(rbPtr self,NodePtr n)//노드가 처음 삽입 되었을때
{
    if (n->parent == NULL)//루트노드일 경우
        n->color = black;
    else
        insert_case2(self,n);//해당하지 않으면 case 2로
}
void insert_case2(rbPtr self,NodePtr n)//부모노드의 color가 검정일때
{
    if (n->parent->color == black)
        return; //부모노드의 color가 black이라면 rb-tree가 여전히 유효하기 때문에 fixup함수 종료
    else
        insert_case3(self,n);//해당하지 않으면 case 3로
}
void insert_case3(rbPtr self,NodePtr n)//삼촌노드가 존재하고, 그 노드의 color가 red일 경우
{
    if (uncle(n) != NULL && uncle(n)->color == red)
    {
        n->parent->color = black;
        uncle(n)->color = black;
        gp(n)->color = red;
 
        insert_case1(self,gp(n));//할아버지 노드의 색이 red이고 black을 자식노드로 가지기 때문에 노드가 처음 삽입되었을때의 알고리즘 적용 가능
    }
    else
        insert_case4(self,n);//해당하지 않으면 case4로
}

void insert_case4(rbPtr self,NodePtr n)//부모노드의 color가 red이고 삼촌노드의 color가 black일 경우    
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
    insert_case5(self,n);//해당하지 않으면 case5로
}
 

void insert_case5(rbPtr self,NodePtr n)//노드와 부모노드, 할아버지 노드까지 모두 같은 방향일 경우
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
	
  if (self->root == NULL){//트리에 아무 노드가 없을때
    self->root = n;
	self->root->color = black;
	n->parent = self->NIL;
	self->count++;
	//self->NIL->color = black;
}
  else if (n->val <= tree->val) {//루트노드가 더 큰경우
    if (tree->left == NULL){
      tree->left = n;
	  n->parent = tree;
	  n->color = red;
	  n->left = self->NIL;
	  n->right = self->NIL;
	  self->count++;
	}//left가 비어있는 경우.
    else
      rb_insert(self,tree->left,n);//recursion을 통해 하위 서브트리 생성.
  }

  else{//루트노드보다 큰 경우
    if (tree->right == NULL){//right가 비었을경우 그 위치에 할당
      tree->right = n;
	  n->parent = tree;
	  n->color = red;
	  n->left = self->NIL;
	  n->right = self->NIL;
	  self->count++;
	}
    else
      rb_insert(self,tree->right,n);//recursion을 통해 하위 서브트리 생성
  }
  
  insert_case1(self,n);//insert_fixup을 위해 case1 함수 호출
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
            if( (s->left)->color == black && (s->right)->color == black)//오류
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
	if(value->left == NULL && value->right == NULL){//타겟노드의 자식노드가 없는경우
		if(value == value->parent->left)
			value->parent->left = NULL;
		else if(value == value->parent->right)
			value->parent->right = NULL;
		self->count--;
	}

	else if(value->left != NULL && value->right == NULL){//자식노드가 왼쪽에만 있는경우
		if(value->parent->left == value){//타겟노드가 부모노드의 left NodePtr인경우
			value->parent->left = value->left;
			value->left->parent = value->parent;
			self->count--;
		}
		else if(value->parent->right == value){//타겟노드가 부모노드의 right NodePtr인경우
			value->parent->right = value->left;
			value->left->parent = value->parent;
			self->count--;
		}
	}
	else if(value->right != NULL && value->left == NULL){//자식노드가 오른쪽에만 있는경우
		if(value->parent->left == value){//타겟노드가 부모노드의 left NodePtr인경우
			value->parent->left = value->right;
			value->right->parent = value->parent;
			self->count--;
		}
		else if(value->parent->right == value) {//타겟노드가 부모노드의 right NodePtr인경우
			value->parent->right = value->right;
			value->right->parent = value->parent;
			self->count--;
		}
	}
	else if(value->left != NULL && value->right != NULL){//타겟노드가 두개의 자식노드를 가지는경우
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
//stack을 이용한 inorder travel
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
  rbPtr rb = rb_alloc();//트리 생성
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
  printf("\n노드의 색:");
  printf("%d ",rb->root->color);
  printf("%d ",rb->root->left->color);
  printf("%d ",rb->root->right->color);
  printf("%d ",rb->root->right->left->color);
  printf("%d ",rb->root->right->right->color);
  printf("%d\n",rb->root->right->right->right->color);
  printf("\n레드:1, 블랙: 0입니다\n");
  printf("총 노드의 개수: %d",rb->count);
  printf("\n\\\\\\\\\\\\\\\\\\\\\\\n");
  
  Node * n = rb_search(rb,rb->root,4);
  rb_delete(rb ,rb->root,n);
  rb_print(rb,rb->root,0);
  printf("%d\n",rb->count);
  rb_inorder_iter(rb->root);

}
