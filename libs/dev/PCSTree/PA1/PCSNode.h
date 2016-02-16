#pragma once

#define PCSNODE_NAME_SIZE (16)
#define PCSNODE_VERSION 1.4

#include <memory>

using namespace std;

// Return codes
enum PCSNodeReturnCode {
	PCSNode_SUCCESS,
	PCSNode_FAIL_NULL_PTR,
	PCSNode_FAIL_RETURN_NOT_INITIALIZED,
	PCSNode_DWORD = 0x7fffffff
};

class PCSNode {
public:
	// constructor
	PCSNode();

	// copy constructor
	PCSNode( const PCSNode &in );

	// Specialize Constructor
	PCSNode( shared_ptr<PCSNode> const inParent, shared_ptr<PCSNode> const inChild, shared_ptr<PCSNode> const inSibling, const char * const inName );
	PCSNode( const char* const inName );

	// destructor
	virtual ~PCSNode();

	// assignment operator
	PCSNode &operator = (const PCSNode &in);

	// equality operator for raw-to-shared compare

	friend bool operator<(const PCSNode& lhs, const PCSNode& rhs) {
		return lhs.level < rhs.level;
	}

	friend bool operator<=(const PCSNode& lhs, const PCSNode& rhs) {
		return !(rhs < lhs);
	}

	friend bool operator>(const PCSNode& lhs, const PCSNode& rhs) {
		return rhs < lhs;
	}

	friend bool operator>=(const PCSNode& lhs, const PCSNode& rhs) {
		return !(lhs < rhs);
	}

	friend bool operator==(const shared_ptr<PCSNode> lhs, const PCSNode* rhs) {
		return lhs->parent		== rhs->parent
				&& lhs->child	== rhs->child
				&& lhs->sibling == rhs->sibling
				&& lhs->level	== rhs->level;
	}
	friend bool operator==(const PCSNode* lhs, const shared_ptr<PCSNode> rhs) {
		return lhs->parent == rhs->getParent().lock()
			&& lhs->child == rhs->getChild().lock()
			&& lhs->sibling == rhs->getSibling().lock()
			&& lhs->level	== rhs->getLevel();
	}

	friend bool operator!=(const shared_ptr<PCSNode> lhs, const PCSNode* rhs) {
		return !(lhs == rhs);
	}
	friend bool operator!=(const PCSNode* lhs, const shared_ptr<PCSNode> rhs) {
		return !(lhs == rhs);
	}

	// accessors
		// Pre-C++11
	void setParent( const PCSNode* const in );
	void setChild( const PCSNode* const in );
	void setSibling( const PCSNode* const in );
		// Post-C++11
	void setParent( const shared_ptr<PCSNode> in );
	void setChild( const shared_ptr<PCSNode> in );
	void setSibling( const shared_ptr<PCSNode> in );
	void setLevel( const int inLevel );
	weak_ptr<PCSNode> getParent( void ) const;
	weak_ptr<PCSNode> getChild( void ) const;
	weak_ptr<PCSNode> getSibling( void ) const;
	int getLevel( void ) const;

	// name
	PCSNodeReturnCode setName( const char * const inName );
	PCSNodeReturnCode getName( char * const outBuffer, int sizeOutBuffer ) const;

	// dump
	void dumpNode() const;
	void dumpChildren() const;
	void dumpSiblings() const;
	void PCSNode::printDown( shared_ptr<PCSNode> root ) const;

	// get number of children/siblings
	int getNumSiblings() const;
	int getNumChildren() const;

private:
	// recursive dump
	void privDumpSibling() const;

private:
	shared_ptr<PCSNode> parent;
	shared_ptr<PCSNode> child;
	shared_ptr<PCSNode> sibling;
	int level;

	char name[PCSNODE_NAME_SIZE];
};