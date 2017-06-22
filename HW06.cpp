//전처리문 선언
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
  
//노드 구조체 선언
typedef struct Pnode{

   //left,right,parent 노드 선언
   Pnode * left;
   Pnode * right;
   Pnode * parent;

   int value;//노드의 value
   char color;//노드의 color

}Node;

//노드포인터 설정
typedef Node * NodePtr;


//트리 구조체 선언
typedef struct RBTree{
   
   //트리의 root 및 NIL노드 선언
   NodePtr root;
   NodePtr NIL;

   int count;//트리에 있는 노드의 개수를 count하는 멤버
   int blackcount;//트리에 있는 black노드의 개수를 count하는 멤버
   int blackheight;//blackheight를 확인하는 멤버
   int insert;//insert된 노드의 개수를 count하는 멤버
   int del;//delete된 노드의 개수를 count하는 멤버
   int miss;//miss된 노드의 개수를 count하는 멤버

}Tree;

//트리포인터 설정
typedef Tree * TreePtr;

//노드를 생성하는 함수 선언
NodePtr NodeAlloc(TreePtr self,int data){

   NodePtr Pnode=(NodePtr)malloc(sizeof(Node));//노드포인터 동적할당
   Pnode->color='R';//노드의 색을 우선 붉은색으로 설정.
   Pnode->value=data;
   //노드이 left,right,parent를 모두 NIL로 초기 설정
   Pnode->left=self->NIL;
   Pnode->right=self->NIL;
   Pnode->parent=self->NIL;
   return Pnode;
}

//트리를 생성하는 함수 선언
TreePtr TreeAlloc(){

   TreePtr PTree = (TreePtr)malloc(sizeof(Tree));//트리포인터 동적할당
   //트리의 멤버변수의 값 초기화
   PTree->count=0;
   PTree->blackcount=0;
   PTree->blackheight=0;
   PTree->del=0;
   PTree->insert=0;
   PTree->miss=0;

   //NIL 노드의 생성 및 초기화
   PTree->NIL=(NodePtr)malloc(sizeof(Node));
   PTree->NIL->left=PTree->NIL;
   PTree->NIL->right=PTree->NIL;
   PTree->NIL->parent=PTree->NIL;
   PTree->NIL->color='B';//터미널 노드의 색은 black
   PTree->NIL->value=0;

   //Root 노드의 생성 및 초기화
   PTree->root=NodeAlloc(PTree,0);
   PTree->root=PTree->NIL;
   PTree->root->parent=PTree->NIL;
   return PTree;
}

//Right Rotation 함수
void Rotate_right(TreePtr self, NodePtr root)
{

   NodePtr y=root->left;
   root->left=y->right;
   
   if(y->right!=self->NIL)
   {
      y->right->parent=root;
   }

   y->parent=root->parent;
   

   if(root->parent==self->NIL)
   {
      self->root=y;
   }
   
   else if(root==root->parent->left)
   {
      root->parent->left=y;
   }
   else 
   {
      root->parent->right=y;
   }
   y->right=root;
   root->parent=y;
   //Right Rotation 완료
}

//Left Rotation 함수
void Rotate_left(TreePtr self, NodePtr root)
{

   NodePtr y= root->right;
   
   root->right=y->left;
   
   if(y->left!=self->NIL)
   {
      y->left->parent=root;
   }
   
   y->parent=root->parent;

   if(root->parent==self->NIL)
   {
      self->root=y;
   }
   
   else if(root==root->parent->left)
   {
      root->parent->left=y;
   }   
   else
   {
         root->parent->right=y;
   }
   y->left=root;
   root->parent=y;
   //Left Rotation 완료
}



//rb_Search 함수 구현
NodePtr rb_search(TreePtr self, NodePtr x, int data)
{
   //찾고자 하는 값이 root노드의 value일 경우
   if(x == self->NIL || x->value == data)
      return x;

   //찾고자 하는 값이 root노드의 value보다 작을경우
   if(data < x->value)
	   return rb_search(self,x->left,data);

   //찾고자 하는 값이 root노드의 value보다 클경우
   else
      return rb_search(self,x->right,data);
}


