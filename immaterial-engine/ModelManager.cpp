#include "ModelManager.h"

#include "File.h"

#include "GraphicsObjectFileHdr.h"
#include "md5.h"

void RitterSphere( Sphere &s, Vect *pt, int numPts );

void ModelManager::LoadModel( const char * const inFileName ) {
	assert( inFileName );

	FileHandle fh;
	FileError  ferror;
	gObjFileHdr modelHdr;

	MyVertex_stride* pVerts;
	Vertex* bound;
	MyTriList* tlist;
	char* modelName;

	Model* myModel;

	// open file and get header data
	ferror = File::open( fh, inFileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	// get header data
	ferror = File::read( fh, &modelHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// create and load vertex buffer
	pVerts = new MyVertex_stride[modelHdr.numVerts];

	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );
	ferror = File::read( fh, pVerts, modelHdr.numVerts * sizeof( MyVertex_stride ) );
	assert( ferror == FILE_SUCCESS );

	// calculate ritter sphere, save into the model later
	bound = new Vertex[modelHdr.numVerts];

	for ( auto i = 0; i < modelHdr.numVerts; i++ ) {
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

	// create and load triList buffer
	tlist = new MyTriList[modelHdr.numTriList];

	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );
	ferror = File::read( fh, tlist, modelHdr.numTriList * sizeof( MyTriList ) );
	assert( ferror == FILE_SUCCESS );

	// close
	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

	// Load model into GPU, store VAO handle ------------------------------------------
	myModel = new Model();

	RitterSphere( myModel->boundingVol, reinterpret_cast< Vect * >(&bound[0].x), modelHdr.numVerts );
	myModel->numTri = modelHdr.numTriList;

	glGenVertexArrays( 1, &myModel->vao );
	glBindVertexArray( myModel->vao );

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	GLuint vbo[2];
	glGenBuffers( 2, vbo );

	// Load the combined data: ---------------------------------------------------------
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * modelHdr.numVerts, pVerts, GL_STATIC_DRAW );

	// VERTEX data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_VERTEX );
	auto offsetVert = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].x) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetVert );

	// Texture data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_TEXTURE0 );
	auto offsetTex = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].s) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetTex );

	// Normal data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_NORMAL );
	auto offsetNorm = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].nx) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetNorm );

	// Load the index data: ---------------------------------------------------------
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * modelHdr.numTriList, tlist, GL_STATIC_DRAW );

	modelName = new char[strlen( inFileName ) - 3];
	memcpy( modelName, inFileName, strlen( inFileName ) - 4 );
	modelName[strlen( inFileName ) - 4] = '\0';

	// hash the model name and store it as the hashName for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(modelName), strlen( modelName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	Add( new ModelNode( modelName, hashID, myModel ) );
}

void ModelManager::LoadBufferedModel( unsigned char * const modelBuff ) {
	assert( modelBuff );

	MyVertex_stride* pVerts;
	Vertex* bound;
	MyTriList* tlist;
	char* modelName;

	Model* myModel;

	auto modelHdr = reinterpret_cast< gObjFileHdr * >(modelBuff);
	auto vOffset = reinterpret_cast< MyVertex_stride * >(reinterpret_cast< unsigned int >( modelBuff ) +modelHdr->vertBufferOffset);

	// copy verts
	pVerts = new MyVertex_stride[modelHdr->numVerts];

	for ( auto i = 0; i < modelHdr->numVerts; i++ ) {
		pVerts[i] = vOffset[i];
	}

	// calc ritter sphere, save into the model later
	bound = new Vertex[modelHdr->numVerts];

	for ( auto i = 0; i < modelHdr->numVerts; i++ ) {
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

	// copy trilist
	auto tOffset = reinterpret_cast< MyTriList * >(reinterpret_cast< unsigned int >( modelBuff ) +modelHdr->triListBufferOffset);
	tlist = new MyTriList[modelHdr->numTriList];

	for ( auto i = 0; i < modelHdr->numTriList; i++ ) {
		tlist[i] = tOffset[i];
	}

	// Load model into GPU, store VAO handle ------------------------------------------
	myModel = new Model();
	RitterSphere( myModel->boundingVol, reinterpret_cast< Vect * >(&bound[0].x), modelHdr->numVerts );
	myModel->numTri = modelHdr->numTriList;

	glGenVertexArrays( 1, &myModel->vao );
	glBindVertexArray( myModel->vao );

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	GLuint vbo[2];
	glGenBuffers( 2, vbo );

	// Load the combined data: ---------------------------------------------------------
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * modelHdr->numVerts, pVerts, GL_STATIC_DRAW );

	// VERTEX data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_VERTEX );
	auto offsetVert = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].x) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetVert );

	// Texture data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_TEXTURE0 );
	auto offsetTex = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].s) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetTex );

	// Normal data: ---------------------------------------------------------
	glEnableVertexAttribArray( GLT_ATTRIBUTE_NORMAL );
	auto offsetNorm = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].nx) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetNorm );

	// Load the index data: ---------------------------------------------------------
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * modelHdr->numTriList, tlist, GL_STATIC_DRAW );

	// Model loaded into GPU, VAO stored in myModel->vao. Generate model name by dropping extension.
	modelName = new char[strlen( modelHdr->objName ) - 3];
	memcpy( modelName, modelHdr->objName, strlen( modelHdr->objName ) - 4 );
	modelName[strlen( modelHdr->objName ) - 4] = '\0';

	// hash the model name and store it as the hash name for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(modelName), strlen( modelName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	Add( new ModelNode( modelName, hashID, myModel ) );
}

Model* ModelManager::Find( char * const inModelName ) {
	assert( inModelName );

	auto walker = static_cast< ModelNode * >(GetObjectList()->getRoot());
	auto tmp = walker->getData();

	// hash inModelName, use the int to find the model
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(inModelName), strlen( inModelName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// if there is a model to be asked for, return it
	if ( strlen( inModelName ) > 0 ) {
		// find node
		while ( walker != nullptr ) {
			if ( hashID == walker->hashName ) {
				break;
			}

			walker = static_cast< ModelNode * >(walker->getSibling());
		}
	} else {
		// otherwise, return a dummy model (first one ever loaded)
		while ( static_cast< ModelNode * >(walker->getSibling()) != nullptr ) {
			walker = static_cast< ModelNode * >(walker->getSibling());
		}
	}

	if ( walker != nullptr ) {
		tmp = walker->getData();
	}

	// return textureID stored in node, used by GPU
	return tmp;
}