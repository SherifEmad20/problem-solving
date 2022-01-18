#include <iostream>
#include <algorithm>

using namespace std;

static int indexCounter = 0;

typedef struct node
{
	int key;
	struct node *left;
	struct node *right;
	int left_child;
	int right_child;
	int height;
};

int maximum (int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int height(node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

node *new_node (int key, int left_child, int right_child)
{
	node *tmp_node = (node *) malloc( sizeof(node) );
	tmp_node -> key = key;
	tmp_node -> left = NULL;
	tmp_node -> right = NULL;
	tmp_node -> height = 1;
	tmp_node -> left_child = left_child;
	tmp_node -> right_child = right_child;
	indexCounter++;
	return tmp_node;
}

node *right_rotate (node *y)
{
	node *x = y -> left;
	node *T2 = x -> right;

	x -> right = y;
	y -> left = T2;

	y -> height = maximum( height(y -> left), height(y -> right) ) + 1;
	x -> height = maximum( height(x -> left), height(x -> right) ) + 1;

	if (T2 != NULL)
        y -> left_child = T2 -> left_child + T2 -> right_child + 1;
	else
        y -> left_child = 0;
	x -> right_child = y -> left_child + y -> right_child + 1;

	return x;
}

node *left_rotate (node *x)
{
	node *y = x -> right;
	node *T2 = y -> left;

	y -> left = x;
	x -> right = T2;

	x -> height = maximum( height(x -> left), height(x -> right) ) + 1;
	y -> height = maximum( height(y -> left), height(y -> right) ) + 1;

	if (T2 != NULL)
        x -> right_child = T2 -> left_child + T2 -> right_child + 1;
    else
        x -> right_child = 0;
	y -> left_child = x -> left_child + x -> right_child + 1;

	return y;
}

int get_balance_factor (node *root)
{
	if (root == NULL)
        return 0;
	else
        return height(root -> left) - height(root -> right);
}

node *insertBST(node *root, int key)
{
	if (root == NULL)
        return new_node(key, 0, 0);

	if (key < root -> key)
	{
		root -> left = insertBST(root -> left, key);
		root -> left_child++;
	}

	else if (key > root -> key)
	{
		root -> right = insertBST(root -> right, key);
		root -> right_child++;
	}
	else
        return root;

	root -> height = maximum( height(root -> left), height(root -> right) ) + 1;

	int balance = get_balance_factor(root);

	if ( (balance > 1) && (key < root -> left -> key) )
        return right_rotate(root);

	if ( (balance < -1) && (key > root -> right -> key) )
        return left_rotate(root);

	if ( (balance > 1) && (key > root -> left -> key) )
    {
		root -> left = left_rotate(root -> left);
		return right_rotate(root);
	}

	if ( (balance < -1) && (key < root -> right -> key) )
    {
		root -> right = right_rotate(root -> right);
		return left_rotate(root);
	}

	return root;
}

bool flag;
int find_index_by_key(node *root, int key)
{
	if (root != NULL)
    {
		if (key > root -> key)
            return root -> left_child + 1 + find_index_by_key(root -> right, key);
		else if (key < root -> key)
            return find_index_by_key(root -> left, key);
		else
            return root -> left_child;
	}
	else
    {
		flag = true;
		return -1;
	}
}

bool flag2;
int find_key_by_index(node *root, int index)
{
    node *traversePtr = root;

    if (traversePtr != nullptr)
    {
        // root
        if (index == traversePtr -> height)
        {
            return traversePtr -> key;
        }

        // right
        else if (index > traversePtr -> height)
        {
            if (index > indexCounter)
            {
                flag2 = true;
                return -1;
            }
            else
                return traversePtr->right->key;
        }

        // left
        else if (index < traversePtr -> height)
        {
            return traversePtr->left->key;
        }
    }
    else
    {
        flag2 = true;
        return -1;
    }

}

node * min_value_node(node* Node)
{
    node* current = Node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

node* delete_by_key(node* root, int key)
{
    if (root == NULL)
        return root;

    if ( key < root->key )
        root->left = delete_by_key(root->left, key);

    else if( key > root->key )
        root->right = delete_by_key(root->right, key);
    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            node *temp = root->left ?
                         root->left :
                         root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            *root = *temp;
            free(temp);
        }
        else
        {
            node* temp = min_value_node(root->right);
            root->key = temp->key;
            root->right = delete_by_key(root->right, temp->key);
        }
    }

    if (root == NULL)
    return root;

    root->height = 1 + maximum(height(root->left), height(root->right));

    int balance = get_balance_factor(root);

    if (balance > 1 && get_balance_factor(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && get_balance_factor(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance_factor(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && get_balance_factor(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

int main ()
{
	int input, element, index, key = 0;
	node *root = NULL;

	cout << "<<INPUTS>>\n";

	while (input != -1)
    {
		cin >> input;

		// insert elements into tree
		if (input == 1)
		{
            cin >> element;
			root = insertBST(root, element);
		}

		// delete elements from tree
        else if (input == 2)
		{
            cin >> element;
		    root = delete_by_key(root, element);
		}

        // finds the index of a given element
		else if (input == 3)
		{
            cin >> element;
			flag = false;
			index = find_index_by_key(root, element) + 1;
			if (flag)
                cout << "-1" << endl;
            else
                cout << index << endl;
		}

        // find element with its index
		else if (input == 4)
        {
            cin >> element;
            flag2 = false;
            key = find_key_by_index(root,element);
            if (flag2)
                cout << "-1" << endl;
            else
                cout << key << endl;
        }

	}
	return 0;
}
