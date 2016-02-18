#pragma once

#include "PCSTree.h"

template<typename T>
class NodeLink;

template<typename T>
class Manager {
public:
	static void Add( NodeLink<T>* const node );
	static void Remove( NodeLink<T>* const node );
	static PCSTree* GetObjectList();
	static void Cleanup();

private:
	static Manager* privGetInstance();
	// Do not give default constructor as public! Keep it secret, keep it safe.
	Manager() {
		active = new PCSTree;
	}
	
	// members
	PCSTree *active;
};

template <typename T>
void Manager<T>::Add(NodeLink<T>* const node) {
	Manager<T>* man = privGetInstance();
	
	NodeLink<T>* root = static_cast< NodeLink<T>* >(man->active->getRoot());
	man->active->insert( node, root );
}

template <typename T>
void Manager<T>::Remove(NodeLink<T>* const node) {
	Manager<T>* man = privGetInstance();

	man->active->remove( node );
}

template <typename T>
PCSTree* Manager<T>::GetObjectList() {
	return privGetInstance()->active;
}

template <typename T>
void Manager<T>::Cleanup() {
	Manager<T>* man = privGetInstance();

	man->active->removeDown( man->active->getRoot() );
}

template <typename T>
Manager<T>* Manager<T>::privGetInstance() {
	static Manager<T> instance;
	return &instance;
};