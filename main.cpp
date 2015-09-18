// Implementation of a self-balancing binary search tree (AVL).
#include <fstream>
#include <iostream>
#include <string>

template < typename T, typename V>
struct TreeNode {
	TreeNode<T,V>* left;
	TreeNode<T,V>* right;
	TreeNode<T,V>* parent;
	T key;
	V value;
};

template < typename T, typename V>
class AVL {
		public:
			AVL();
			~AVL();
			void insert(T, V);
			V lookup(TreeNode<T,V>* N, T);
			void print(TreeNode<T,V>* N);
			TreeNode<T,V>* Delete(TreeNode<T,V>* N, T);
			TreeNode<T,V>* root;
		private:
			int height(TreeNode<T,V>* Node);
			void leftRotate(TreeNode<T,V>*& Node);
			void rightRotate(TreeNode<T,V>*& Node);
			void leftLeftRotate(TreeNode<T,V>*& Node);
			void rightRightRotate(TreeNode<T,V>*& Node);
};

template < typename T, typename V>
int AVL<T,V>::height(TreeNode<T,V>* node){
	if (node == NULL)
		return 0;
	else
		return std::max(height(node->left), height(node->right)) + 1;
}

// Used to do a right rotation (meaning it's on a left case)
template < typename T, typename V>
void AVL<T,V>::leftRotate(TreeNode<T,V>*& node){
	TreeNode<T,V>* rotatedNode;
	rotatedNode = node->left;
	node->left = rotatedNode->right;
	rotatedNode->right = node;
	node = rotatedNode;
}

// Used to do a left rotation (meaning it's on a right case)
template < typename T, typename V>
void AVL<T,V>::rightRotate(TreeNode<T,V>*& node){ 
	TreeNode<T,V>* rotatedNode;
	rotatedNode = node->right;
	node->right = rotatedNode->left;
	rotatedNode->left = node;
	node = rotatedNode;
}

template < typename T, typename V>
void AVL<T,V>::leftLeftRotate(TreeNode<T,V>*& node){
	rightRotate(node->left);
	leftRotate(node);
}

template < typename T, typename V>
void AVL<T,V>::rightRightRotate(TreeNode<T,V>*& node){
	leftRotate(node->right);
	rightRotate(node);
}

template < typename T, typename V>
AVL<T,V>::AVL(){
	root = NULL;
};

template < typename T, typename V>
AVL<T,V>::~AVL(){
	if (root != NULL)
	{
		delete root->left;
		delete root->right;
	}
};

template < typename T, typename V>
void AVL<T,V>::insert(T Key, V Value){
	
	// I created a new node to add to the AVL tree
	// and populated it with the Key/Value pair
	TreeNode<T,V>* newNode = new TreeNode<T,V>;
	TreeNode<T,V>* parentNode;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = Key;
	newNode->value = Value;
	parentNode = NULL;
	
	
	if (root == NULL) // If the AVL tree is empty, create a new start node.
		root = newNode;
	else  // Otherwise, run through the tree and add it to the appropriate end.
	{
		TreeNode<T,V>* currentNode;
		currentNode = root;
		while(currentNode != NULL) // While the end of the AVL tree hasn't been reached
		{
			parentNode = currentNode;
			if (newNode->key < currentNode->key) // If the key being added is less then the current key, go left
				currentNode = currentNode->left; // Then set the current node to the next left node
			else  // Otherwise go Right
				currentNode = currentNode->right;				
		}
		
		if (newNode->key < parentNode->key) // Once the end is reached,
		{
			parentNode->left = newNode; // if the new key is less then it's parents key add it to the left
			if(height(root->left) - height(root->right) > 1) // if the height of the left side is greater then 1
			{
				if(height(root->left->right) - height(root->left->left) > 1)
					leftLeftRotate(root); // do either the left right case 
				else
					leftRotate(root); // or the left-left case
			}
		}
		else
		{
			parentNode->right = newNode;
			if(height(root->right) - height(root->left) > 1)
			{
				if(height(root->right->left) - height(root->right->right) > 1)
					rightRightRotate(root);
				else
					rightRotate(root);
			}
		}
		
		// I used this to make sure the tree was properly balanced
		// When included the height of both the right and left sides of the AVL tree were 30
		// std::cout << height(root->left) << ", " << height(root->right) << std::endl;
	}
}

template < typename T, typename V>
V AVL<T,V>::lookup(TreeNode<T,V>* N, T Key){
	if(Key == N->key)
		return N->value;
	else if (Key < N->key)
	{
		if (N->left == NULL)
			return static_cast<V>(NULL);
		else
			return lookup(N->left, Key);
	}
	else if (Key > N->key)
	{
		if (N->right == NULL)
			return static_cast<V>(NULL);
		else
			return lookup(N->right, Key);
	}
	return static_cast<V>(NULL);
}

//Non-working delete function :(
template < typename T, typename V>
TreeNode<T,V>* AVL<T,V>::Delete(TreeNode<T,V>* N, T Key){
	if (Key < N->key)
	{
		if (N->left != NULL)
			return Delete(N->left, Key);
		else
			return NULL;
	}
	else if(Key > N->key)
	{
		if (N->right != NULL)
			return Delete(N->right, Key);
		else
			return NULL;
	}
	else if(Key == N->key)
	{
		if (N->left == NULL and N->right == NULL)
		{
			N = NULL;
			return NULL;
		}
		else if (N->left == NULL)
		{
			N = N->right;
			return NULL;
		}
		else if (N->right == NULL)
		{
			N = N->left;
			return NULL;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

template < typename T, typename V>
void AVL<T,V>::print(TreeNode<T,V>* N){
	if (N != NULL)
	{
		print(N->left);
		std::cout << N->key << ", " << N->value << std::endl;
		print(N->right);
	}
}


//I used a .txt filled with employees names and ID numbers to create the AVL binary tree.
int main() 
{
    std::string file_name = "data/employees.txt";
    std::ifstream in_file(file_name);
    
    AVL <std::string, std::string> EmployeeData;
    
    //Parse the employee names and idss out of the text file
    std::string line;
    while(std::getline(in_file,line)) 
    {
        // Gets the employees name
        std::string employee_name = line.substr(0,line.find(","));
        // Gets the employees id number
        std::string employee_id = line.substr(line.find(",")+1);
        EmployeeData.insert(employee_name, employee_id);
    }
    //EmployeeData.Delete(EmployeeData.root, "Zyon Newman");
    EmployeeData.print(EmployeeData.root);
    std::cout << "Testing Lookup" << std::endl;
    std::cout << "Zyler Whitney: " << EmployeeData.lookup(EmployeeData.root, "Zyler Whitney") << std::endl;

    return 0;
}
