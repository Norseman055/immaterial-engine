
#include <string.h>

#include "OpenGL.h"
#include "DEBUGGING.h"
#include "MathEngine.h"
#include "File.h"

#include "ModelNode.h"
#include "ModelManager.h"
#include "GraphicsObjectFileHdr.h"
#include "md5.h"
#include "BoundingSphere.h"

#include "Align16.h"

void RitterSphere(Sphere &s, Vect *pt, int numPts);

ModelMan::ModelMan()
{
	this->active = 0;
}

void ModelMan::DeleteModels()
{
	ModelMan *mMan = privGetInstance();
	ModelNodeLink *walker = mMan->active;
	ModelNodeLink *tmp = walker;
	while (walker != 0)	{
		walker = walker->next;
		delete tmp;
		tmp = walker;
	}
};

// singleton
ModelMan* ModelMan::privGetInstance()
{
	static ModelMan modelMan;
	return &modelMan;
}

void ModelMan::LoadModel( const char * const inFileName )
{
	assert (inFileName);
	
	FileHandle fh;
	FileError  ferror;

	ferror = File::open( fh, inFileName, FILE_READ );
	assert (ferror == FILE_SUCCESS);

		// get header data
	gObjFileHdr modelHdr;

	ferror = File::read( fh, &modelHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

		// create vertex buffer
	MyVertex_stride *pVerts = new MyVertex_stride[modelHdr.numVerts];

		// load verts
	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.vertBufferOffset );
	assert (ferror == FILE_SUCCESS );

	ferror = File::read( fh, pVerts, modelHdr.numVerts * sizeof(MyVertex_stride) );
	assert (ferror == FILE_SUCCESS );

	// calc ritter sphere, save into the model later
	Vertex *bound = new Vertex[modelHdr.numVerts];
	for (int i = 0; i < modelHdr.numVerts; i++)
	{
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

		// create triList buffer
	MyTriList *tlist = new MyTriList[modelHdr.numTriList];

		// load triList
	ferror = File::seek( fh, FILE_SEEK_BEGIN, modelHdr.triListBufferOffset );
	assert (ferror == FILE_SUCCESS);

	ferror = File::read( fh, tlist, modelHdr.numTriList * sizeof(MyTriList) );
	assert (ferror == FILE_SUCCESS);

		// close
	ferror = File::close( fh );
	assert (ferror == FILE_SUCCESS);

	// debug testing- make sure i have all verts and all tris
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
	Model *myModel = new Model;
	RitterSphere(myModel->boundingVol, (Vect *)&bound[0].x, modelHdr.numVerts);
	myModel->numTri = modelHdr.numTriList;

		/* Allocate and assign a Vertex Array Object to our handle */
		glGenVertexArrays(1, &myModel->vao);
 
		/* Bind our Vertex Array Object as the current used object */
		glBindVertexArray(myModel->vao);

		//GLuint vbo;
		GLuint vbo[2];

		/* Allocate and assign two Vertex Buffer Objects to our handle */
		glGenBuffers(2, vbo);
 
    // Load the combined data: ---------------------------------------------------------

		/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
 
		/* Copy the vertex data to our buffer */
		// glBufferData(type, size in bytes, data, usage) 
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * modelHdr.numVerts, pVerts, GL_STATIC_DRAW);
		
   // VERTEX data: ---------------------------------------------------------
		
		// Set Attribute to 0
		//           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_VERTEX = 0

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);  

		/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetVert = (void *)((unsigned int)&pVerts[0].x - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetVert);
          
	// Texture data: ---------------------------------------------------------

		// Set Attribute to 3
		//           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_TEXTURE0 = 3

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);  

		/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetTex = (void *)((unsigned int)&pVerts[0].s - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetTex);

	// Normal data: ---------------------------------------------------------

		// Set Attribute to 2
		//           WHY - 2? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_NORMAL = 2

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);  

		/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetNorm = (void *)((unsigned int)&pVerts[0].nx - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetNorm);

	// Load the index data: ---------------------------------------------------------
	
		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

		/* Copy the index data to our buffer */
		// glBufferData(type, size in bytes, data, usage) 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList) * modelHdr.numTriList, tlist, GL_STATIC_DRAW);

	// Model loaded into GPU, VAO stored in myModel->vao. Generate model name by dropping extension.
		ModelMan *pMMan = ModelMan::privGetInstance();
		ModelNode *pNode = new ModelNode;
		
		char * modName = new char[strlen(inFileName) - 3];
		memcpy(modName, inFileName, strlen(inFileName)-4 );
		modName[strlen(inFileName)-4] = '\0';

		// hash the model name and store it as the hashName for the manager
		MD5Output out;
		MD5Buffer ((unsigned char *)modName, strlen(modName), out);
		GLuint hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

		pNode->set(modName, hashID, myModel);
		pMMan->privAddToFront(pNode, pMMan->active);

		delete[](modName);
		delete[](tlist);
		delete[](bound);
		delete[](pVerts);
}

