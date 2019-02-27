#include <iostream>
#include <vector>
#include <fstream>
#include <ctype.h>

using namespace std;
#define black 0
#define red 1

class node{
public:
    string english;
    string chinese;
    int color;
    node* left= nullptr;
    node* right= nullptr;
    node* father= nullptr;
    bool operator==(const node& lhs){
        return this->english == lhs.english && this->chinese == lhs.chinese && this->right == lhs.right && this->left == lhs.left && this->color == lhs.color && this->father == lhs.father;
    }
};

bool _isOdd(int i){
    return i % 2 == 1;
}

bool _isEven(int i){
    return i%2==0;
}

vector<node> read_in(ifstream& in){
    string line;
    node Node;
    vector<node> temp;
    int i=1;
    while(getline(in,line)){
        if(_isOdd(i))
            Node.english=line;
        else{
            Node.chinese=line;
            temp.push_back(Node);
        }
        i++;
    }
    return temp;
}

class rb_tree{
private:
    node* null_node;
    node* root_node;
protected:
    void _tree_walk(node* root_node);
public:
    rb_tree();
    ~rb_tree();

    string _insert(string english,string chinese);
    void _insert_fixup(node* temp);
    void _left_rotate(node* temp);
    void _right_rotate(node* temp);
    node* _tree_successor(node* temp);
    string _put(string english,string chinese);
    node* _get(string english);
    string _delete(string english);
    void _delete_fixup(node* temp);
    string load(string filename);
    void dump();
};

rb_tree::rb_tree() {
    this->null_node=new node;
    this->root_node=null_node;
    this->null_node->left=root_node;
    this->null_node->right=root_node;
    this->null_node->father=root_node;
    this->null_node->color=black;
}

string rb_tree::load(string filename) {
    ifstream in(
//            "//Users//Aliez//CLionProjects//Rb-Tree//"+
                        filename);
    vector<node> nodes=read_in(in);
    for(int i=0;i<nodes.size();i++)
        _insert(nodes[i].english,nodes[i].chinese);
    return "complete";
}

node* rb_tree::_get(string english) {
    node* temp=root_node;
    while(temp!=null_node&&temp->english!=english){
        if(english<temp->english)
            temp=temp->left;
        else
            temp=temp->right;
    }
    if(temp== null_node)
        cout<<"not found";
    return temp;
}

string rb_tree::_put(string english, string chinese) {
    if(_get(english)->chinese!="")
        _get(english)->chinese=chinese;
    else
        _insert(english,chinese);
    return "success";
}

void rb_tree::_tree_walk(node *root_node) {
    if(root_node!=null_node) {
        _tree_walk(root_node->left);
        cout << root_node->english << endl << root_node->chinese << endl;
        _tree_walk(root_node->right);
    }
}

void rb_tree::dump() {
    node* start=root_node;
    _tree_walk(start);
}

string rb_tree::_insert(string english, string chinese) {
    if(_get(english)->chinese!="")
        return "key conflict";
    node* temp_1=null_node;
    node* temp_2=root_node;
    node* new_node=new node;
    new_node->english=english;
    new_node->chinese=chinese;
    new_node->color=red;
    while(temp_2!=null_node){
        temp_1=temp_2;
        if(new_node->english<temp_2->english)
            temp_2=temp_2->left;
        else
            temp_2=temp_2->right;
    }
    new_node->father=temp_1;
    if(temp_1==null_node)
        root_node=new_node;
    else if(new_node->english<temp_1->english)
        temp_1->left=new_node;
    else
        temp_1->right=new_node;
    new_node->left=null_node;
    new_node->right=null_node;
    _insert_fixup(new_node);
    new_node= nullptr;
    return "success";
}

void rb_tree::_insert_fixup(node* temp) {
    node* temp_1;
    while(temp->father->color==red){
        if(temp->father==temp->father->father->left){
            temp_1=temp->father->father->right;
            if(temp_1->color==red){
                temp->father->color=black;
                temp_1->color=black;
                temp->father->father->color=red;
                temp=temp->father->father;
            }
            else {
                if (temp == temp->father->right) {
                    temp = temp->father;
                    _left_rotate(temp);
                }
                temp->father->color = black;
                temp->father->father->color = red;
                _right_rotate(temp->father->father);
            }
        }
        else{
            temp_1=temp->father->father->left;
            if(temp_1->color==red){
                temp->father->color=black;
                temp_1->color=black;
                temp->father->father->color=red;
                temp=temp->father->father;
            }
            else {
                if (temp == temp->father->left) {
                    temp = temp->father;
                    _right_rotate(temp);
                }
                temp->father->color = black;
                temp->father->father->color = red;
                _left_rotate(temp->father->father);
            }
        }
    }
    root_node->color=black;
}

