#include<iostream>
#include<string>
#include<fstream>

using namespace std;

struct ACCOUNT  // 이름, 계좌번호, 금액 구조체
{
	string name;
	int number;  // key
	int money;
};

template<class ItemType>
struct TreeNode
{
	ItemType info;  // struct(ACCOUNT)
	TreeNode<ItemType>* left;  // left_child
	TreeNode<ItemType>* right;  // right_child
};

template<class ItemType>
class BINARY_TREE
{
public:
	BINARY_TREE()
	{
		root = NULL;
	}

	~BINARY_TREE()
	{
		Destroy(root);
	}

	void Destroy(TreeNode<ItemType>*& tree)  // 재귀함수를 진행함으로써 Tree를 지워나감
	{
		if (tree != NULL){
			Destroy(tree->left);
			Destroy(tree->right);
			delete tree;
		}
	} 

	bool IsFull() const  // Tree가 Full인지 아닌지 판단
	{
		TreeNode<ItemType>* location;

		try
		{
			location = new TreeNode<ItemType>;
			delete location;
			return false;
		}
		catch(std::bad_alloc exception)
		{
			return true;
		}
	}

	bool IsEmpty() const  // Tree가 Empty인지 아닌지 판단
	{
		return root == NULL;
	}

	int CountNodes(TreeNode<ItemType>* tree)  // 재귀함수를 진행함으로써 Node의 갯수를 셈
	{
		if (tree == NULL)
			return 0;
		else 
			return CountNodes(tree->left) + CountNodes(tree->right) + 1;
	}

	int GetSize() const
	{
		return CountNodes(root);
	}

	void Retrieve(TreeNode<ItemType>* tree, ItemType& item, bool& found)  // key 값을 이용해 나머지 정보들을 찾음
	{
		if (tree == NULL)
			found = false;                     
		else if (item.number < tree->info.number)      
			Retrieve(tree->left, item, found);   
		else if (item.number > tree->info.number)
			Retrieve(tree->right, item, found);
		else
		{
			item = tree->info;              
			found = true;
		}
	}

	void GetItem(ItemType& item, bool& found)
	{
		Retrieve(root, item, found);
	}

	void Insert(TreeNode<ItemType>*& tree, ItemType item)  // key 값에 따라 Tree에 삽입함
	{
		if (tree == NULL){
			tree = new TreeNode<ItemType>;
			tree->right = NULL;
			tree->left = NULL;
			tree->info = item;
		}
		else if (item.number < tree->info.number)
			Insert(tree->left, item);    
		else
			Insert(tree->right, item);   
	}

	void InsertItem(ItemType item)
	{
		if( IsFull() ){
			cout << "Error: the tree is full." << endl;
			cout << endl;
			return;
		}

		Insert(root, item);
	}

	void GetPredecessor(TreeNode<ItemType>* tree, ItemType& data)  // key 값을 이용해 나머지 정보들을 찾음
	{
		while (tree->right != NULL)
			tree = tree->right;

		data = tree->info;
	}

	void DeleteNode(TreeNode<ItemType>*& tree)  // Node를 삭제함
	{
		ItemType data;
		TreeNode<ItemType>* tempPtr;
		tempPtr = tree;
		if (tree->left == NULL) {
			tree = tree->right;
			delete tempPtr; 
		}
		else if (tree->right == NULL){
			tree = tree->left;
			delete tempPtr;
		}
		else{
			GetPredecessor(tree->left, data);
			tree->info = data;
			Delete(tree->left, data);
		}
	} 

	void Delete(TreeNode<ItemType>*& tree, ItemType item)
	{
		if (item.number < tree->info.number)
			Delete(tree->left, item);   
		else if (item.number > tree->info.number)
			Delete(tree->right, item);  
		else
			DeleteNode(tree);
	}

	void DeleteItem(ItemType item)
	{
		Delete(root, item);
	}

	void CopyTree(TreeNode<ItemType>*& copy, const TreeNode<ItemType>* originalTree)  // Tree를 복사함
	{
		if (originalTree == NULL)
			copy = NULL;
		else
		{
			copy = new TreeNode<ItemType>;
			copy->info = originalTree->info;
			CopyTree(copy->left,  originalTree->left);
			CopyTree(copy->right, originalTree->right);
		}
	} 

