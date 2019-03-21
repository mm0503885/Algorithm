#include <fstream>
#include <iostream>
#include <string>
using namespace std;

  ifstream fin;
  ofstream fout;

class RBT;
class TreeNode{
private:
    int key;
    string color;
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    friend class RBT;
public:
    TreeNode(int key):leftchild(0),rightchild(0),parent(0),key(key),color("red"){}
    TreeNode():leftchild(0),rightchild(0),parent(0),key(0),color("red"){}
};

class RBT{
private:
    TreeNode *neel;
    TreeNode *root;

    void LeftRotation(TreeNode *current);
    void RightRotation(TreeNode *current);
    void InsertFixedUpRBT(TreeNode *current);
    void DeleteFixedUpRBT(TreeNode *current);
    void Transplant(TreeNode *u,TreeNode *v);
    TreeNode* Minimum(TreeNode *x);
public:
    RBT(){
        neel = new TreeNode;
        neel->color = "black";
        root = neel;
        root->parent = neel;
    };

    void InsertRBT(int key);
    void DeleteRBT(int KEY);
    void Display_tree_inorder(TreeNode *current);
    void Display_tree();

};

TreeNode* RBT::Minimum(TreeNode *x)
{
  while(x->leftchild!=neel)
    x=x->leftchild;
  return x;
}


void RBT::Display_tree()
{
  Display_tree_inorder(root);
}

void RBT::Display_tree_inorder(TreeNode *current)
{
  if(current!=neel)
  {
    Display_tree_inorder(current->leftchild);
    if(current==root)
      fout<<"key: "<<current->key<<" parent:  "<<" color: "<<current->color<<endl;
    else
      fout<<"key: "<<current->key<<" parent: "<<current->parent->key<<" color: "<<current->color<<endl;
    Display_tree_inorder(current->rightchild);
  }
}


void RBT::LeftRotation(TreeNode *x)
{
    TreeNode *y = x->rightchild;
    x->rightchild = y->leftchild;
    if (y->leftchild != neel) y->leftchild->parent = x;
    y->parent = x->parent;
    if (x->parent == neel) root = y;
    else if (x == x->parent->leftchild) x->parent->leftchild = y;
    else x->parent->rightchild = y;
    y->leftchild = x;
    x->parent = y;
}
void RBT::RightRotation(TreeNode *x)
{
    TreeNode *y = x->leftchild;
    x->leftchild = y->rightchild;
    if (y->rightchild != neel) y->rightchild->parent = x;
    y->parent = x->parent;
    if (x->parent == neel) root = y;
    else if (x == x->parent->leftchild) x->parent->leftchild = y;
    else x->parent->rightchild = y;
    y->rightchild = x;
    x->parent = y;
}

void RBT::Transplant(TreeNode *u,TreeNode *v)
{
  if(u->parent==neel) root=v;
  else if(u==u->parent->leftchild) u->parent->leftchild=v;
  else u->parent->rightchild=v;
  v->parent=u->parent;
}

void RBT::InsertRBT(int key){
    TreeNode *insert_node = new TreeNode(key);
    insert_node->leftchild = neel;
    insert_node->rightchild = neel;
    TreeNode *current = root;
    TreeNode *current_parent = neel;

    if(root==neel)
    {
      insert_node->parent=neel;
      insert_node->color="black";
      root = insert_node;
    }
    else
    {
      while(current!=neel)
      {
        current_parent = current;
        if(insert_node->key < current->key)
          current=current->leftchild;
        else
          current=current->rightchild;
      }
      insert_node->parent = current_parent;
      if(insert_node->key < current_parent->key)
        current_parent->leftchild = insert_node;
      else
        current_parent->rightchild = insert_node;
    }

    InsertFixedUpRBT(insert_node);
}

void RBT::InsertFixedUpRBT(TreeNode *current)
{
  while (current->parent->color == "red")
  {
    if(current->parent == current->parent->parent->leftchild)
    {
      TreeNode *uncle = current->parent->parent->rightchild;
      if (uncle->color == "red")
      {
        current->parent->parent->color = "red";
        uncle->color = "black";
        current->parent->color = "black";
        current = current->parent->parent;
      }
      else
      {
        if (current == current->parent->rightchild)
        {
          current = current->parent;
          LeftRotation(current);
        }
        current->parent->parent->color = "red";
        current->parent->color = "black";
        RightRotation(current->parent->parent);
      }
    }
    else
    {
      TreeNode *uncle = current->parent->parent->leftchild;
      if (uncle->color == "red")
      {
        current->parent->parent->color = "red";
        uncle->color = "black";
        current->parent->color = "black";
        current = current->parent->parent;
      }
      else
      {
        if (current == current->parent->leftchild)
        {
          current = current->parent;
          RightRotation(current);
        }
        current->parent->parent->color = "red";
        current->parent->color = "black";
        LeftRotation(current->parent->parent);
      }
    }
  }
    root->color = "black";
}

