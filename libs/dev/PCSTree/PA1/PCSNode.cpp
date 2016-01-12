#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "PCSTree.h"
#include "PCSNode.h"

// constructor
PCSNode::PCSNode()
{
	parent = NULL;
	child = NULL;
	sibling = NULL;
	level = 0;
	memset( &name[0], 0x0, 16);
}
   
// copy constructor
PCSNode::PCSNode(const PCSNode &in )
{
	this->parent = in.parent;
	this->child = in.child;
	this->sibling = in.sibling;
	this->level = in.level;
	memcpy(name, in.name, 16);
	name[15] = '\0';

}

// Specialize Constructor
PCSNode::PCSNode( PCSNode * const inParent, PCSNode * const inChild, PCSNode * const inSibling, const char * const inName)
{
	parent = inParent;
	child = inChild;
	sibling = inSibling;
	memcpy(name, inName, 16);
	name[15] = '\0';
}
   
PCSNode::PCSNode( const char * const inName)
{
	parent = NULL;
	child = NULL;
	sibling = NULL;
	memcpy(name, inName, 16);
	name[15] = '\0';
}
   
// destructor
PCSNode::~PCSNode()
{
}

// assignment operator
PCSNode &PCSNode::operator = (const PCSNode &in)
{
	parent = in.parent;
	child = in.child;
	sibling = in.sibling;
	level = in.level;
	memcpy(name, in.name, 16);
	name[15] = '\0';
	return *this;
}


void PCSNode::setParent( PCSNode * const in )
{
	this->parent = in;
}

void PCSNode::setChild( PCSNode * const in )
{
	this->child = in;
}

void PCSNode::setSibling( PCSNode * const in )
{
	this->sibling = in;
}

void PCSNode::setLevel( int const inLevel )
{
	this->level = inLevel;
}


PCSNode *PCSNode::getParent( void ) const
{
   return (PCSNode *)parent;
}

PCSNode *PCSNode::getChild( void ) const
{
   return (PCSNode *)child;
}

PCSNode *PCSNode::getSibling( void ) const
{
   return (PCSNode *)sibling;
}

int PCSNode::getLevel( void ) const
{
	return level;
}


PCSNodeReturnCode PCSNode::setName(const char * const inName )
{
	PCSNodeReturnCode tmp = PCSNode_FAIL_NULL_PTR;
	if (inName)
	{
		memcpy(name, inName, 16);
		name[15] = '\0';
		tmp = PCSNode_SUCCESS;
	}
	
	return tmp;
}


PCSNodeReturnCode PCSNode::getName(char * const outBuffer, int sizeOutBuffer ) const
{
	PCSNodeReturnCode tmp = PCSNode_FAIL_NULL_PTR;
	if (outBuffer)
	{
		if (sizeOutBuffer <= 0)					// user is messing with us. cant deliver a negative number of characters.
		{
			memset( &outBuffer[0], 0x0, 16);	// just copy 0's to the outBuffer. dont want the program to explode.
			outBuffer[15] = '\0';				// include a null terminator just because.
		}

		if (sizeOutBuffer > 16)					// asking for name larger than buffer
		{
			memcpy(outBuffer, this->name, 16);			// only copy name up to length of buffer
			outBuffer[15] = '\0';
		}

		if (sizeOutBuffer <= 16)				// asking for name less than or equal to buffer
		{
			memcpy(outBuffer, this->name, sizeOutBuffer-1);	// copy name into buffer byte by byte
			outBuffer[sizeOutBuffer-1] = '\0';
		}

		tmp = PCSNode_SUCCESS;
	}
	return tmp;
}

// the dumpNode() method contains code heavily modeled after Ed Keenan's work displayed in class.
// I made only minor changes to it, so all credit goes to him.
void PCSNode::dumpNode() const
{
	char parentName[16];
	char childName[16];
	char siblingName[16];

	if (parent)
		this->parent->getName(parentName, 16);
	else
		memcpy(parentName, "0", 2);

	if (child)
		this->child->getName(childName, 16);
	else
		memcpy(childName, "0", 2);

	if (sibling)
		this->sibling->getName(siblingName, 16);
	else
		memcpy(siblingName, "0", 2);

	printf("\n");
	printf("    Name: %s\n", this->name);
	printf(" Address: 0x%p\n", this);
	printf("  Parent: %s (0x%p)\n", parentName, this->parent);
	printf("   Child: %s (0x%p)\n", childName, this->child);
	printf(" Sibling: %s (0x%p)\n", siblingName, this->sibling);
}

void PCSNode::dumpChildren() const
{
	if (this->getChild())	// if there is a child, go down and print
	{
		PCSNode *tmp = this->getChild();
		tmp->dumpNode();
		while (tmp->getSibling())
		{
			tmp = tmp->getSibling();
			tmp->dumpNode();
		}
	}
}


void PCSNode::dumpSiblings() const
{
	if (this->getSibling())	// if there is a sibling, print all siblings
	{
		PCSNode *tmp = this->getSibling();
		tmp->dumpNode();

		while (tmp->getSibling())
		{
			tmp = tmp->getSibling();
			tmp->dumpNode();
		}
	}
}

void PCSNode::printDown( PCSNode * root )
{
	root->dumpNode();

	if (root->getChild() != NULL)		// if root has a child, go down again.
	{
		printDown(root->getChild());
	}

	if (root->getSibling() != NULL)		// now traverse siblings.
	{
		printDown(root->getSibling());
	}
}

int PCSNode::getNumSiblings() const
{
	int count = 1;
	if (this->getParent())					// if this is not the root node
	{
		if (this->getParent()->getChild())	// if there are siblings, count them.
		{
			PCSNode *tmp = this->getParent()->getChild();	// go to first child on list
			while (tmp->getSibling())		// while there are siblings, count them
			{
				tmp = tmp->getSibling();
				count++;
			}
		}	
	}
	
	return count;
}

int PCSNode::getNumChildren() const
{
   int count = 0;
   if (this->getChild())	// if there are children, count them.
   {
	   PCSNode *tmp = this->getChild();
	   count++;
	   
	   while (tmp->getSibling())
	   {
		   tmp = tmp->getSibling();
		   count++;
	   }
   }

   return count;
}
