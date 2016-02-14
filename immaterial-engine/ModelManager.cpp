#include <string.h>

#include "DEBUGGING.h"
#include "MathEngine.h"
#include "File.h"

#include "ModelManager.h"
#include "GraphicsObjectFileHdr.h"
#include "md5.h"

void RitterSphere( Sphere &s, Vect *pt, int numPts );

ModelMan::ModelMan()
	: active( nullptr ) { }

void ModelMan::DeleteModels() {
	auto walker = privGetInstance()->active;
	auto tmp = walker;
	while ( walker != nullptr ) {
		walker = walker->next;
		delete tmp;
		tmp = walker;
	}
};

// singleton
ModelMan* ModelMan::privGetInstance() {
	static ModelMan modelMan;
	return &modelMan;
}

void ModelMan::LoadModel( const char * const inFileName ) {
	assert( inFileName );

	FileHandle fh;
	FileError  ferror;

	ferror = File::open( fh, inFileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	// get header data
	gObjFileHdr modelHdr;

	ferror = File::read( fh, &modelHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// create vertex buffer
	auto pVerts = new MyVertex_stride[modelHdr.numVerts];

	// load verts
	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, pVerts, modelHdr.numVerts * sizeof( MyVertex_stride ) );
	assert( ferror == FILE_SUCCESS );

	// calculate ritter sphere, save into the model later
	auto bound = new Vertex[modelHdr.numVerts];
	for ( auto i = 0; i < modelHdr.numVerts; i++ ) {
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

	// create triList buffer
	auto tlist = new MyTriList[modelHdr.numTriList];

	// load triList
	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, tlist, modelHdr.numTriList * sizeof( MyTriList ) );
	assert( ferror == FILE_SUCCESS );

	// close
	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

	// debug testing- make sure i have all verts and all tri's
	//printf("\n");
	//for (int i = 0; i < modelHdr.numVerts; i++)
	//{
	//	MyVertex_stride tmp = pVerts[i];
	//	printf("vert[%d]: %f %f %f %f %f %f %f \n", i, tmp.x, tmp.y, tmp.z, tmp.s, tmp.t, tmp.nx, tmp.ny, tmp.nz);
	//}

	//for (int i = 0; i < modelHdr.numTriList; i++)
	//{
	//	MyTriList tmp = tlist[i];
	//	printf("tri[%d]: %d %d %d\n", i, tmp.v1, tmp.v2, tmp.v3);
	//}

	// Load model into GPU, store VAO handle ------------------------------------------
	auto myModel = new Model;
	RitterSphere( myModel->boundingVol, reinterpret_cast< Vect * >(&bound[0].x), modelHdr.numVerts );
	myModel->numTri = modelHdr.numTriList;

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays( 1, &myModel->vao );

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray( myModel->vao );

	//GLuint vbo;
	GLuint vbo[2];

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers( 2, vbo );

	// Load the combined data: ---------------------------------------------------------

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

	/* Copy the vertex data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * modelHdr.numVerts, pVerts, GL_STATIC_DRAW );

	// VERTEX data: ---------------------------------------------------------

	// Set Attribute to 0
	//           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_VERTEX = 0

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_VERTEX );

	/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetVert = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].x) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetVert );

	// Texture data: ---------------------------------------------------------

	// Set Attribute to 3
	//           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_TEXTURE0 = 3

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_TEXTURE0 );

	/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetTex = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].s) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetTex );

	// Normal data: ---------------------------------------------------------

	// Set Attribute to 2
	//           WHY - 2? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_NORMAL = 2

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_NORMAL );

	/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetNorm = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].nx) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetNorm );

	// Load the index data: ---------------------------------------------------------

	/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );

	/* Copy the index data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * modelHdr.numTriList, tlist, GL_STATIC_DRAW );

	// Model loaded into GPU, VAO stored in myModel->vao. Generate model name by dropping extension.
	auto pMMan = privGetInstance();

	auto modName = new char[strlen( inFileName ) - 3];
	memcpy( modName, inFileName, strlen( inFileName ) - 4 );
	modName[strlen( inFileName ) - 4] = '\0';

	// hash the model name and store it as the hashName for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(modName), strlen( modName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	auto pNode = new ModelNode;
	pNode->set( modName, hashID, myModel );
	pMMan->privAddToFront( pNode, pMMan->active );

	delete[]( modName );
	delete[]( tlist );
	delete[]( bound );
	delete[]( pVerts );
}

void ModelMan::LoadBufferedModel( unsigned char * const modelBuff ) {
	auto modelHdr = reinterpret_cast< gObjFileHdr * >(modelBuff);
	auto vOffset = reinterpret_cast< MyVertex_stride * >(reinterpret_cast< unsigned int >( modelBuff ) +modelHdr->vertBufferOffset);

	// copy verts
	auto pVerts = new MyVertex_stride[modelHdr->numVerts];
	for ( auto i = 0; i < modelHdr->numVerts; i++ ) {
		pVerts[i] = vOffset[i];
	}

	// calc ritter sphere, save into the model later
	auto bound = new Vertex[modelHdr->numVerts];
	for ( auto i = 0; i < modelHdr->numVerts; i++ ) {
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

	// copy trilist
	auto tOffset = reinterpret_cast< MyTriList * >(reinterpret_cast< unsigned int >( modelBuff ) +modelHdr->triListBufferOffset);
	auto tlist = new MyTriList[modelHdr->numTriList];
	for ( auto i = 0; i < modelHdr->numTriList; i++ ) {
		tlist[i] = tOffset[i];
	}

	// Load model into GPU, store VAO handle ------------------------------------------
	auto myModel = new Model;
	RitterSphere( myModel->boundingVol, reinterpret_cast< Vect * >(&bound[0].x), modelHdr->numVerts );
	myModel->numTri = modelHdr->numTriList;

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays( 1, &myModel->vao );

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray( myModel->vao );

	//GLuint vbo;
	GLuint vbo[2];

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers( 2, vbo );

	// Load the combined data: ---------------------------------------------------------

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

	/* Copy the vertex data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * modelHdr->numVerts, pVerts, GL_STATIC_DRAW );

	// VERTEX data: ---------------------------------------------------------

	// Set Attribute to 0
	//           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_VERTEX = 0

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_VERTEX );

	/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetVert = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].x) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetVert );

	// Texture data: ---------------------------------------------------------

	// Set Attribute to 3
	//           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_TEXTURE0 = 3

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_TEXTURE0 );

	/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetTex = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].s) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetTex );

	// Normal data: ---------------------------------------------------------

	// Set Attribute to 2
	//           WHY - 2? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_NORMAL = 2

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( GLT_ATTRIBUTE_NORMAL );

	/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetNorm = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].nx) - reinterpret_cast< unsigned int >(pVerts));
	glVertexAttribPointer( GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetNorm );

	// Load the index data: ---------------------------------------------------------

	/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );

	/* Copy the index data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * modelHdr->numTriList, tlist, GL_STATIC_DRAW );

	// Model loaded into GPU, VAO stored in myModel->vao. Generate model name by dropping extension.
	auto pMMan = privGetInstance();

	auto modName = new char[strlen( modelHdr->objName ) - 3];
	memcpy( modName, modelHdr->objName, strlen( modelHdr->objName ) - 4 );
	modName[strlen( modelHdr->objName ) - 4] = '\0';

	// hash the model name and store it as the hash name for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(modName), strlen( modName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	auto pNode = new ModelNode;
	pNode->set( modName, hashID, myModel );
	pMMan->privAddToFront( pNode, pMMan->active );

	delete[]( modName );
	delete[]( tlist );
	delete[]( bound );
	delete[]( pVerts );
}

Model* ModelMan::Find( char * const inModelName ) {
	assert( inModelName );

	auto walker = static_cast< ModelNode * >(privGetInstance()->active);
	auto tmp = walker->storedModel;

	// hash inModelName, use the int to find the model
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(inModelName), strlen( inModelName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// if there is a model to be asked for, return it
	if ( strlen( inModelName ) > 0 ) {
		// find node
		while ( walker != nullptr ) {
			// i REALLY dislike strings, but for now it'll serve the purpose. maybe hash the object name and use that as a compare?
			if ( hashID == walker->hashName ) {
				break;
			}

			walker = static_cast< ModelNode * >(walker->next);
		}
	} else {
		// otherwise, return a dummy model (first one ever loaded)
		while ( static_cast< ModelNode * >(walker->next) != nullptr ) {
			walker = static_cast< ModelNode * >(walker->next);
		}
	}

	if ( walker != nullptr ) {
		tmp = walker->storedModel;
	}

	// return textureID stored in node, used by GPU
	return tmp;
}

void ModelMan::privAddToFront( ModelNodeLink* const node, ModelNodeLink *& head ) const {
	assert( node != nullptr );

	// empty list
	if ( head == nullptr ) {
		head = node;
	} else {
		// non-empty list, add to front
		node->next = head;
		head->prev = node;
		head = node;
	}
}