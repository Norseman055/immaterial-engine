#include "CameraModel.h"
#include "CameraManager.h"

extern GLShaderManager shaderManager;

CameraModel::CameraModel() : vao( 0 ) { }

void CameraModel::update() {
	this->createVAO( CameraManager::Find( CAMERA_CULLING ) );
}

void CameraModel::createVAO( Camera* const cam ) {
	struct MyVertex_stride {
		float x;
		float y;
		float z;
	};

	struct MyTriList {
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};

	MyVertex_stride pVerts[9];
	MyTriList tlist[8];

	tlist[0].v1 = 0;
	tlist[0].v2 = 1;
	tlist[0].v3 = 2;

	tlist[1].v1 = 0;
	tlist[1].v2 = 2;
	tlist[1].v3 = 3;

	tlist[2].v1 = 0;
	tlist[2].v2 = 3;
	tlist[2].v3 = 4;

	tlist[3].v1 = 0;
	tlist[3].v2 = 4;
	tlist[3].v3 = 1;

	// far
	tlist[4].v1 = 1;
	tlist[4].v2 = 2;
	tlist[4].v3 = 3;

	// far
	tlist[5].v1 = 3;
	tlist[5].v2 = 4;
	tlist[5].v3 = 1;

	// near
	tlist[6].v1 = 5;
	tlist[6].v2 = 6;
	tlist[6].v3 = 7;

	// near
	tlist[7].v1 = 7;
	tlist[7].v2 = 8;
	tlist[7].v3 = 5;

	Vect vTmp;

	cam->getPos( vTmp );
	pVerts[0].x = vTmp[x];
	pVerts[0].y = vTmp[y];
	pVerts[0].z = vTmp[z];

	cam->getFarTopRight( vTmp );
	pVerts[1].x = vTmp[x];
	pVerts[1].y = vTmp[y];
	pVerts[1].z = vTmp[z];

	cam->getFarTopLeft( vTmp );
	pVerts[2].x = vTmp[x];
	pVerts[2].y = vTmp[y];
	pVerts[2].z = vTmp[z];

	cam->getFarBottomLeft( vTmp );
	pVerts[3].x = vTmp[x];
	pVerts[3].y = vTmp[y];
	pVerts[3].z = vTmp[z];

	cam->getFarBottomRight( vTmp );
	pVerts[4].x = vTmp[x];
	pVerts[4].y = vTmp[y];
	pVerts[4].z = vTmp[z];

	cam->getNearTopRight( vTmp );
	pVerts[5].x = vTmp[x];
	pVerts[5].y = vTmp[y];
	pVerts[5].z = vTmp[z];

	cam->getNearTopLeft( vTmp );
	pVerts[6].x = vTmp[x];
	pVerts[6].y = vTmp[y];
	pVerts[6].z = vTmp[z];

	cam->getNearBottomLeft( vTmp );
	pVerts[7].x = vTmp[x];
	pVerts[7].y = vTmp[y];
	pVerts[7].z = vTmp[z];

	cam->getNearBottomRight( vTmp );
	pVerts[8].x = vTmp[x];
	pVerts[8].y = vTmp[y];
	pVerts[8].z = vTmp[z];

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays( 1, &this->vao );

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray( this->vao );

	//GLuint vbo;
	GLuint vbo[2];

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers( 2, vbo );

	// Load the combined data: ---------------------------------------------------------

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

	/* Copy the vertex data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * 9, pVerts, GL_STATIC_DRAW );

	// VERTEX data: ---------------------------------------------------------

	// Set Attribute to 0
	//           WHY - 0? and not 1,2,3 (this is tied to the shader attribute, it is defined in GLShaderManager.h)
	//           GLT_ATTRIBUTE_VERTEX = 0

	// Specifies the index of the generic vertex attribute to be enabled
	glEnableVertexAttribArray( 0 );

	/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
	// ( GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);
	auto offsetVert = reinterpret_cast< void * >(reinterpret_cast< unsigned int >(&pVerts[0].x) - reinterpret_cast< unsigned int >(&pVerts[0].x));
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( MyVertex_stride ), offsetVert );

	// Load the index data: ---------------------------------------------------------

	/* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );

	/* Copy the index data to our buffer */
	// glBufferData(type, size in bytes, data, usage)
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * 8, &tlist[0].v1, GL_STATIC_DRAW );
}