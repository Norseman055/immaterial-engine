#include <string.h>
#include <stdio.h>

#include "PCSNode.h"

// constructor
PCSNode::PCSNode()
	: level( 0 ) {
	this->parent = nullptr;
	this->child = nullptr;
	this->sibling = nullptr;
	memset( &name[0], 0x0, 16 );
}

// copy constructor
PCSNode::PCSNode( const PCSNode &in )
	:level( in.level ) {
	this->parent = in.parent;
	this->child = in.child;
	this->sibling = in.sibling;
	memcpy( name, in.name, 16 );
	name[15] = '\0';
}

// Specialize Constructor
PCSNode::PCSNode( shared_ptr<PCSNode> const inParent, shared_ptr<PCSNode> const inChild, shared_ptr<PCSNode> const inSibling, const char* const inName ) {
	parent = inParent;
	child = inChild;
	sibling = inSibling;
	level = inSibling->level;
	memcpy( name, inName, 16 );
	name[15] = '\0';
}

PCSNode::PCSNode( const char* const inName )
	: level( 0 ) {
	parent = nullptr;
	child = nullptr;
	sibling = nullptr;
	memcpy( name, inName, 16 );
	name[15] = '\0';
}

// destructor
PCSNode::~PCSNode() { }

// assignment operator
PCSNode &PCSNode::operator = (const PCSNode &in) {
	parent = in.parent;
	child = in.child;
	sibling = in.sibling;
	level = in.level;
	memcpy( name, in.name, 16 );
	name[15] = '\0';
	return *this;
}

void PCSNode::setParent( const PCSNode* const in ) {
	this->parent = make_shared<PCSNode>( *in );
}

void PCSNode::setChild( const PCSNode* const in ) {
	this->child = make_shared<PCSNode>( *in );
}

void PCSNode::setSibling( const PCSNode* const in ) {
	this->sibling = make_shared<PCSNode>( *in );
}

void PCSNode::setParent( const shared_ptr<PCSNode> in ) {
	this->parent = in;
}

void PCSNode::setChild( const shared_ptr<PCSNode> in ) {
	this->child = in;
}

void PCSNode::setSibling( const shared_ptr<PCSNode> in ) {
	this->sibling = in;
}

void PCSNode::setLevel( const int inLevel ) {
	this->level = inLevel;
}

weak_ptr<PCSNode> PCSNode::getParent( void ) const {
	return parent;
}

weak_ptr<PCSNode> PCSNode::getChild( void ) const {
	return child;
}

weak_ptr<PCSNode> PCSNode::getSibling( void ) const {
	return sibling;
}

int PCSNode::getLevel( void ) const {
	return level;
}

PCSNodeReturnCode PCSNode::setName( const char * const inName ) {
	auto tmp = PCSNode_FAIL_NULL_PTR;
	if ( inName ) {
		memcpy( name, inName, 16 );
		name[15] = '\0';
		tmp = PCSNode_SUCCESS;
	}

	return tmp;
}

PCSNodeReturnCode PCSNode::getName( char * const outBuffer, int sizeOutBuffer ) const {
	auto tmp = PCSNode_FAIL_NULL_PTR;
	if ( outBuffer ) {
		if ( sizeOutBuffer <= 0 )					// user is messing with us. cant deliver a negative number of characters.
		{
			memset( outBuffer, 0x0, 16 );	// just copy 0's to the outBuffer. don't want the program to explode.
			outBuffer[15] = '\0';				// include a null terminator just because.
		}

		if ( sizeOutBuffer > 16 )					// asking for name larger than buffer
		{
			memcpy( outBuffer, this->name, 16 );			// only copy name up to length of buffer
			outBuffer[15] = '\0';
		}

		if ( sizeOutBuffer <= 16 )				// asking for name less than or equal to buffer
		{
			memcpy( outBuffer, this->name, sizeOutBuffer - 1 );	// copy name into buffer byte by byte
			outBuffer[sizeOutBuffer - 1] = '\0';
		}

		tmp = PCSNode_SUCCESS;
	}
	return tmp;
}

// the dumpNode() method contains code heavily modeled after Ed Keenan's work displayed in class.
// I made only minor changes to it, so all credit goes to him.
void PCSNode::dumpNode() const {
	char parentName[16];
	char childName[16];
	char siblingName[16];

	if ( parent )
		this->parent->getName( parentName, 16 );
	else
		memcpy( parentName, "0", 2 );

	if ( child )
		this->child->getName( childName, 16 );
	else
		memcpy( childName, "0", 2 );

	if ( sibling )
		this->sibling->getName( siblingName, 16 );
	else
		memcpy( siblingName, "0", 2 );

	printf( "\n" );
	printf( "    Name: %s\n", this->name );
	printf( " Address: 0x%p\n", this );
	printf( "  Parent: %s (0x%p)\n", parentName, this->parent.get() );
	printf( "   Child: %s (0x%p)\n", childName, this->child.get() );
	printf( " Sibling: %s (0x%p)\n", siblingName, this->sibling.get() );
}

void PCSNode::dumpChildren() const {
	if ( this->child ) {
		child->dumpNode();
		child->dumpSiblings();
	}
}

void PCSNode::dumpSiblings() const {
	if ( this->sibling ) {
		this->sibling->privDumpSibling();
	}
}

void PCSNode::privDumpSibling() const {
	this->dumpNode();
	if ( this->sibling ) {
		this->sibling->privDumpSibling();
	}
}

void PCSNode::printDown( shared_ptr<PCSNode> root ) const {
	root->dumpNode();

	if ( this->child )		// if root has a child, go down again.
	{
		printDown( this->child );
	}

	if ( this->sibling )		// now traverse siblings.
	{
		printDown( this->sibling );
	}
}

int PCSNode::getNumSiblings() const {
	auto count = 1;

	if ( this->parent && this->parent->child )	// if this is not the root node
	{
		auto walker = this->parent->child;	// go to first child on list
		while ( walker->sibling )		// while there are siblings, count them
		{
			walker = walker->sibling;
			count++;
		}
	}

	return count;
}

int PCSNode::getNumChildren() const {
	auto count = 0;
	if ( this->child )	// if there are children, count them.
	{
		auto node = this->child;
		count++;

		while ( node->sibling ) {
			node = node->sibling;
			count++;
		}
	}

	return count;
}