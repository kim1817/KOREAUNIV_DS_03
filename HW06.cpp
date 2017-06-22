//��ó���� ����
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
  
//��� ����ü ����
typedef struct Pnode{

   //left,right,parent ��� ����
   Pnode * left;
   Pnode * right;
   Pnode * parent;

   int value;//����� value
   char color;//����� color

}Node;

//��������� ����
typedef Node * NodePtr;


//Ʈ�� ����ü ����
typedef struct RBTree{
   
   //Ʈ���� root �� NIL��� ����
   NodePtr root;
   NodePtr NIL;

   int count;//Ʈ���� �ִ� ����� ������ count�ϴ� ���
   int blackcount;//Ʈ���� �ִ� black����� ������ count�ϴ� ���
   int blackheight;//blackheight�� Ȯ���ϴ� ���
   int insert;//insert�� ����� ������ count�ϴ� ���
   int del;//delete�� ����� ������ count�ϴ� ���
   int miss;//miss�� ����� ������ count�ϴ� ���

}Tree;

//Ʈ�������� ����
typedef Tree * TreePtr;

//��带 �����ϴ� �Լ� ����
NodePtr NodeAlloc(TreePtr self,int data){

   NodePtr Pnode=(NodePtr)malloc(sizeof(Node));//��������� �����Ҵ�
   Pnode->color='R';//����� ���� �켱 ���������� ����.
   Pnode->value=data;
   //����� left,right,parent�� ��� NIL�� �ʱ� ����
   Pnode->left=self->NIL;
   Pnode->right=self->NIL;
   Pnode->parent=self->NIL;
   return Pnode;
}

//Ʈ���� �����ϴ� �Լ� ����
TreePtr TreeAlloc(){

   TreePtr PTree = (TreePtr)malloc(sizeof(Tree));//Ʈ�������� �����Ҵ�
   //Ʈ���� ��������� �� �ʱ�ȭ
   PTree->count=0;
   PTree->blackcount=0;
   PTree->blackheight=0;
   PTree->del=0;
   PTree->insert=0;
   PTree->miss=0;

   //NIL ����� ���� �� �ʱ�ȭ
   PTree->NIL=(NodePtr)malloc(sizeof(Node));
   PTree->NIL->left=PTree->NIL;
   PTree->NIL->right=PTree->NIL;
   PTree->NIL->parent=PTree->NIL;
   PTree->NIL->color='B';//�͹̳� ����� ���� black
   PTree->NIL->value=0;

   //Root ����� ���� �� �ʱ�ȭ
   PTree->root=NodeAlloc(PTree,0);
   PTree->root=PTree->NIL;
   PTree->root->parent=PTree->NIL;
   return PTree;
}

//Right Rotation �Լ�
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
   //Right Rotation �Ϸ�
}

//Left Rotation �Լ�
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
   //Left Rotation �Ϸ�
}



//rb_Search �Լ� ����
NodePtr rb_search(TreePtr self, NodePtr x, int data)
{
   //ã���� �ϴ� ���� root����� value�� ���
   if(x == self->NIL || x->value == data)
      return x;

   //ã���� �ϴ� ���� root����� value���� �������
   if(data < x->value)
	   return rb_search(self,x->left,data);

   //ã���� �ϴ� ���� root����� value���� Ŭ���
   else
      return rb_search(self,x->right,data);
}


//black����� ������ ���� �Լ� ����
void rb_blackcount(TreePtr self, NodePtr x)
{
   //��尡 NULL�϶�
   if(x==self->NIL)
      return ;
   //����带 ��������
   if(x->color=='B')
      self->blackcount++;//blackcount�� ����

   //��Ʈ��带 �������� �������� recursion
   rb_blackcount(self, x->left);
   //��Ʈ��带 �������� ���������� recursion
   rb_blackcount(self, x->right);
}


//blackheight�� ����ϴ� �Լ� ����
int rb_blackheight(TreePtr self, NodePtr x)
{
   int bh=1;//���̸� ��Ÿ���� ���� ���� �� �ʱ�ȭ

   
   while (x->left != self->NIL && x->right != self->NIL)//terminal������ ����
   {
      if(x->color=='B')
         bh++;
      x=x->right;
   }
   return bh;
}

//Ʈ���� �ִ� ������ inorder traversal����� ������ִ� �Լ� ����
void RBprint_inorder(TreePtr self, NodePtr x)
{
   //Ʈ���� �ƹ� ��尡 ������� �Լ� ����
   if(x == self->NIL)
      return;

   //recursion�� �̿��Ͽ� red-blackƮ���� ���Ĺ�Ŀ� ���߾� ��带 ����Ѵ�
   RBprint_inorder(self,x->left);
   printf("%d",x->value);
   printf(" %c\n",x->color);
   RBprint_inorder(self,x->right);
}


//Red-Black tree Insert Operation����

