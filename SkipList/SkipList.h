#pragma once

#include<time.h>
#include<stdlib.h>
#include<string>

#define MAXLEVEL 16

template<class T>
class Node
{
public:
	Node(T val) :value(val) 
	{
		maxLevel = 0;
		
		for (int i = 0; i < MAXLEVEL; i++)
			next[i] = nullptr;
	}

	T		 value;
	int		 maxLevel;
	Node<T>* next[MAXLEVEL];
};

template<class T>
class SkipList
{
public:
	SkipList()
	{
		m_pHead = new Node<T>(-1);
		m_nLevelCount = 1;
		srand(int(time(0)));
	}

	int  Insert(const T& value)
	{
		Node<T>* update[MAXLEVEL] = { nullptr };
		Node<T>* p = m_pHead;
		int randLevel = RandomLevel();

		Node<T>* pNewNode = new (std::nothrow)Node<T>(value);
		if (pNewNode == nullptr)
		{
			return -1;
		}

		for (int i = MAXLEVEL - 1; i >= 0; i--)
		{
			while (p->next[i] != nullptr && p->next[i]->value < value)
				p = p->next[i];

			if (i <= randLevel - 1)
			{
				update[i] = p;
			}
		}
		
		for (int i = randLevel - 1; i >= 0; i--)
		{
			pNewNode->next[i] = update[i]->next[i];
			update[i]->next[i] = pNewNode;
		}

		if (m_nLevelCount < randLevel)
			m_nLevelCount = randLevel;

		m_nCount++;

		return 1;
	}

	void  Delete(const T& value)
	{
		Node<T>* p = m_pHead;
		Node<T>* update[MAXLEVEL] = { nullptr };

		for (int i = m_nLevelCount - 1; i >= 0; i--)
		{
			while (p->next[i] != nullptr && p->next[i]->value < value)
				p = p->next[i];

			update[i] = p;
		}

		Node<T>* pDel = nullptr;

		if (p->next[0] != nullptr && p->next[0]->value == value)
		{
			for (int i = m_nLevelCount - 1; i >= 0; i--)
			{
				if (update[i]->next[i] != nullptr && update[i]->next[i]->value == value)
				{
					pDel = update[i]->next[i];
					update[i]->next[i] = update[i]->next[i]->next[i];

					if (i == 0)
					{
						m_nCount--;
						delete pDel;
					}
				}
			}
		}
	}

	Node<T>* Find(const T& value) 
	{
		Node<T>* p = m_pHead;

		for (int i = m_nLevelCount - 1; i > 0 ; i--)
		{
			while (p->next[i] != nullptr && p->next[i]->value < value)
				p = p->next[i];
		}

		if (p->next[0] != nullptr && p->next[0]->value == value)
			return p->next[0];

		return nullptr;
	}

	int RandomLevel()
	{
		int level = 1;

		for (int i = 1; i < MAXLEVEL; i++)
		{
			if (rand() % 2 == 1)
			{
				level++;
			}
		}

		return level;
	}

	void Clear()
	{
		Node<T>* p = m_pHead->next[0], *pNextDel = nullptr;

		while (p != nullptr)
		{
			pNextDel = p->next[0];
			delete p;
			p = pNextDel;
		}
		
		m_nLevelCount = 1;
		m_nCount = 0;

		for (int i = 0; i < MAXLEVEL; i++)
			m_pHead->next[i] = nullptr;
	}

	int        GetCount() { return m_nCount; }
	Node<T>*   GetHead() { return m_pHead; }

private:
	int			m_nLevelCount;
	int			m_nCount;
	Node<T>*	m_pHead;
	
};