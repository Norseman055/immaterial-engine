#pragma once

#include <memory>

using namespace std;

// Simple data structure
struct PCSTreeInfo {
	int numNodes;
	int maxNodeCount;
	int numLevels;
	int maxLevelCount;
};

// forward declare
class PCSNode;

// PCSTree class
class PCSTree {
public:
	// constructor
	PCSTree();

	// destructor
	virtual ~PCSTree();

	// get Root
	weak_ptr<PCSNode> getRoot( void ) const;

	// insert
	void insert( const shared_ptr<PCSNode> inNode, const shared_ptr<PCSNode> parent );

	// remove
	void remove( const shared_ptr<PCSNode> inNode );

	// get info
	void getInfo( PCSTreeInfo &infoContainer );
	void dumpTree() const;
	void goDown( const shared_ptr<PCSNode> root );
	void removeDown( shared_ptr<PCSNode> root );
	void checkLevels( const shared_ptr<PCSNode> root );

private:
	// copy constructor
	PCSTree( PCSTree &in );

	// assignment operator
	PCSTree & operator = (const PCSTree &in);

	// Data
	PCSTreeInfo info;
	shared_ptr<PCSNode> root;
};