void RBT::DeleteRBT(int KEY)
{
  TreeNode *delete_node=root;
  TreeNode *x;
  while(delete_node!=neel)
  {
    if(delete_node->key==KEY) break;
    else if(delete_node->key < KEY) delete_node=delete_node->rightchild;
    else delete_node=delete_node->leftchild;
  }
  TreeNode *y = delete_node;
  string y_original_color = y->color;
  if(delete_node->leftchild==neel)
  {
    x=delete_node->rightchild;
    Transplant(delete_node,delete_node->rightchild);
  }
  else if(delete_node->rightchild==neel)
  {
    x=delete_node->leftchild;
    Transplant(delete_node,delete_node->leftchild);
  }
  else
  {
    y = Minimum(delete_node->rightchild);
    y_original_color = y->color;
    x = y->rightchild;
    if(y->parent == delete_node) x->parent = y ;
    else
    {
      Transplant(y,y->rightchild);
      y->rightchild=delete_node->rightchild;
      delete_node->rightchild->parent=y;
    }
    Transplant(delete_node,y);
    y->leftchild = delete_node->leftchild;
    delete_node->leftchild->parent = y;
    y->color = delete_node->color;
  }
  if(y_original_color == "black")
    DeleteFixedUpRBT(x);



}
void RBT::DeleteFixedUpRBT(TreeNode *current)
{
  while(current!=root && current->color=="black")
  {
    TreeNode *sibling;
    if(current==current->parent->leftchild)
    {
      sibling = current->parent->rightchild;
      if(sibling->color == "red")
      {
        sibling->color = "black";
        current->parent->color = "red";
        LeftRotation(current->parent);
        sibling = current->parent->rightchild;
      }
      if(sibling->leftchild->color == "black" && sibling->rightchild->color == "black")
      {
        sibling->color == "red";
        current=current->parent;
      }
      else
      {
        if(sibling->rightchild->color == "black")
        {
          sibling->leftchild->color == "black";
          sibling->color == "red";
          RightRotation(sibling);
          sibling = current->parent->rightchild;
        }
        sibling->color = current->parent->color;
        current->parent->color = "black";
        sibling->rightchild->color = "black";
        LeftRotation(current->parent);
        current = root;
      }
    }
    else
    {
      sibling = current->parent->leftchild;
      if(sibling->color == "red")
      {
        sibling->color = "black";
        current->parent->color = "red";
        RightRotation(current->parent);
        sibling = current->parent->leftchild;
      }
      if(sibling->rightchild->color == "black" && sibling->leftchild->color == "black")
      {
        sibling->color == "red";
        current=current->parent;
      }
      else
      {
        if(sibling->leftchild->color == "black")
        {
          sibling->rightchild->color == "black";
          sibling->color == "red";
          LeftRotation(sibling);
          sibling = current->parent->leftchild;
        }
        sibling->color = current->parent->color;
        current->parent->color = "black";
        sibling->leftchild->color = "black";
        RightRotation(current->parent);
        current = root;
      }
    }
  }
  current->color = "black";
}
int main()
{
    fin.open("input.txt");
    fout.open("output.txt");

    char c;
    unsigned int i;

     while(fin.get(c))
    {
      RBT red_black_tree;
      unsigned int number_of_works=0;
      while(c>=48 && c<=57)
      {
        number_of_works*=10;
        i=c-'0';
        number_of_works+=i;
        fin.get(c);
      }
      char op;
      for(unsigned int turn=0;turn<number_of_works;turn++)
      {
        fin.get(c);
        op=c;
        fin.get(c);
        if(op=='1')
        {
          fout<<"Insert:";
          bool negative=false;
          int insert_key=0;
          while(fin.get(c))
          {
            if(c=='\n' || c==' ')
            {
              if(negative==true) insert_key=(-insert_key);
              if(c=='\n') fout<<" "<<insert_key<<endl;
              else fout<<" "<<insert_key<<",";
              red_black_tree.InsertRBT(insert_key);
              insert_key=0;
              negative=false;
              if(c=='\n') break;
            }
            else if(c=='-')
              negative=true;
            else
            {
              insert_key*=10;
              i=c-'0';
              insert_key+=i;
            }
          }
          if(turn==(number_of_works-1))
          {
            if(negative==true) insert_key=(-insert_key);
            fout<<" "<<insert_key<<endl;
            red_black_tree.InsertRBT(insert_key);
            insert_key=0;
            negative=false;
          }
            red_black_tree.Display_tree();
        }
        else
        {
          fout<<"Delete:";
          bool negative=false;
          int delete_key=0;
          while(fin.get(c))
          {
            if(c=='\n' || c==' ')
            {
              if(negative==true) delete_key=(-delete_key);
              if(c=='\n') fout<<" "<<delete_key<<endl;
              else fout<<" "<<delete_key<<",";
              red_black_tree.DeleteRBT(delete_key);
              delete_key=0;
              negative=false;
              if(c=='\n') break;
            }
            else if(c=='-')
              negative=true;
            else
            {
              delete_key*=10;
              i=c-'0';
              delete_key+=i;
            }
          }
          if(turn==(number_of_works-1))
          {
            if(negative==true) delete_key=(-delete_key);
            fout<<" "<<delete_key<<endl;
            red_black_tree.DeleteRBT(delete_key);
            delete_key=0;
            negative=false;
          }
            red_black_tree.Display_tree();
        }

      }
    }













    fin.close();
    fout.close();
    return 0;

}