void ModelMan::LoadBufferedModel( const unsigned char * const modelBuff)
{
	gObjFileHdr *modelHdr = (gObjFileHdr *)modelBuff;
	MyVertex_stride *vOffset = (MyVertex_stride *)((unsigned int)modelBuff + modelHdr->vertBufferOffset);

	// copy verts
	MyVertex_stride *pVerts = new MyVertex_stride[modelHdr->numVerts];
	for(int i = 0; i < modelHdr->numVerts; i++)	{
		pVerts[i] = vOffset[i];
	}

	// calc ritter sphere, save into the model later
	Vertex *bound = new Vertex[modelHdr->numVerts];
	for (int i = 0; i < modelHdr->numVerts; i++)
	{
		bound[i].x = pVerts[i].x;
		bound[i].y = pVerts[i].y;
		bound[i].z = pVerts[i].z;
	}

	// copy trilist
	MyTriList *tOffset = (MyTriList *)((unsigned int)modelBuff + modelHdr->triListBufferOffset);
	MyTriList *tlist = new MyTriList[modelHdr->numTriList];
	for(int i = 0; i < modelHdr->numTriList; i++)	{
		tlist[i] = tOffset[i];
	}

		// Load model into GPU, store VAO handle ------------------------------------------
	Model *myModel = new Model;
	RitterSphere(myModel->boundingVol, (Vect *)&bound[0].x, modelHdr->numVerts);
	myModel->numTri = modelHdr->numTriList;

		/* Allocate and assign a Vertex Array Object to our handle */
		glGenVertexArrays(1, &myModel->vao);
 
		/* Bind our Vertex Array Object as the current used object */
		glBindVertexArray(myModel->vao);

		//GLuint vbo;
		GLuint vbo[2];

		/* Allocate and assign two Vertex Buffer Objects to our handle */
		glGenBuffers(2, vbo);
 
    // Load the combined data: ---------------------------------------------------------

		/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
 
		/* Copy the vertex data to our buffer */
		// glBufferData(type, size in bytes, data, usage) 
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * modelHdr->numVerts, pVerts, GL_STATIC_DRAW);
		
   // VERTEX data: ---------------------------------------------------------
		
		// Set Attribute to 0
		//           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_VERTEX = 0

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);  

		/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetVert = (void *)((unsigned int)&pVerts[0].x - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetVert);
          
	// Texture data: ---------------------------------------------------------

		// Set Attribute to 3
		//           WHY - 3? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_TEXTURE0 = 3

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);  

		/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetTex = (void *)((unsigned int)&pVerts[0].s - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetTex);

	// Normal data: ---------------------------------------------------------

		// Set Attribute to 2
		//           WHY - 2? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
		//           GLT_ATTRIBUTE_NORMAL = 2

		// Specifies the index of the generic vertex attribute to be enabled
		glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);  

		/* Specify that our coordinate data is going into attribute index 3, and contains 2 floats per vertex */
		// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
		void *offsetNorm = (void *)((unsigned int)&pVerts[0].nx - (unsigned int)pVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT,  GL_FALSE, sizeof(MyVertex_stride), offsetNorm);

	// Load the index data: ---------------------------------------------------------
	
		/* Bind our 2nd VBO as being the active buffer and storing index ) */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

		/* Copy the index data to our buffer */
		// glBufferData(type, size in bytes, data, usage) 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList) * modelHdr->numTriList, tlist, GL_STATIC_DRAW);

	// Model loaded into GPU, VAO stored in myModel->vao. Generate model name by dropping extension.
		ModelMan *pMMan = ModelMan::privGetInstance();
		ModelNode *pNode = new ModelNode;

		char * modName = new char[strlen(modelHdr->objName) - 3];
		memcpy(modName, modelHdr->objName, strlen(modelHdr->objName)-4 );
		modName[strlen(modelHdr->objName)-4] = '\0';

		// hash the model name and store it as the hash name for the manager
		MD5Output out;
		MD5Buffer ((unsigned char *)modName, strlen(modName), out);
		GLuint hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

		pNode->set(modName, hashID, myModel);
		pMMan->privAddToFront(pNode, pMMan->active);

		delete[](modName);		
		delete[](tlist);
		delete[](bound);
		delete[](pVerts);
}

Model* ModelMan::Find( const char * const inModelName )
{
	assert (inModelName);

	ModelMan *pModMan = ModelMan::privGetInstance();

	ModelNode *walker = (ModelNode *)pModMan->active;
	Model *tmp = walker->storedModel;

	// hash inModelName, use the int to find the model
	MD5Output out;
	MD5Buffer ((unsigned char *)inModelName, strlen(inModelName), out);
	GLuint hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;
		
	// if there is a model to be asked for, return it
	if (strlen(inModelName) > 0)
	{
		// find node
		while ( walker != 0 )
		{	
			// i REALLY dislike strings, but for now itll serve the purpose. maybe hash the object name and use that as a compare?
			if ( hashID == walker->hashName )
			{
				break;
			}

			walker = (ModelNode *)walker->next;
		}
	}
	else	// otherwise, return a dummy model (first one ever loaded)
	{
		while ( (ModelNode *)walker->next != 0 )
		{
			walker = (ModelNode *)walker->next;
		}
	}

	if (walker != 0)
		tmp = walker->storedModel;


	// return textureID stored in node, used by GPU
	return tmp;
}

void ModelMan::privAddToFront( ModelNodeLink *node, ModelNodeLink *&head )
{
	assert (node != 0);

	// empty list
	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else	// non-empty list, add to front
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}