//black노드의 개수를 세는 함수 구현
void rb_blackcount(TreePtr self, NodePtr x)
{
   //노드가 NULL일때
   if(x==self->NIL)
      return ;
   //블랙노드를 만났을때
   if(x->color=='B')
      self->blackcount++;//blackcount값 증가

   //루트노드를 기준으로 왼쪽으로 recursion
   rb_blackcount(self, x->left);
   //루트노드를 기준으로 오른쪽으로 recursion
   rb_blackcount(self, x->right);
}


//blackheight를 계산하는 함수 구현
int rb_blackheight(TreePtr self, NodePtr x)
{
   int bh=1;//높이를 나타내는 변수 선언 및 초기화

   
   while (x->left != self->NIL && x->right != self->NIL)//terminal노드까지 진행
   {
      if(x->color=='B')
         bh++;
      x=x->right;
   }
   return bh;
}

//트리에 있는 노드들의 inorder traversal결과를 출력해주는 함수 구현
void RBprint_inorder(TreePtr self, NodePtr x)
{
   //트리에 아무 노드가 없을경우 함수 종료
   if(x == self->NIL)
      return;

   //recursion을 이용하여 red-black트리의 정렬방식에 맞추어 노드를 출력한다
   RBprint_inorder(self,x->left);
   printf("%d",x->value);
   printf(" %c\n",x->color);
   RBprint_inorder(self,x->right);
}


//Red-Black tree Insert Operation구현

// 1. Insert Fixup 함수
int Insert_Fixup(TreePtr self, NodePtr target)
{
   //타겟노드가 트리의 루트노드이거나 타겟노드의 부모노드가 트리의 루트노드일때
   if( target==self->root || target->parent==self->root)
   {
      self->root->color='B';
      return 0;
   }
   

   while(target != self->NIL && target != self->root && target->parent->color=='R')
   {

     //타켓노드의 부모노드가 타겟노드의 할아버지 노드의 left일때
      if(target->parent == target->parent->parent->left)
      {
         NodePtr uncle = target->parent->parent->right;//삼촌노드 선언

       //삼촌노드의 color에 따라 case를 나누어서 진행
         if(uncle->color == 'R')
         {
            target->parent->color='B';
            uncle->color = 'B';
            target->parent->parent->color = 'R';
            target = target->parent->parent;
         }

         else 
            { 
               if( target == target->parent->right)
               {
                  target = target->parent;
                  Rotate_left(self,target);
               }         
            
            target->parent->color='B';
            target->parent->parent->color='R';
            Rotate_right(self,target->parent->parent);
         }
      }

     //타켓노드의 부모노드가 타겟노드의 할아버지 노드의 right일때
      else
      {
         NodePtr uncle = target->parent->parent->left;//삼촌노드 선언

       //삼촌노드의 color에 따라 진행
         if(uncle->color == 'R')
         {
            target->parent->color='B';
            uncle->color = 'B';
            target->parent->parent->color='R';
            target = target->parent->parent;
         }

         else 
         {
               if( target == target->parent->left)
               {
                  target = target->parent;
                  Rotate_right(self,target);
               }
            target->parent->color='B';
            target->parent->parent->color='R';
            Rotate_left(self,target->parent->parent);
         }
      }
   }

   self->root->color = 'B';//트리의 root노드의 색 RBT 정의에 따라 검정색으로 설정

   return 0;
}


// 2. Insert 함수 구현
int rb_insert(TreePtr self, int data)
{
   // NIL노드, Root노드, 추가할 노드 선언
   NodePtr a = self->NIL;
   NodePtr b = self->root;
   NodePtr newNode = NodeAlloc(self, data);

   //트리가 비었을때
   if(self->count==0)
   {
      self->root=newNode;//새로 추가된 노드를 Root노드로 설정
      self->count++;
      return 0;
   }

   
   while ( b != self->NIL)         
   {
      a = b;
      if(newNode->value < b->value)
      {
         b = b->left;
      }
      else
      {
         b = b->right;
      }
   }

   newNode->parent = a;

   if( newNode->value < a->value)
   {
      a->left = newNode;
   }
   else
   {
      a->right = newNode;
   }

   //추가된 노드에 대하여 left노드, right노드, color 지정
   newNode->left = self->NIL;
   newNode->right = self->NIL;
   newNode->color = 'R';

   //Insert가 되면서 Red-Black트리의 균형이 무너졌으므로 Fixup을 해준다.
   Insert_Fixup(self, newNode);
   //총 노드의 수 늘려주기
   self->count++;
   return 0;
}



