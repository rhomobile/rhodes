#ifndef __clist
#define __clist


#include <stdlib.h>

// ------------------------------------------------------------------

class xBounds {};
class xAlloc {};

// ------------------------------------------------------------------

template <class T>
class CListNode
	{
	private:
		CListNode* Next;
		CListNode* Prev;
		T* Object;

	public:
		CListNode(T* ob): Next(NULL), Prev(NULL), Object(ob) {}
		~CListNode() {if (Object) delete Object;}
		CListNode* GetNext() {return Next;}
		CListNode* GetPrev() {return Prev;}
		void SetNext(CListNode* next) {Next = next;}
		void SetPrev(CListNode* prev) {Prev = prev;}
		T* GetObjectP() {return Object;}
	};

// ------------------------------------------------------------------

template <class T>
class CList
	{
	private:
		unsigned int NodeCt;
		CListNode<T>* First;
		CListNode<T>* Last;
		CListNode<T>* GetNodePtr(unsigned int offset);

	public:
		CList(): First(NULL), Last(NULL), NodeCt(0) {}
		~CList();
		void Add(T* ob);
		void Remove(unsigned int offset);
		void RemoveAll(void);
		unsigned int Count() {return NodeCt;}
		T* GetObPtr(unsigned int offset) {return GetNodePtr(offset)->GetObjectP();}
		T* operator [](unsigned int offset) {return GetNodePtr(offset)->GetObjectP();}
	};

// ------------------------------------------------------------------

template <class T>
CList<T>::~CList()
	{
	RemoveAll();
	}

// ------------------------------------------------------------------

template <class T>
CListNode<T>* CList<T>::GetNodePtr(unsigned int offset)
	{
	if (offset >= NodeCt)
		throw xBounds();

	CListNode<T>* pnode = First;

	for (unsigned int ct=0; ct<offset; ct++)
		pnode = pnode->GetNext();

	return pnode;
	}

// ------------------------------------------------------------------

template <class T>
void CList<T>::Add(T* ob)
	{
	CListNode<T>* pnode = new CListNode<T>(ob);

	if (!pnode) throw xAlloc();

	if (First == NULL)
		{
		First = pnode;
		Last = pnode;
		pnode->SetPrev(NULL);
		pnode->SetNext(NULL);
		}
	else
		{
		Last->SetNext(pnode);
		pnode->SetPrev(Last);
		Last = pnode;
		}

	NodeCt++;
	}

// ------------------------------------------------------------------

template <class T>
void CList<T>::Remove(unsigned int offset)
	{
	CListNode<T>* pnode;

	if ((pnode = GetNodePtr(offset)) != NULL)
		{
		if (First == Last)			// There is only one node
			{
			delete pnode;
			First = Last = NULL;
			}
		else if (pnode == First)	// Its the first node
			{
			First = First->GetNext();
			First->SetPrev(NULL);
			delete pnode;
			}
		else if (pnode == Last)		// Its the last node
			{
			Last = Last->GetPrev();
			Last->SetNext(NULL);
			delete pnode;
			}
		else								// Its somewhere in the middle
			{
			pnode->GetNext()->SetPrev(pnode->GetPrev());
			pnode->GetPrev()->SetNext(pnode->GetNext());
			delete pnode;
			}

		NodeCt--;
		}
	else
		throw xBounds();
	}

// ------------------------------------------------------------------

template <class T>
void CList<T>::RemoveAll(void)
	{
	while(NodeCt)
		Remove(0);
	}


#endif