#include <stdio.h>

#include "PCSTree.h"
#include "PCSNode.h"

// constructor
PCSTree::PCSTree() {
	info.maxLevelCount = 0;
	info.maxNodeCount = 0;
	info.numLevels = 0;
	info.numNodes = 0;
	root = nullptr;
};

// destructor
PCSTree::~PCSTree() { };

// get Root
weak_ptr<PCSNode> PCSTree::getRoot( void ) const {
	return root;
}

// insert
void PCSTree::insert( shared_ptr<PCSNode> const inNode, shared_ptr<PCSNode> const parent ) {
	if ( this->root )				// if tree has a root already
	{
		inNode->setParent( parent );		// set node's parent to specified parent.
		inNode->setLevel( parent->getLevel() + 1 );

		if ( !parent->getChild().expired() )			// if parent node has children, add it as first sibling on list.
		{	// store first sibling as tmp, add node as parent's child, adjust sibling list
			auto child = parent->getChild().lock();
			parent->setChild( inNode );
			inNode->setSibling( child );
		} else							// if that parent node has no children, make it a child.
		{
			parent->setChild( inNode );	// set parents node child to inNode
		}
	}

	else								// tree has no root. it is now the root. forget your parent specifier.
	{
		root = inNode;					// make root the first node
		inNode->setParent( nullptr );		// set the node's parent to null. its now the root.
		inNode->setLevel( 1 );
	}

	checkLevels( inNode );
	info.maxNodeCount++;
	info.numNodes++;
}

// Remove
// super easy process. just remove its link from the parent and make sure siblings to left link to the right
void PCSTree::remove( shared_ptr<PCSNode> const inNode ) {
	// make sure inNode is NOT the root. if it is the root, just zero out all its stats.
	if ( inNode != root ) {
		auto parent = inNode->getParent().lock();
		auto parentChild = parent->getChild().lock();

		// check to see if inNode is NOT the only child.
		if ( !inNode->getSibling().expired() || parentChild != inNode ) {
			if ( parentChild == inNode ) {
				if ( !inNode->getSibling().expired() ) {
					// first child
					parent->setChild( inNode->getSibling().lock() );
				} else {
					// only child
					parent->setChild( nullptr );
				}
			} else {
				auto tmp = parentChild;					// create temp node to find previous child
				while ( tmp->getSibling().lock() != inNode )
					tmp = tmp->getSibling().lock();		// move temp node to position right before child

				if ( !inNode->getSibling().expired() )
					// middle
					tmp->setSibling( inNode->getSibling().lock() );	// link inNode's previous sibling to its next sibling.
				else
					// youngest
					tmp->setSibling( nullptr );		// set final sibling to null
			}
		}

		// inNode is only child. just tell parent that it has no children.
		else
			parent->setChild( nullptr );
	} else
		root = nullptr;

	// set everything derived from inNode (children and their siblings, etc) to 0
	if ( !inNode->getChild().expired() )
		removeDown( inNode->getChild().lock() );

	// clean up inNode links to 0
	inNode->setChild( nullptr );
	inNode->setParent( nullptr );
	inNode->setSibling( nullptr );

	// update info
	info.numNodes--;
}

// get tree info
void PCSTree::getInfo( PCSTreeInfo &infoContainer ) {
	info.numLevels = 0;
	auto travelTree = root;
	if ( travelTree )	// if it isn't a null pointer, check the depth of the tree.
	{
		goDown( travelTree );
	}

	infoContainer.maxLevelCount = info.maxLevelCount;
	infoContainer.maxNodeCount = info.maxNodeCount;
	infoContainer.numLevels = info.numLevels;
	infoContainer.numNodes = info.numNodes;
}

void PCSTree::dumpTree() const {
	if ( root ) {
		printf( "\ndumpTree() ----------------" );
		root->printDown( root );
	}
}

// this function is called just once to check the levels with getInfo().
void PCSTree::goDown( const shared_ptr<PCSNode> _root ) {
	if ( !_root->getChild().expired() )		// if root has a child, go down again.
	{
		goDown( _root->getChild().lock() );
	}

	checkLevels( _root );					// check levels for that tree

	if ( !_root->getSibling().expired() )		// now traverse siblings.
	{
		goDown( _root->getSibling().lock() );
	}
}

// this deletes all children below a specified original root. basically the same as goDown, but calls remove function.
// at its final iteration, it is always at the final sibling, so it zeros everything out.
void PCSTree::removeDown( shared_ptr<PCSNode> const _root ) {
	if ( !_root->getChild().expired() )		// if root has a child, go down again.
	{
		removeDown( _root->getChild().lock() );
	}

	if ( !_root->getSibling().expired() )		// now traverse siblings.
	{
		removeDown( _root->getSibling().lock() );
	}

	// zero out current node
	_root->setChild( nullptr );
	_root->setParent( nullptr );
	_root->setSibling( nullptr );

	// update info
	info.numNodes--;
}

// this is a function that just checks the number of levels away from the root any given node is.
void PCSTree::checkLevels( const shared_ptr<PCSNode> inNode ) {
	if ( inNode->getLevel() > info.maxLevelCount )		// if current level is greater than max, increase max
		info.maxLevelCount = inNode->getLevel();

	if ( inNode->getLevel() > info.numLevels )			// if current level is greater than recorded levels, increase recorded levels
		info.numLevels = inNode->getLevel();
}