//Red-Black tree Delete Operation구현

// 1. Transplant 구현
void Transplant(TreePtr self, NodePtr a, NodePtr b)
{
   if(a->parent == self->NIL)
      self->root=b;

   else if(a == a->parent->left)
      a->parent->left = b;
   
   else
      a->parent->right = b;

   b->parent = a->parent;
}

// 2. Minimun 구현
NodePtr Minimum(TreePtr self, NodePtr x)
{
   while(x->left != self->NIL){
      x = x->left;
   }
   return x;
}


// 3. Delete_Fixup 구현
void Delete_fixup(TreePtr self, NodePtr x) {

   while (x != self->root && x->color == 'B') {
       
	   //타겟노드가 부모노드의 left노드일때
	   if (x == x->parent->left) {

         NodePtr b = x->parent->right;//형제노드 선언
         
		 //형제노드의 color가 Red일때
		 if (b->color == 'R') 
         {
            b->color = 'B';
            x->parent->color = 'R';
            Rotate_left(self, x->parent);
            b = x->parent->right;
         }

		 //형제노드이 color가 Black일때
         if (b->left->color =='B' && b->right->color == 'B')
         {
            b->color = 'R';
            x = x->parent;
         }

         else if (b->right->color == 'B') 
         {
            b->left->color = 'B';
            b->color = 'R';
            Rotate_right(self, b);
            b = x->parent->right;
         }

         b->color = x->parent->color;
         x->parent->color = 'B';
         b->right->color = 'B';
         Rotate_left(self, x->parent);
         x = self->root;
      }
	  

	  //타겟노드가 부모노드의 right노드일때
	  else {

         NodePtr b = x->parent->left;//형제노드 선언
         
		 //형제노드의 color가 Red일때
		 if (b->color == 'R') {
            b->color = 'B';
            x->parent->color = 'R';
            Rotate_left(self, x->parent);
            b = x->parent->left;
         }

		 //형제노드의 Color가 Black일때
         if (b->right->color == 'B' && b->left->color == 'B') {
            b->color = 'R';
            x = x->parent;
         }

         else if (b->left->color == 'B') {
            b->right->color = 'B';
            b->color = 'R';
            Rotate_left(self, b);
            b = x->parent->left;
         }

         b->color = x->parent->color;
         x->parent->color = 'B';
         b->left->color = 'B';
         Rotate_right(self, x->parent);
         x = self->root;
      }
   }

   x->color = 'B';
}

// 4. Delete 함수 구현
void rb_delete(TreePtr self, NodePtr target)
{
   NodePtr x;
   NodePtr y = target;//타겟노드의 원래 정보를 저장하기 위한 변수선언

   char original_color = y->color;//타겟노드의 원래 색을 저장하는 변수


   //타겟노드의 left노드가 없을때
   if (target->left == self->NIL)
   {
      x = target->right;
      Transplant(self,target,target->right);
   }

   //타겟노드의 right노드가 없을때
   else if (target->right == self->NIL)
   {
      x = target->left;
      Transplant(self,target,target->left);
   }


   else 
   {
      y = Minimum(self, target->right);
      original_color = y->color;
      x = y->right;

      if (y->parent == target) 
      {
         x->parent = y;
      }
      else
      {
         Transplant(self,y,y->right);
         y->right = target->right;
         y->right->parent = y;
      }

      Transplant(self,target,y);
      y->left = target->left;
      y->left->parent = y;
      y->color = target->color;
   }


   //타겟노드의 색이 Red라면 Fixup을 거치지 않아도 된다
   if (original_color == 'B') 
   {
      Delete_fixup(self, x); 
   }

   //총 노드의 수 줄여주기
   self->count--;
}


// Tree-Maximum 구현
NodePtr Maximum(TreePtr self, NodePtr x){
	while(x->right != self->NIL){
		x = x->right;
	}
	return x;
}