	void SaveAccounts(TreeNode<ItemType>* tree, ItemType* arr, int& size)  // <new> Tree에 있는 정보들을 모두 배열에 저장 (오버로딩)
	{
		if(tree != NULL){
			SaveAccounts(tree->left, arr, size);
			SaveAccounts(tree->right, arr, size);
			arr[size] = tree->info;
			size++;
		}
	}

	void SaveAccounts(TreeNode<ItemType>* tree, ItemType item, ItemType* arr, int& size)  // <new> Tree에서 name 값이 같은 정보들만 배열에 저장 (오버로딩)
	{
		if(tree != NULL){
			SaveAccounts(tree->left, item, arr, size);
			SaveAccounts(tree->right, item, arr, size);
			
			if(tree->info.name == item.name){
				arr[size] = tree->info;
				size++;
			}		
		}
	}

	void OutputWithCleaning(ostream& out)  // Tree에 있는 정보들을 모두 정렬한 다음 출력함 (오버로딩)
	{
		int size=0;
		ItemType arr[100];
		SaveAccounts(root, arr, size);

		int i = 0;
		while(i < size-1){
			if(arr[i].number > arr[i+1].number){
				ItemType swap = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = swap;
				i = 0;
			}
			else
				i++;
		}

		for(int i=0; i<size; i++)
			out << arr[i].number << " "<< arr[i].name << " "<< arr[i].money << endl;

		out << endl;
	}

	void OutputWithCleaning(ItemType item, ostream& out)  // Tree에서 item과 정보가 같은 것들만 모은 후 정렬한 다음 출력함 (오버로딩)
	{
		int size=0;
		ItemType arr[100];
		SaveAccounts(root, item, arr, size);

		if(size == 0){
			cout << "Error: 존재하지 않는 이름" << endl;
			cout << endl;
			return;
		}

		int i = 0;
		while(i < size-1){
			if(arr[i].number > arr[i+1].number){
				ItemType swap = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = swap;
				i = 0;
			}
			else
				i++;
		}

		for(int i=0; i<size; i++)
			out << arr[i].number << " " << arr[i].money << endl;

		out << endl;
	}

	void OutputSum(ostream& out)  // Tree에 있는 정보들을 모두 모든 후 money의 합계를 출력함
	{
		int size=0;
		ItemType arr[100];
		SaveAccounts(root, arr, size);

		if(size == 0){
			cout << "Total money: 0" << endl;
			cout << endl;
			return;
		}

		int sum = 0;
		for(int i=0; i<size; i++)
			sum += arr[i].money;

		out << "Total money: " << sum << endl;
		out << endl;
	}
	

private:
	TreeNode<ItemType>* root;
};

