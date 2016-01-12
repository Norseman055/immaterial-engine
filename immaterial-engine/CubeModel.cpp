
// MY HEADERS
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CubeModel.h"
#include "MathEngine.h"
#include "File.h"
#include "GraphicsObjectFileHdr.h"

// GL GLOBALS
extern GLShaderManager shaderManager;

// DEFINES
#define USE_CUBE_FILE 1 // 1 - uses the data file "cube.cdm", 0 - creates and uses the file "cube.cdm".

CubeModel::CubeModel()
{
	out("CubeModel(): ----------------------\n");
}

void CubeModel::createVAO(void)
{
	struct MyVertex_stride
	{
		float x;
		float y;
		float z;
		float s;
		float t;
		float nx;
		float ny;
		float nz;
	};
	
	struct MyTriList
	{
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};
	
	FileHandle fh;
	FileError  ferror;

#if USE_CUBE_FILE

	ferror = File::open( fh, "cube.cdm", FILE_READ );
	assert (ferror == FILE_SUCCESS);

	// get header data
	gObjFileHdr cubeHdr;

	ferror = File::read( fh, &cubeHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

	// create vertex buffer
	MyVertex_stride *pVerts = (MyVertex_stride *)malloc(cubeHdr.numVerts * sizeof(MyVertex_stride) );

	// load verts
	ferror = File::seek( fh, FILE_SEEK_BEGIN, cubeHdr.vertBufferOffset );
	assert (ferror == FILE_SUCCESS);

	ferror = File::read( fh, pVerts, cubeHdr.numVerts * sizeof(MyVertex_stride) );

	// create trilist buffer
	MyTriList *tlist = (MyTriList *)malloc( cubeHdr.numTriList * sizeof(MyTriList) );

	// load triList
	ferror = File::seek( fh, FILE_SEEK_BEGIN, cubeHdr.triListBufferOffset );
	assert (ferror == FILE_SUCCESS);

	ferror = File::read( fh, tlist, cubeHdr.numTriList * sizeof(MyTriList) );
	assert (ferror == FILE_SUCCESS);

	ferror = File::close( fh );
	assert (ferror == FILE_SUCCESS);

#else
	MyTriList tlist[12];
	MyVertex_stride pVerts[36];

	tlist[0].v1 = 0;
	tlist[0].v2 = 1;
	tlist[0].v3 = 2;

	tlist[1].v1 = 3;
	tlist[1].v2 = 4;
	tlist[1].v3 = 5;

	tlist[2].v1 = 6;
	tlist[2].v2 = 7;
	tlist[2].v3 = 8;

	tlist[3].v1 = 9;
	tlist[3].v2 = 10;
	tlist[3].v3 = 11;

	tlist[4].v1 = 12;
	tlist[4].v2 = 13;
	tlist[4].v3 = 14;

	tlist[5].v1 = 15;
	tlist[5].v2 = 16;
	tlist[5].v3 = 17;

	tlist[6].v1 = 18;
	tlist[6].v2 = 19;
	tlist[6].v3 = 20;

	tlist[7].v1 = 21;
	tlist[7].v2 = 22;
	tlist[7].v3 = 23;

	tlist[8].v1 = 24;
	tlist[8].v2 = 25;
	tlist[8].v3 = 26;

	tlist[9].v1 = 27;
	tlist[9].v2 = 28;
	tlist[9].v3 = 29;

	tlist[10].v1 = 30;
	tlist[10].v2 = 31;
	tlist[10].v3 = 32;

	tlist[11].v1 = 33;
	tlist[11].v2 = 34;
	tlist[11].v3 = 35;

	pVerts[0].x  = 0.5;
	pVerts[0].y  = 0.5;
	pVerts[0].z  = 0.5;
	pVerts[0].s  =  0.5;
	pVerts[0].t  =  0.5;
	pVerts[0].nx = 0;
	pVerts[0].ny = 0.5;
	pVerts[0].nz =  0;

	pVerts[1].x  = 0.5;
	pVerts[1].y  = 0.5;
	pVerts[1].z  = -0.5;
	pVerts[1].s  =  0.5;
	pVerts[1].t  =  0;
	pVerts[1].nx = 0;
	pVerts[1].ny = 0.5;
	pVerts[1].nz =  0;

	pVerts[2].x  = -0.5;
	pVerts[2].y  = 0.5;
	pVerts[2].z  = -0.5;
	pVerts[2].s  =  0;
	pVerts[2].t  =  0;
	pVerts[2].nx = 0;
	pVerts[2].ny = 0.5;
	pVerts[2].nz =  0;

	pVerts[3].x  = 0.5;
	pVerts[3].y  = 0.5;
	pVerts[3].z  = 0.5;
	pVerts[3].s  = 0.5;
	pVerts[3].t  = 0.5;
	pVerts[3].nx = 0;
	pVerts[3].ny = 0.5;
	pVerts[3].nz = 0;

	pVerts[4].x  = -0.5;
	pVerts[4].y  = 0.5;
	pVerts[4].z  = -0.5;
	pVerts[4].s  =  0;
	pVerts[4].t  =  0;
	pVerts[4].nx = 0;
	pVerts[4].ny = 0.5;
	pVerts[4].nz =  0;

	pVerts[5].x  = -0.5;
	pVerts[5].y  = 0.5;
	pVerts[5].z  = 0.5;
	pVerts[5].s  = 0;
	pVerts[5].t  = 0.5;
	pVerts[5].nx = 0;
	pVerts[5].ny = 0.5;
	pVerts[5].nz = 0;

	pVerts[6].x  = -0.5;
	pVerts[6].y  = -0.5;
	pVerts[6].z  = -0.5;
	pVerts[6].s  =  0;
	pVerts[6].t  =  0;
	pVerts[6].nx = 0;
	pVerts[6].ny = -0.5;
	pVerts[6].nz =  0;

	pVerts[7].x  = 0.5;
	pVerts[7].y  = -0.5;
	pVerts[7].z  = -0.5;
	pVerts[7].s  =  0.5;
	pVerts[7].t  =  0;
	pVerts[7].nx = 0;
	pVerts[7].ny = -0.5;
	pVerts[7].nz =  0;

	pVerts[8].x  = 0.5;
	pVerts[8].y  = -0.5;
	pVerts[8].z  = 0.5;
	pVerts[8].s  =  0.5;
	pVerts[8].t  =  0.5;
	pVerts[8].nx = 0;
	pVerts[8].ny = -0.5;
	pVerts[8].nz =  0;

	pVerts[9].x  = -0.5;
	pVerts[9].y  = -0.5;
	pVerts[9].z  = 0.5;
	pVerts[9].s  =  0;
	pVerts[9].t  =  0.5;
	pVerts[9].nx = 0;
	pVerts[9].ny = -0.5;
	pVerts[9].nz =  0;

	pVerts[10].x  = -0.5;
	pVerts[10].y  = -0.5;
	pVerts[10].z  = -0.5;
	pVerts[10].s  =  0;
	pVerts[10].t  =  0;
	pVerts[10].nx = 0;
	pVerts[10].ny = -0.5;
	pVerts[10].nz =  0;

	pVerts[11].x  = 0.5;
	pVerts[11].y  = -0.5;
	pVerts[11].z  = 0.5;
	pVerts[11].s  =  0.5;
	pVerts[11].t  =  0.5;
	pVerts[11].nx = 0;
	pVerts[11].ny = -0.5;
	pVerts[11].nz =  0;

	pVerts[12].x  = -0.5;
	pVerts[12].y  = 0.5;
	pVerts[12].z  = 0.5;
	pVerts[12].s  =  0.5;
	pVerts[12].t  =  0.5;
	pVerts[12].nx = -0.5;
	pVerts[12].ny = 0;
	pVerts[12].nz = 0;

	pVerts[13].x  = -0.5;
	pVerts[13].y  = 0.5;
	pVerts[13].z  = -0.5;
	pVerts[13].s  =  0.5;
	pVerts[13].t  =  0;
	pVerts[13].nx = -0.5;
	pVerts[13].ny = 0;
	pVerts[13].nz =  0;

	pVerts[14].x  = -0.5;
	pVerts[14].y  = -0.5;
	pVerts[14].z  = -0.5;
	pVerts[14].s  =  0;
	pVerts[14].t  =  0;
	pVerts[14].nx = -0.5;
	pVerts[14].ny = 0;
	pVerts[14].nz = 0;

	pVerts[15].x  = -0.5;
	pVerts[15].y  = 0.5;
	pVerts[15].z  = 0.5;
	pVerts[15].s  =  0.5;
	pVerts[15].t  =  0.5;
	pVerts[15].nx = -0.5;
	pVerts[15].ny = 0;
	pVerts[15].nz = 0;

	pVerts[16].x  = -0.5;
	pVerts[16].y  = -0.5;
	pVerts[16].z  = -0.5;
	pVerts[16].s  =  0;
	pVerts[16].t  =  0;
	pVerts[16].nx = -0.5;
	pVerts[16].ny = 0;
	pVerts[16].nz =  0;

	pVerts[17].x  = -0.5;
	pVerts[17].y  = -0.5;
	pVerts[17].z  = 0.5;
	pVerts[17].s  =  0;
	pVerts[17].t  =  0.5;
	pVerts[17].nx = -0.5;
	pVerts[17].ny = 0;
	pVerts[17].nz =  0;

	pVerts[18].x  = 0.5;
	pVerts[18].y  = -0.5;
	pVerts[18].z  = -0.5;
	pVerts[18].s  =  0;
	pVerts[18].t  =  0;
	pVerts[18].nx = 0.5;
	pVerts[18].ny = 0;
	pVerts[18].nz = 0;

	pVerts[19].x  = 0.5;
	pVerts[19].y  = 0.5;
	pVerts[19].z  = -0.5;
	pVerts[19].s  = 0.5;
	pVerts[19].t  =  0;
	pVerts[19].nx = 0.5;
	pVerts[19].ny = 0;
	pVerts[19].nz = 0;

	pVerts[20].x  = 0.5;
	pVerts[20].y  = 0.5;
	pVerts[20].z  = 0.5;
	pVerts[20].s  = 0.5;
	pVerts[20].t  = 0.5;
	pVerts[20].nx = 0.5;
	pVerts[20].ny = 0;
	pVerts[20].nz = 0;

	pVerts[21].x  = 0.5;
	pVerts[21].y  = 0.5;
	pVerts[21].z  = 0.5;
	pVerts[21].s  = 0.5;
	pVerts[21].t  = 0.5;
	pVerts[21].nx = 0.5;
	pVerts[21].ny = 0;
	pVerts[21].nz = 0;

	pVerts[22].x  = 0.5;
	pVerts[22].y  = -0.5;
	pVerts[22].z  = 0.5;
	pVerts[22].s  =  0;
	pVerts[22].t  =  0.5;
	pVerts[22].nx = 0.5;
	pVerts[22].ny = 0;
	pVerts[22].nz = 0;
		  
	pVerts[23].x  = 0.5;
	pVerts[23].y  = -0.5;
	pVerts[23].z  = -0.5;
	pVerts[23].s  =  0;
	pVerts[23].t  =  0;
	pVerts[23].nx = 0.5;
	pVerts[23].ny = 0;
	pVerts[23].nz = 0;
		   
	pVerts[24].x  = 0.5;
	pVerts[24].y  = -0.5;
	pVerts[24].z  = 0.5;
	pVerts[24].s  =  0.5;
	pVerts[24].t  =  0;
	pVerts[24].nx = 0;
	pVerts[24].ny = 0;
	pVerts[24].nz =  0.5;
		  
	pVerts[25].x  = 0.5;
	pVerts[25].y  = 0.5;
	pVerts[25].z  = 0.5;
	pVerts[25].s  = 0.5;
	pVerts[25].t  = 0.5;
	pVerts[25].nx = 0;
	pVerts[25].ny = 0;
	pVerts[25].nz = 0.5;
		   
	pVerts[26].x  = -0.5;
	pVerts[26].y  = 0.5;
	pVerts[26].z  = 0.5;
	pVerts[26].s  =  0;
	pVerts[26].t  =  0.5;
	pVerts[26].nx = 0;
	pVerts[26].ny = 0;
	pVerts[26].nz = 0.5;
		  
	pVerts[27].x  = -0.5;
	pVerts[27].y  = 0.5;
	pVerts[27].z  = 0.5;
	pVerts[27].s  =  0;
	pVerts[27].t  =  0.5;
	pVerts[27].nx = 0;
	pVerts[27].ny = 0;
	pVerts[27].nz = 0.5;
		   
	pVerts[28].x  = -0.5;
	pVerts[28].y  = -0.5;
	pVerts[28].z  = 0.5;
	pVerts[28].s  =  0;
	pVerts[28].t  =  0;
	pVerts[28].nx = 0;
	pVerts[28].ny = 0;
	pVerts[28].nz = 0.5;
		   
	pVerts[29].x  = 0.5;
	pVerts[29].y  = -0.5;
	pVerts[29].z  = 0.5;
	pVerts[29].s  =  0.5;
	pVerts[29].t  =  0;
	pVerts[29].nx = 0;
	pVerts[29].ny = 0;
	pVerts[29].nz = 0.5;

	pVerts[30].x  = 0.5;
	pVerts[30].y  = -0.5;
	pVerts[30].z  = -0.5;
	pVerts[30].s  =  0.5;
	pVerts[30].t  =  0;
	pVerts[30].nx = 0;
	pVerts[30].ny = 0;
	pVerts[30].nz = -0.5;
		   
	pVerts[31].x  = -0.5;
	pVerts[31].y  = -0.5;
	pVerts[31].z  = -0.5;
	pVerts[31].s  =  0;
	pVerts[31].t  =  0;
	pVerts[31].nx = 0;
	pVerts[31].ny = 0;
	pVerts[31].nz = -0.5;
		   
	pVerts[32].x  = -0.5;
	pVerts[32].y  = 0.5;
	pVerts[32].z  = -0.5;
	pVerts[32].s  =  0;
	pVerts[32].t  =  0.5;
	pVerts[32].nx = 0;
	pVerts[32].ny = 0;
	pVerts[32].nz = -0.5;
		   
	pVerts[33].x  = -0.5;
	pVerts[33].y  = 0.5;
	pVerts[33].z  = -0.5;
	pVerts[33].s  =  0;
	pVerts[33].t  =  0.5;
	pVerts[33].nx = 0;
	pVerts[33].ny = 0;
	pVerts[33].nz = -0.5;
		   
	pVerts[34].x  = 0.5;
	pVerts[34].y  = 0.5;
	pVerts[34].z  = -0.5;
	pVerts[34].s  =  0.5;
	pVerts[34].t  =  0.5;
	pVerts[34].nx = 0;
	pVerts[34].ny = 0;
	pVerts[34].nz = -0.5;
		   
	pVerts[35].x  = 0.5;
	pVerts[35].y  = -0.5;
	pVerts[35].z  = -0.5;
	pVerts[35].s  =  0.5;
	pVerts[35].t  =  0;
	pVerts[35].nx = 0;
	pVerts[35].ny = 0;
	pVerts[35].nz = -0.5;

	//----------- WRITE ------------------------------------------
	// 1. Get the existing header

	// open file
	ferror = File::open( fh, "cube.cdm", FILE_READ );
	assert( ferror == FILE_SUCCESS );

	gObjFileHdr cubeHdr;

	// write data of header to cubeHdr
	ferror = File::read( fh, &cubeHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

	// close
	ferror = File::close( fh );

		// 2. Write the data to the file
	
	// create file if not done already, start with header
	ferror = File::open( fh, "cube.cdm", FILE_READ_WRITE );
	assert (ferror == FILE_SUCCESS);

	// write the header
	ferror = File::write( fh, &cubeHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

		// update header with numVerts, VertBufferOffset
	cubeHdr.numVerts = 36;
	ferror = File::tell( fh, cubeHdr.vertBufferOffset );
	assert (ferror == FILE_SUCCESS);

		// write the vertex data
	ferror = File::write( fh, pVerts, 36 * sizeof(MyVertex_stride) );
	assert (ferror == FILE_SUCCESS);

		// update header with numTriList, triListBufferOffset
	cubeHdr.numTriList = 12;
	ferror = File::tell( fh, cubeHdr.triListBufferOffset );
	assert (ferror == FILE_SUCCESS);

		// write the triList data
	ferror = File::write( fh, tlist, 12 * sizeof(MyTriList) );
	assert (ferror == FILE_SUCCESS);

		// fix header with updated data
	ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
	assert (ferror == FILE_SUCCESS);

	ferror = File::write( fh, &cubeHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

		// close file
	ferror = File::close( fh );
	assert (ferror == FILE_SUCCESS);
		   
#endif

// -------------------------------------------------------------------

	/* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &this->vao);
 
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(this->vao);

    //GLuint vbo;
    GLuint vbo[2];

    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);
 
    // Load the combined data: ---------------------------------------------------------

		/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
 
		/* Copy the vertex data to our buffer */
		// glBufferData(type, size in bytes, data, usage) 
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * 36, pVerts, GL_STATIC_DRAW);
		
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList) * 12, tlist, GL_STATIC_DRAW);
}