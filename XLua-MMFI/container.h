#ifndef CONTAINER_H_
#define CONTAINER_H_

class SLList {
public:
	
	class SLNode {
	public:

		SLNode (void* d, SLNode* n) : data(d), next(n) { }

		SLNode*	next;
		void*	data;
	};

	SLList () : head(0) { }

	~SLList () {
		while (head) {
			removeFirst();
		}
	}

	SLNode* first () {
		return head;
	}

	void add (void* data) {
		head = new SLNode(data, head);
	}

	void removeFirst () {
		SLNode* oldHead = head;
		if (head)
			head = head->next;

		delete oldHead;
	}

	void removeNext (SLNode* node) {
		SLNode* next = node->next;
		if (next)
			node->next = next->next;

		delete next;
	}

protected:

	SLNode*	head;
};

#endif