// MY HEADERS
#include "DEBUGGING.h"
#include "PyramidModel.h"
#include "MathEngine.h"
#include "File.h"
#include "GraphicsObjectFileHdr.h"

// DEFINES
#define USE_PYRAMID_FILE 1	// 1 - uses the data file "pyramid.cdm", 0 - creates and uses the file "pyramid.cdm".

PyramidModel::PyramidModel() : vao( 0 ) {
	out( "PyramidModel(): ---------------------\n" );
}

void PyramidModel::createVAO() {
	struct MyVertex_stride {
		float x;
		float y;
		float z;
		float s;
		float t;
		float nx;
		float ny;
		float nz;
	};

	struct MyTriList {
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};

	FileHandle fh;
	FileError  ferror;

#if USE_PYRAMID_FILE

	ferror = File::open( fh, "pyramid.cdm", FILE_READ );
	assert( ferror == FILE_SUCCESS );

	// get header data
	gObjFileHdr pyramidHdr;

	ferror = File::read( fh, &pyramidHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// create vertex buffer
	auto pVerts = static_cast< MyVertex_stride * >(malloc( pyramidHdr.numVerts * sizeof( MyVertex_stride ) ));

	// load verts
	ferror = File::seek( fh, FILE_SEEK_BEGIN, pyramidHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, pVerts, pyramidHdr.numVerts * sizeof( MyVertex_stride ) );
	assert( ferror == FILE_SUCCESS );

	// create triList buffer
	auto tlist = static_cast< MyTriList * >(malloc( pyramidHdr.numTriList * sizeof( MyTriList ) ));

	// load triList
	ferror = File::seek( fh, FILE_SEEK_BEGIN, pyramidHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, tlist, pyramidHdr.numTriList * sizeof( MyTriList ) );
	assert( ferror == FILE_SUCCESS );

	// close
	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

#else
	MyTriList	tlist[6];
	MyVertex_stride pVerts[18];

	// front
	tlist[0].v1 = 0;
	tlist[0].v2 = 1;
	tlist[0].v3 = 2;

	// left
	tlist[1].v1 = 3;
	tlist[1].v2 = 4;
	tlist[1].v3 = 5;

	// right
	tlist[2].v1 = 6;
	tlist[2].v2 = 7;
	tlist[2].v3 = 8;

	// back
	tlist[3].v1 = 9;
	tlist[3].v2 = 10;
	tlist[3].v3 = 11;

	// bottom 1
	tlist[4].v1 = 12;
	tlist[4].v2 = 13;
	tlist[4].v3 = 14;

	// bottom 2
	tlist[5].v1 = 15;
	tlist[5].v2 = 16;
	tlist[5].v3 = 17;

	pVerts[0].x = -1;
	pVerts[0].y = -1;
	pVerts[0].z = -1;
	pVerts[0].s = 0;
	pVerts[0].t = 0;
	pVerts[0].nx = 0;
	pVerts[0].ny = -1;
	pVerts[0].nz = 0;

	pVerts[1].x = 1;
	pVerts[1].y = -1;
	pVerts[1].z = -1;
	pVerts[1].s = 1;
	pVerts[1].t = 0;
	pVerts[1].nx = 0;
	pVerts[1].ny = -1;
	pVerts[1].nz = 0;

	pVerts[2].x = 1;
	pVerts[2].y = -1;
	pVerts[2].z = 1;
	pVerts[2].s = 1;
	pVerts[2].t = 1;
	pVerts[2].nx = 0;
	pVerts[2].ny = -1;
	pVerts[2].nz = 0;

	pVerts[3].x = -1;
	pVerts[3].y = -1;
	pVerts[3].z = 1;
	pVerts[3].s = 0;
	pVerts[3].t = 1;
	pVerts[3].nx = 0;
	pVerts[3].ny = -1;
	pVerts[3].nz = 0;

	pVerts[4].x = -1;
	pVerts[4].y = -1;
	pVerts[4].z = -1;
	pVerts[4].s = 0;
	pVerts[4].t = 0;
	pVerts[4].nx = 0;
	pVerts[4].ny = -1;
	pVerts[4].nz = 0;

	pVerts[5].x = 1;
	pVerts[5].y = -1;
	pVerts[5].z = 1;
	pVerts[5].s = 1;
	pVerts[5].t = 1;
	pVerts[5].nx = 0;
	pVerts[5].ny = -1;
	pVerts[5].nz = 0;

	pVerts[6].x = 0;
	pVerts[6].y = 1;
	pVerts[6].z = 0;
	pVerts[6].s = 0.5;
	pVerts[6].t = 1.0;
	pVerts[6].nx = 0;
	pVerts[6].ny = 2;
	pVerts[6].nz = 4;

	pVerts[7].x = -1;
	pVerts[7].y = -1;
	pVerts[7].z = 1;
	pVerts[7].s = 0;
	pVerts[7].t = 0;
	pVerts[7].nx = 0;
	pVerts[7].ny = 2;
	pVerts[7].nz = 4;

	pVerts[8].x = 1;
	pVerts[8].y = -1;
	pVerts[8].z = 1;
	pVerts[8].s = 1;
	pVerts[8].t = 0;
	pVerts[8].nx = 0;
	pVerts[8].ny = 2;
	pVerts[8].nz = 4;

	pVerts[9].x = 0;
	pVerts[9].y = 1;
	pVerts[9].z = 0;
	pVerts[9].s = 0.5;
	pVerts[9].t = 1.0f;
	pVerts[9].nx = -4;
	pVerts[9].ny = 2;
	pVerts[9].nz = 0;

	pVerts[10].x = -1;
	pVerts[10].y = -1;
	pVerts[10].z = -1;
	pVerts[10].s = 1;
	pVerts[10].t = 0;
	pVerts[10].nx = -4;
	pVerts[10].ny = 2;
	pVerts[10].nz = 0;

	pVerts[11].x = -1;
	pVerts[11].y = -1;
	pVerts[11].z = 1;
	pVerts[11].s = 0;
	pVerts[11].t = 0;
	pVerts[11].nx = -4;
	pVerts[11].ny = 2;
	pVerts[11].nz = 0;

	pVerts[12].x = 0;
	pVerts[12].y = 1;
	pVerts[12].z = 0;
	pVerts[12].s = 0.5;
	pVerts[12].t = 1.0;
	pVerts[12].nx = 4;
	pVerts[12].ny = 2;
	pVerts[12].nz = 0;

	pVerts[13].x = 1;
	pVerts[13].y = -1;
	pVerts[13].z = 1;
	pVerts[13].s = 1;
	pVerts[13].t = 0;
	pVerts[13].nx = 4;
	pVerts[13].ny = 2;
	pVerts[13].nz = 0;

	pVerts[14].x = 1;
	pVerts[14].y = -1;
	pVerts[14].z = -1;
	pVerts[14].s = 0;
	pVerts[14].t = 0;
	pVerts[14].nx = 4;
	pVerts[14].ny = 2;
	pVerts[14].nz = 0;

	pVerts[15].x = 0;
	pVerts[15].y = 1;
	pVerts[15].z = 0;
	pVerts[15].s = 0.5;
	pVerts[15].t = 1.0;
	pVerts[15].nx = 0;
	pVerts[15].ny = 2;
	pVerts[15].nz = -4;

	pVerts[16].x = 1;
	pVerts[16].y = -1;
	pVerts[16].z = -1;
	pVerts[16].s = 0;
	pVerts[16].t = 0;
	pVerts[16].nx = 0;
	pVerts[16].ny = 2;
	pVerts[16].nz = -4;

	pVerts[17].x = -1;
	pVerts[17].y = -1;
	pVerts[17].z = -1;
	pVerts[17].s = 1;
	pVerts[17].t = 0;
	pVerts[17].nx = 0;
	pVerts[17].ny = 2;
	pVerts[17].nz = -4;

	// Write the data to a file ---------------------------------------------

	//----------- WRITE ------------------------------------------
	// 1. Get the existing header

	// open file
	ferror = File::open( fh, "pyramid.cdm", FILE_READ );
	assert( ferror == FILE_SUCCESS );

	gObjFileHdr pyramidHdr;

	// write data of header to pyramidHdr
	ferror = File::read( fh, &pyramidHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// close
	ferror = File::close( fh );

	// 2. Write the data to the file

	// create file if not done already, start with header
	ferror = File::open( fh, "pyramid.cdm", FILE_READ_WRITE );
	assert( ferror == FILE_SUCCESS );

	// write the header
	ferror = File::write( fh, &pyramidHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// update header with numVerts, VertBufferOffset
	pyramidHdr.numVerts = 18;
	ferror = File::tell( fh, pyramidHdr.vertBufferOffset );
	assert( ferror == FILE_SUCCESS );

	// write the vertex data
	ferror = File::write( fh, pVerts, 18 * sizeof( MyVertex_stride ) );
	assert( ferror == FILE_SUCCESS );

	// update header with numTriList, triListBufferOffset
	pyramidHdr.numTriList = 6;
	ferror = File::tell( fh, pyramidHdr.triListBufferOffset );
	assert( ferror == FILE_SUCCESS );

	// write the triList data
	ferror = File::write( fh, tlist, 6 * sizeof( MyTriList ) );
	assert( ferror == FILE_SUCCESS );

	// fix header with updated data
	ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
	assert( ferror == FILE_SUCCESS );

	ferror = File::write( fh, &pyramidHdr, sizeof( gObjFileHdr ) );
	assert( ferror == FILE_SUCCESS );

	// close file
	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

#endif

	// -------------------------------------------------------------------
	// MODIFYING THE MODEL FOR TESTING PURPOSES WITH ANIMATION SYSTEM
	for ( int i = 0; i < 18; i++ ) {
		// first center around origin
		Matrix Trans( TRANS, 0.0f, 1.0f, 0.0f );

		// shrink the Y axis to be 0-1,
		Matrix Scale( SCALE, 1.0f, 0.5f, 1.0f );

		// rotates it to Z axis
		Matrix Rot( ROT_X, 90.0f * MATH_PI_180 );

		// Creates unit length pyramid in the Z direction
		auto M = Trans * Scale * Rot;

		// Now process all verts

		Vect vert( pVerts[i].x, pVerts[i].y, pVerts[i].z );
		Vect vertNorm( pVerts[i].nx, pVerts[i].ny, pVerts[i].nz );

		Vect vout;
		Vect voutNorm;

		vout = vert * M;
		voutNorm = vertNorm * M;
		voutNorm.norm();

		pVerts[i].x = vout[x];
		pVerts[i].y = vout[y];
		pVerts[i].z = vout[z];
		pVerts[i].nx = voutNorm[x];
		pVerts[i].ny = voutNorm[y];
		pVerts[i].nz = voutNorm[z];

		//printf(" vert[%d]  %f  %f  %f\n",i, vout[x], vout[y], vout[z]);
	}

	// -------------------------------------------------------------------

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
	glBufferData( GL_ARRAY_BUFFER, sizeof( MyVertex_stride ) * 18, pVerts, GL_STATIC_DRAW );

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
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( MyTriList ) * 6, tlist, GL_STATIC_DRAW );
}