void rb_tree::_left_rotate(node* temp) {
    node* temp_1=temp->right;
    temp->right=temp_1->left;
    if(temp_1->left!=null_node)
        temp_1->left->father=temp;
    temp_1->father=temp->father;
    if(temp->father==null_node)
        root_node=temp_1;
    else if(temp->father->left==temp)
        temp->father->left=temp_1;
    else
        temp->father->right=temp_1;
    temp_1->left=temp;
    temp->father=temp_1;
}

void rb_tree::_right_rotate(node* temp) {
    node* temp_1=temp->left;
    temp->left=temp_1->right;
    if(temp_1->right!=null_node)
        temp_1->right->father=temp;
    temp_1->father=temp->father;
    if(temp->father==null_node)
        root_node=temp_1;
    else if(temp->father->left==temp)
        temp->father->left=temp_1;
    else
        temp->father->right=temp_1;
    temp_1->right=temp;
    temp->father=temp_1;
}

string rb_tree::_delete(string english) {
    if(_get(english)->chinese=="")
        return "key missing";
    node* new_node=_get(english);
    node* temp_1;
    node* temp_2;
    if(new_node->left==null_node||new_node->right==null_node)
        temp_1=new_node;
    else
        temp_1=_tree_successor(new_node);
    if(temp_1->left!=null_node)
        temp_2=temp_1->left;
    else
        temp_2=temp_1->right;
    temp_2->father=temp_1->father;
    if(temp_1->father==null_node)
        root_node=temp_2;
    else if(temp_1==temp_1->father->left)
        temp_1->father->left=temp_2;
    else
        temp_1->father->right=temp_2;
    if(temp_1!=new_node)
        new_node->english=temp_1->english;
    if(temp_1->color==black)
        _delete_fixup(temp_2);
    return "success";
}

node* rb_tree::_tree_successor(node *temp) {
    node* temp_1;
    if(temp->right!=null_node) {
        while (temp->right->left != null_node)
            temp_1 = temp->right->left;
        return temp_1;
    }
    temp_1=temp->father;
    while(temp_1!=null_node&&temp==temp_1->right){
        temp=temp_1;
        temp_1=temp->father;
    }
    return temp_1;
}

void rb_tree::_delete_fixup(node *temp) {
    while(temp!=root_node&&temp->color==black){
        if(temp==temp->father->left){
            node* temp_1=temp->father->right;
            if(temp_1->color==red){
                temp_1->color=black;
                temp->father->color=red;
                _left_rotate(temp->father);
                temp_1=temp->father->right;
            }
            if(temp_1->left->color==black&&temp_1->right->color==black){
                temp_1->color=red;
                temp=temp->father;
            }
            else if(temp_1->right->color==black){
                temp_1->left->color=black;
                temp_1->color=red;
                _right_rotate(temp_1);
                temp_1=temp->father->right;
            }
            temp_1->color=temp->father->color;
            temp->father->color=black;
            temp_1->right->color=black;
            _left_rotate(temp->father);
            temp=root_node;
        }
        else{
            node* temp_1=temp->father->left;
            if(temp_1->color==red){
                temp_1->color=black;
                temp->father->color=red;
                _right_rotate(temp->father);
                temp_1=temp->father->left;
            }
            if(temp_1->right->color==black&&temp_1->left->color==black){
                temp_1->color=red;
                temp=temp->father;
            }
            else if(temp_1->left->color==black){
                temp_1->right->color=black;
                temp_1->color=red;
                _left_rotate(temp_1);
                temp_1=temp->father->left;
            }
            temp_1->color=temp->father->color;
            temp->father->color=black;
            temp_1->left->color=black;
            _right_rotate(temp->father);
            temp=root_node;
        }
    }
    temp->color=black;
}

rb_tree::~rb_tree() {
    root_node->english.clear();
    root_node->chinese.clear();
    root_node->color=0;
    root_node->left= nullptr;
    root_node->right= nullptr;
    root_node->father= nullptr;
    delete null_node;
}

int main() {
    string filename;
    cout<<"请输入检查文本的绝对路径（注意使用双斜杠）"<<endl;
    cin>>filename;
    ifstream in(filename);
    string line;
    rb_tree new_tree;

    while(getline(in,line)){
        vector<string> t;
        int i=0;
        for(int j=0;j<line.size();j++){
            if(line[j] == ' ') {
                t.push_back(line.substr(i, j-i));
                i = j+1;
            }
            if(j==line.size()-1)
                t.push_back(line.substr(i, j-i+1));
        }
        if(t[0]=="LOAD")
            cout<<new_tree.load(t[1])<<endl;
        else if(t[0]=="INSERT")
            cout<<new_tree._insert(t[1],t[2])<<endl;
        else if(t[0]=="PUT")
            cout<<new_tree._put(t[1],t[2])<<endl;
        else if(t[0]=="GET")
            cout<<new_tree._get(t[1])->chinese<<endl;
        else if(t[0]=="DELETE")
            cout<<new_tree._delete(t[1])<<endl;
        else if(t[0]=="DUMP")
            new_tree.dump();
        t.clear();
    }

    return 0;
}