//Successor 구하는 함수 구현
NodePtr Tree_Successor(TreePtr self, NodePtr x){

	if(x->right != self->NIL){
		return Minimum(self,x->right);
	}

	NodePtr y = x->parent;

	while(y!=self->NIL && x == y->right){
		x = y;
		y = y->parent;
	}
	return y;
}

//Predeccessor 구하는 함수 구현
NodePtr Tree_predeccessor(TreePtr self, NodePtr x){

	if(x->left != self->NIL){
		return Maximum(self,x->left);
	}
	NodePtr y = x->parent;
	while(y != self->NIL && x == y->left){
		x = y;
		y = y->parent;
	}
	return y;
}

void search_operation(TreePtr self, int data){
	FILE *fp3=NULL;

	fp3 = fopen("output.txt","a");


	if(data == 0)
		return;
	//찾고자 하는 data가 트리에 있을때
	if(rb_search(self,self->root, data) != self->NIL){
		if(Tree_predeccessor(self,rb_search(self, self->root, data)) != self->NIL){
			fprintf(fp3,"%d  ", Tree_predeccessor(self,rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3,"NIL  ");
		
		fprintf(fp3,"%d  ", data);
		
		if(Tree_Successor(self,rb_search(self, self->root, data)) != self->NIL){
			fprintf(fp3,"%d\n", Tree_Successor(self,rb_search(self, self->root, data))->value);
		}
		else 
			fprintf(fp3,"NIL\n");
	}

	//찾고자 하는 data가 트리에 없을때
	else
	{
		rb_insert(self,data);

		if(Tree_predeccessor(self,rb_search(self, self->root, data)) != self->NIL){
			fprintf(fp3,"%d  ", Tree_predeccessor(self,rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3,"NIL  ");
		
		fprintf(fp3,"NIL  ");
		
		if(Tree_Successor(self,rb_search(self, self->root, data)) != self->NIL){
			fprintf(fp3,"%d\n", Tree_Successor(self,rb_search(self, self->root, data))->value);
		}
		else 
			fprintf(fp3,"NIL\n");

		rb_delete(self,rb_search(self, self->root, data));

	}
	fclose(fp3);

}

void input_operation(TreePtr self, int data)
{

   //0보다 큰 값을 만나면 데이터를 트리에 Insert해야하므로 Insert-Operation함수를 호출하여 작업을 수행한다.
   if(data>0)
   {
	   rb_insert(self,data);
       self->insert++;//Insert한 노드의 수를 세는 변수를 증가시켜준다
   }


   //0보다 작은 값을 만나면 데이터를 트리에서 Delete해야하므로 Delete-Operation함수를 호출하여 작업을 수행한다.
   else if(data<0) 
   {
      if(rb_search(self,self->root,(data*(-1)))==self->NIL)
        {
         self->miss++;//지우려고 했으나 트리에 없던 노드의 수를 세는 변수를 증가시켜준다
     }
     else 
      {
         rb_delete(self,rb_search(self,self->root,(data*(-1))));
         self->del++;//Delete한 노드의 수를 세는 변수를 증가시켜준다
      }
   }
   else
	   return ;
   
}


void main()
{
    TreePtr RBT = TreeAlloc();//트리생성
       int data1, data2;//text파일 안의 데이터를 나타내는 변수 선언

       FILE *fp1=NULL;
	   FILE *fp2=NULL;


	   //Input Operation
       fopen_s(&fp1,"input.txt", "r");
       fscanf_s(fp1, "%d", &data1);
       while(data1!=0)
       {
          input_operation(RBT,data1);
          fscanf_s(fp1, "%d", &data1);
       }
	  //0인경우 처리
      input_operation(RBT,data1);
      fclose(fp1);


	  //Search Operation
	  fopen_s(&fp2,"search.txt","r");
	  fscanf_s(fp2, "%d", &data2);
	  while(data2!=0)
	  {
		  search_operation(RBT,data2);
		  fscanf_s(fp2, "%d", &data2);
	  }
	  search_operation(RBT,data2);
	  fclose(fp2);


	  return ;
}