// 1. Insert Fixup �Լ�
int Insert_Fixup(TreePtr self, NodePtr target)
{
   //Ÿ�ٳ�尡 Ʈ���� ��Ʈ����̰ų� Ÿ�ٳ���� �θ��尡 Ʈ���� ��Ʈ����϶�
   if( target==self->root || target->parent==self->root)
   {
      self->root->color='B';
      return 0;
   }
   

   while(target != self->NIL && target != self->root && target->parent->color=='R')
   {

     //Ÿ�ϳ���� �θ��尡 Ÿ�ٳ���� �Ҿƹ��� ����� left�϶�
      if(target->parent == target->parent->parent->left)
      {
         NodePtr uncle = target->parent->parent->right;//���̳�� ����

       //���̳���� color�� ���� case�� ����� ����
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

     //Ÿ�ϳ���� �θ��尡 Ÿ�ٳ���� �Ҿƹ��� ����� right�϶�
      else
      {
         NodePtr uncle = target->parent->parent->left;//���̳�� ����

       //���̳���� color�� ���� ����
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

   self->root->color = 'B';//Ʈ���� root����� �� RBT ���ǿ� ���� ���������� ����

   return 0;
}


// 2. Insert �Լ� ����
int rb_insert(TreePtr self, int data)
{
   // NIL���, Root���, �߰��� ��� ����
   NodePtr a = self->NIL;
   NodePtr b = self->root;
   NodePtr newNode = NodeAlloc(self, data);

   //Ʈ���� �������
   if(self->count==0)
   {
      self->root=newNode;//���� �߰��� ��带 Root���� ����
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

   //�߰��� ��忡 ���Ͽ� left���, right���, color ����
   newNode->left = self->NIL;
   newNode->right = self->NIL;
   newNode->color = 'R';

   //Insert�� �Ǹ鼭 Red-BlackƮ���� ������ ���������Ƿ� Fixup�� ���ش�.
   Insert_Fixup(self, newNode);
   //�� ����� �� �÷��ֱ�
   self->count++;
   return 0;
}



//Red-Black tree Delete Operation����

// 1. Transplant ����
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

// 2. Minimun ����
NodePtr Minimum(TreePtr self, NodePtr x)
{
   while(x->left != self->NIL){
      x = x->left;
   }
   return x;
}


// 3. Delete_Fixup ����
void Delete_fixup(TreePtr self, NodePtr x) {

   while (x != self->root && x->color == 'B') {
       
	   //Ÿ�ٳ�尡 �θ����� left����϶�
	   if (x == x->parent->left) {

         NodePtr b = x->parent->right;//������� ����
         
		 //��������� color�� Red�϶�
		 if (b->color == 'R') 
         {
            b->color = 'B';
            x->parent->color = 'R';
            Rotate_left(self, x->parent);
            b = x->parent->right;
         }

		 //��������� color�� Black�϶�
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
	  

	  //Ÿ�ٳ�尡 �θ����� right����϶�
	  else {

         NodePtr b = x->parent->left;//������� ����
         
		 //��������� color�� Red�϶�
		 if (b->color == 'R') {
            b->color = 'B';
            x->parent->color = 'R';
            Rotate_left(self, x->parent);
            b = x->parent->left;
         }

		 //��������� Color�� Black�϶�
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

// 4. Delete �Լ� ����
void rb_delete(TreePtr self, NodePtr target)
{
   NodePtr x;
   NodePtr y = target;//Ÿ�ٳ���� ���� ������ �����ϱ� ���� ��������

   char original_color = y->color;//Ÿ�ٳ���� ���� ���� �����ϴ� ����


   //Ÿ�ٳ���� left��尡 ������
   if (target->left == self->NIL)
   {
      x = target->right;
      Transplant(self,target,target->right);
   }

   //Ÿ�ٳ���� right��尡 ������
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


   //Ÿ�ٳ���� ���� Red��� Fixup�� ��ġ�� �ʾƵ� �ȴ�
   if (original_color == 'B') 
   {
      Delete_fixup(self, x); 
   }

   //�� ����� �� �ٿ��ֱ�
   self->count--;
}


// Tree-Maximum ����
NodePtr Maximum(TreePtr self, NodePtr x){
	while(x->right != self->NIL){
		x = x->right;
	}
	return x;
}

//Successor ���ϴ� �Լ� ����
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

//Predeccessor ���ϴ� �Լ� ����
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
	//ã���� �ϴ� data�� Ʈ���� ������
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

	//ã���� �ϴ� data�� Ʈ���� ������
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

   //0���� ū ���� ������ �����͸� Ʈ���� Insert�ؾ��ϹǷ� Insert-Operation�Լ��� ȣ���Ͽ� �۾��� �����Ѵ�.
   if(data>0)
   {
	   rb_insert(self,data);
       self->insert++;//Insert�� ����� ���� ���� ������ ���������ش�
   }


   //0���� ���� ���� ������ �����͸� Ʈ������ Delete�ؾ��ϹǷ� Delete-Operation�Լ��� ȣ���Ͽ� �۾��� �����Ѵ�.
   else if(data<0) 
   {
      if(rb_search(self,self->root,(data*(-1)))==self->NIL)
        {
         self->miss++;//������� ������ Ʈ���� ���� ����� ���� ���� ������ ���������ش�
     }
     else 
      {
         rb_delete(self,rb_search(self,self->root,(data*(-1))));
         self->del++;//Delete�� ����� ���� ���� ������ ���������ش�
      }
   }
   else
	   return ;
   
}


void main()
{
    TreePtr RBT = TreeAlloc();//Ʈ������
       int data1, data2;//text���� ���� �����͸� ��Ÿ���� ���� ����

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
	  //0�ΰ�� ó��
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