int main(){

	string order;  // order: 명령어를 입력받기 위한 변수
	BINARY_TREE<ACCOUNT> Tree;  // Tree: class Tree

	ACCOUNT the_A;  // ths_A: struct account
	string the_name;  // the_name: 이름 값을 저장하기 위한 변수
	int the_number;  // the_number: 계좌번호 값을 저장하기 위한 변수
	int the_money;  // the_money: 금액 값을 저장하기 위한 변수
	bool founded;

	while(true){
		
		cout << "Input the order. (N, W, T, C, Q, R, S, P, L, F) : ";
		cin >> order;  // 명령어를 입력받음

		if(order == "F") break;

		if(order == "N"){
			cout << "N: 신규 계좌" << endl;
			cout << "Input the number, name, and money : ";
			cin >> the_A.number >> the_A.name >> the_A.money;  // 계좌번호, 이름, 금액을 입력받음

			Tree.GetItem(the_A, founded);

			if( founded )  // 이미 존재하는 계좌번호일 경우 error
			{
				cout << "Error: 기존에 있는 계좌번호를 신규계좌로 입력" << endl;
				cout << endl;
			}
			else
			{
				Tree.InsertItem(the_A);  // 입력받은 계정을 Tree에 삽입함
			}
		}
		else if(order == "W"){
			cout << "W: 인출" << endl;
			cout << "Input the number and money : ";
			cin >> the_A.number >> the_money;  // 계좌번호, 금액을 입력받음

			Tree.GetItem(the_A, founded);

			if( !founded )  // 존재하지 않는 계좌번호일 경우 error
			{
				cout << "Error: 존재하지 않는 계좌번호" << endl;
				cout << endl;
			}
			else
			{
				if(the_A.money < the_money)  // 잔액이 부족할 경우 error
				{
					cout << "Error: 인출금액이 잔액을 넘음" << endl;
					cout << endl;
				}
				else
				{
					Tree.DeleteItem(the_A);

					the_A.money = the_A.money - the_money;
					Tree.InsertItem(the_A);  // Tree에서 계정을 지운 다음 수정 후 다시 삽입
				}
			}
		}
		else if(order == "T"){
			ACCOUNT A2;
			cout << "T: 이체" << endl;
			cout << "Input the number1, number2 and money : ";
			cin >> the_A.number >> A2.number >> the_money;  // 계좌번호 2개, 금액을 입력받음

			bool founded2;

			Tree.GetItem(the_A, founded);
			Tree.GetItem(A2, founded2);

			if( !founded || !founded2 )  // 계좌번호 2개 중 하나라도 존재하지 않을 경우 error
			{
				cout << "Error: 존재하지 않는 계좌번호" << endl;
				cout << endl;
			}
			else
			{
				if(the_A.money < the_money)  // 잔액이 부족할 경우 error
				{
					cout << "Error: 이체금액이 잔액을 넘음" << endl;
					cout << endl;
				}
				else
				{
					Tree.DeleteItem(the_A);
					Tree.DeleteItem(A2);

					the_A.money = the_A.money - the_money;
					Tree.InsertItem(the_A);  // Tree에서 계정을 지운 다음 수정 후 다시 삽입(보내는 계좌)

					A2.money = A2.money + the_money;
					Tree.InsertItem(A2);  // Tree에서 계정을 지운 다음 수정 후 다시 삽입(받는 계좌)
				}
			}
		}
		else if(order == "C"){
			cout << "C: 계좌 해지" << endl;
			cout << "Input the number : ";
			cin >> the_A.number;  // 계좌번호를 입력받음

			Tree.GetItem(the_A, founded);

			if( !founded )  // 존재하지 않는 계좌번호일 경우 error
			{
				cout << "Error: 존재하지 않는 계좌번호" << endl;
				cout << endl;
			}
			else
			{
				Tree.DeleteItem(the_A);  // 해당 계정를 삭제함
			}
		}
		else if(order == "Q"){
			cout << "Q: 조회" << endl;
			cout << "Input the number : ";
			cin >> the_A.number;  // 계좌번호를 입력받음

			Tree.GetItem(the_A, founded);

			if( !founded )  // 존재하지 않는 계좌번호일 경우 error
			{
				cout << "Error: 존재하지 않는 계좌번호" << endl;
				cout << endl;
			}
			else
			{
				cout << the_A.number << " " << the_A.name << " "<< the_A.money << endl;  // 해당 계정를 가져온 후 출력
			}
		}
		else if(order == "R"){
			cout << "R: 예금주이름의 모든 계좌번호와 잔액을 출력" << endl;
			cout << "Input the name : ";
			cin >> the_A.name;  // 이름을 입력받음

			Tree.OutputWithCleaning(the_A, cout);  // 해당 이름의 계정들을 모두 출력
		}
		else if(order == "S"){
			cout << "S: 입금" << endl;
			cout << "Input the number and money : ";
			cin >> the_A.number >> the_money;  // 계좌번호, 금액을 입력받음

			Tree.GetItem(the_A, founded);

			if( !founded )  // 존재하지 않는 계좌번호일 경우 error
			{
				cout << "Error: 존재하지 않는 계좌번호" << endl;
				cout << endl;
			}
			else
			{
				Tree.DeleteItem(the_A);

				the_A.money = the_A.money + the_money;
				Tree.InsertItem(the_A);  // Tree에서 계정을 지운 다음 수정 후 다시 삽입
			}
		}
		else if(order == "P"){
			Tree.OutputSum(cout);  // money의 합계를 출력함
		}
		else if(order == "L"){
			Tree.OutputWithCleaning(cout);  // Tree 전체를 출력함
		}
		else  // 유효하지 않은 명령어 일 경우 error
		{
			cout << "Error: 존재하지 않는 명령어" << endl;
			cout << endl;
		}

		cout << endl;

	}

	cout << endl;
	cout << "Thanks for using this program!" << endl;
	cout << "Please refer to the text file~" << endl;
	cout << endl;

	ofstream fout;  // fout: ofstream
	fout.open("bank.txt");
	Tree.OutputWithCleaning(fout);  // bank.text에 정보를 저장함

	return 0;
 }