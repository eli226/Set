#include "Containers/List.h"

List::List(MemoryManager& mem) : AbstractList(mem)
{
	head = NULL;
	global_elems = 0;
	global_size = mem.size();
}

List::~List() { clear(); }

void* List::front(size_t& size)
{
	if (head)
	{
		size = (size_t)head->size;
		return head->data;
	}
	return NULL;
}

void List::pop_front()
{
	if (head)
	{
		node* current = head;
		head = current->next;
		global_size += (sizeof(node) + current->size);
		_memory.freeMem(current->data);
		_memory.freeMem(current);
		global_elems--;
	}
	else std::cout << "mistake in pop!!!" << std::endl;
}

int List::insert(Iterator* iter, void* elem, size_t elemSize)
{
	if (iter == NULL || elem == NULL || elemSize <= 0) return 1;
	node* current = head;
	List::Iterator_list* liter = (Iterator_list*)iter;
	node* new_node = (node*)_memory.allocMem(sizeof(node));
	if (new_node == NULL) return 1;
	if (current == liter->curr)
	{
		new_node->size = elemSize;
		new_node->data = _memory.allocMem(elemSize);
		if (new_node->data == NULL)
		{
			_memory.freeMem(new_node);
			return 1;
		}
		memcpy(new_node->data, elem, elemSize);
		new_node->next = head;
		head = new_node;
		global_elems++;
		global_size = global_size - (sizeof(node) + elemSize);
		
		return 0;
	}
	while (current && current->next != liter->curr)
		current = current->next;
	if (!current)
	{
		_memory.freeMem(new_node);
		return 1;
	}
	if (new_node && current)
	{
		new_node->size = elemSize;
		new_node->data = _memory.allocMem(elemSize);
		memcpy(new_node->data, elem, elemSize);
		new_node->next = liter->curr;
		current->next = new_node;
		global_elems++;
		global_size = global_size - (sizeof(node) + elemSize);
		
		return 0;
	}
	
	return 1;
}

int List::push_front(void* elem, size_t elemSize)
{
	if (!elem || elemSize <= 0) return 1;
	node* new_node = (node*)_memory.allocMem(sizeof(node));
	if (new_node)
	{
		new_node->size = elemSize;
		void* data = _memory.allocMem(elemSize);
		memcpy(data, elem, elemSize);
		new_node->data = data;
		new_node->next = head;
		head = new_node;
		global_elems++;
		global_size = global_size - (sizeof(node) + elemSize);
		return 0;
	}
	return 1;
}

void* List::Iterator_list::getElement(size_t& size)
{
	if (curr) 
	{
		size = curr->size;  
		return curr->data;
	}
	return NULL;
}

bool List::Iterator_list::hasNext()
{
	if (curr && curr->next)
	{
		return true;
	}
	return false;
}

void List::Iterator_list::goToNext()
{
	if (hasNext()) {
		prev = curr;
		curr = curr->next;
	}
}

bool List::Iterator_list::equals(Iterator* right)
{
	if (!right) return false;
	return ((Iterator_list*)right)->curr == curr;
}

int List::size()
{
	return global_elems;
}

size_t List::max_bytes()
{
	return global_size;
}

Container::Iterator* List::find(void* elem, size_t size)
{
	Iterator_list* object = new Iterator_list(head); 
	node* prev = NULL;
	node* current = head;
	while (current != NULL)
	{
		if (memcmp(current->data, elem, size) == 0 && current->size == size)
		{
			break;
		}
		prev = current;
		current = current->next;
	}
	if (current == NULL)
	{
		delete(object);
		return NULL;
	}
	object->prev = prev;
	object->curr = current;
	return object;
}

Container::Iterator* List::newIterator()
{
	Iterator_list* object = new Iterator_list(head);
	if (object == NULL) return NULL;
	object->prev = NULL;
	object->curr = head; 
	return object;
}

void List::remove(Iterator* iter)
{
	if (iter == NULL || head == NULL) return;
	List::Iterator_list* current = (Iterator_list*)iter; 
	node* now = head;
	if (now == current->curr)
	{
		current->curr = now->next;
		pop_front();
		return;
	}
	if (current->prev == NULL) return; 
	
	
	if (current->curr) 
	{
		if (current->curr->next == NULL)
		{
			current->prev->next = NULL;
			global_size += (current->curr->size + sizeof(node));
			_memory.freeMem(current->curr->data);
			_memory.freeMem(current->curr);
			current->curr = NULL;
			global_elems--;
			return;

		}
		current->prev->next = current->curr->next;
		global_size += (current->curr->size + sizeof(node)); 
		_memory.freeMem(current->curr->data );
		_memory.freeMem(current->curr); 
		current->curr = current->prev->next;
		global_elems--;  
	}

}
void List::clear()
{
	if (head)
	{
		while (head)
		{
			pop_front();
		}
	}
}
bool List::empty()
{
	if (global_elems != 0) return false;
	return true;
}