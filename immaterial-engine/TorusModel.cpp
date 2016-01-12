
// MY HEADERS
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "TorusModel.h"
#include "MathEngine.h"
#include "File.h"
#include "GraphicsObjectFileHdr.h"

// GL GLOBALS
extern GLShaderManager shaderManager;

// DEFINES
#define USE_TORUS_FILE 1 // 1 - uses the data file "torus.cdm", 0 - creates and uses the file "sphere.cdm".

TorusModel::TorusModel()
{
	out("TorusModel(): ---------------\n");
}

void TorusModel::createVAO(void)
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

#if USE_TORUS_FILE

	ferror = File::open( fh, "torus.cdm", FILE_READ );
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
	MyTriList tlist[220];
	MyVertex_stride pVerts[132];

	tlist[0].v1 = 0;
	tlist[0].v2 = 1;
	tlist[0].v3 = 2;

	tlist[1].v1 = 1;
	tlist[1].v2 = 3;
	tlist[1].v3 = 2;

	tlist[2].v1 = 2;
	tlist[2].v2 = 3;
	tlist[2].v3 = 4;

	tlist[3].v1 = 3;
	tlist[3].v2 = 5;
	tlist[3].v3 = 4;

	tlist[4].v1 = 4;
	tlist[4].v2 = 5;
	tlist[4].v3 = 6;

	tlist[5].v1 = 5;
	tlist[5].v2 = 7;
	tlist[5].v3 = 6;

	tlist[6].v1 = 6;
	tlist[6].v2 = 7;
	tlist[6].v3 = 8;

	tlist[7].v1 = 7;
	tlist[7].v2 = 9;
	tlist[7].v3 = 8;

	tlist[8].v1 = 8;
	tlist[8].v2 = 9;
	tlist[8].v3 = 10;

	tlist[9].v1 = 9;
	tlist[9].v2 = 11;
	tlist[9].v3 = 10;

	tlist[10].v1 = 10;
	tlist[10].v2 = 11;
	tlist[10].v3 = 12;

	tlist[11].v1 = 11;
	tlist[11].v2 = 13;
	tlist[11].v3 = 12;

	tlist[12].v1 = 12;
	tlist[12].v2 = 13;
	tlist[12].v3 = 14;

	tlist[13].v1 = 13;
	tlist[13].v2 = 15;
	tlist[13].v3 = 14;

	tlist[14].v1 = 14;
	tlist[14].v2 = 15;
	tlist[14].v3 = 16;

	tlist[15].v1 = 15;
	tlist[15].v2 = 17;
	tlist[15].v3 = 16;

	tlist[16].v1 = 16;
	tlist[16].v2 = 17;
	tlist[16].v3 = 18;

	tlist[17].v1 = 17;
	tlist[17].v2 = 19;
	tlist[17].v3 = 18;

	tlist[18].v1 = 18;
	tlist[18].v2 = 19;
	tlist[18].v3 = 20;

	tlist[19].v1 = 19;
	tlist[19].v2 = 21;
	tlist[19].v3 = 20;

	tlist[20].v1 = 20;
	tlist[20].v2 = 21;
	tlist[20].v3 = 22;

	tlist[21].v1 = 21;
	tlist[21].v2 = 23;
	tlist[21].v3 = 22;

	tlist[22].v1 = 1;
	tlist[22].v2 = 24;
	tlist[22].v3 = 3;

	tlist[23].v1 = 24;
	tlist[23].v2 = 25;
	tlist[23].v3 = 3;

	tlist[24].v1 = 3;
	tlist[24].v2 = 25;
	tlist[24].v3 = 5;

	tlist[25].v1 = 25;
	tlist[25].v2 = 26;
	tlist[25].v3 = 5;

	tlist[26].v1 = 5;
	tlist[26].v2 = 26;
	tlist[26].v3 = 7;

	tlist[27].v1 = 26;
	tlist[27].v2 = 27;
	tlist[27].v3 = 7;

	tlist[28].v1 = 7;
	tlist[28].v2 = 27;
	tlist[28].v3 = 9;

	tlist[29].v1 = 27;
	tlist[29].v2 = 28;
	tlist[29].v3 = 9;

	tlist[30].v1 = 9;
	tlist[30].v2 = 28;
	tlist[30].v3 = 11;

	tlist[31].v1 = 28;
	tlist[31].v2 = 29;
	tlist[31].v3 = 11;

	tlist[32].v1 = 11;
	tlist[32].v2 = 29;
	tlist[32].v3 = 13;

	tlist[33].v1 = 29;
	tlist[33].v2 = 30;
	tlist[33].v3 = 13;

	tlist[34].v1 = 13;
	tlist[34].v2 = 30;
	tlist[34].v3 = 15;

	tlist[35].v1 = 30;
	tlist[35].v2 = 31;
	tlist[35].v3 = 15;

	tlist[36].v1 = 15;
	tlist[36].v2 = 31;
	tlist[36].v3 = 17;

	tlist[37].v1 = 31;
	tlist[37].v2 = 32;
	tlist[37].v3 = 17;

	tlist[38].v1 = 17;
	tlist[38].v2 = 32;
	tlist[38].v3 = 19;

	tlist[39].v1 = 32;
	tlist[39].v2 = 33;
	tlist[39].v3 = 19;

	tlist[40].v1 = 19;
	tlist[40].v2 = 33;
	tlist[40].v3 = 21;

	tlist[41].v1 = 33;
	tlist[41].v2 = 34;
	tlist[41].v3 = 21;

	tlist[42].v1 = 21;
	tlist[42].v2 = 34;
	tlist[42].v3 = 23;

	tlist[43].v1 = 34;
	tlist[43].v2 = 35;
	tlist[43].v3 = 23;

	tlist[44].v1 = 24;
	tlist[44].v2 = 36;
	tlist[44].v3 = 25;

	tlist[45].v1 = 36;
	tlist[45].v2 = 37;
	tlist[45].v3 = 25;

	tlist[46].v1 = 25;
	tlist[46].v2 = 37;
	tlist[46].v3 = 26;

	tlist[47].v1 = 37;
	tlist[47].v2 = 38;
	tlist[47].v3 = 26;

	tlist[48].v1 = 26;
	tlist[48].v2 = 38;
	tlist[48].v3 = 27;

	tlist[49].v1 = 38;
	tlist[49].v2 = 39;
	tlist[49].v3 = 27;

	tlist[50].v1 = 27;
	tlist[50].v2 = 39;
	tlist[50].v3 = 28;

	tlist[51].v1 = 39;
	tlist[51].v2 = 40;
	tlist[51].v3 = 28;

	tlist[52].v1 = 28;
	tlist[52].v2 = 40;
	tlist[52].v3 = 29;

	tlist[53].v1 = 40;
	tlist[53].v2 = 41;
	tlist[53].v3 = 29;

	tlist[54].v1 = 29;
	tlist[54].v2 = 41;
	tlist[54].v3 = 30;

	tlist[55].v1 = 41;
	tlist[55].v2 = 42;
	tlist[55].v3 = 30;

	tlist[56].v1 = 30;
	tlist[56].v2 = 42;
	tlist[56].v3 = 31;

	tlist[57].v1 = 42;
	tlist[57].v2 = 43;
	tlist[57].v3 = 31;

	tlist[58].v1 = 31;
	tlist[58].v2 = 43;
	tlist[58].v3 = 32;

	tlist[59].v1 = 43;
	tlist[59].v2 = 44;
	tlist[59].v3 = 32;

	tlist[60].v1 = 32;
	tlist[60].v2 = 44;
	tlist[60].v3 = 33;

	tlist[61].v1 = 44;
	tlist[61].v2 = 45;
	tlist[61].v3 = 33;

	tlist[62].v1 = 33;
	tlist[62].v2 = 45;
	tlist[62].v3 = 34;

	tlist[63].v1 = 45;
	tlist[63].v2 = 46;
	tlist[63].v3 = 34;

	tlist[64].v1 = 34;
	tlist[64].v2 = 46;
	tlist[64].v3 = 35;

	tlist[65].v1 = 46;
	tlist[65].v2 = 47;
	tlist[65].v3 = 35;

	tlist[66].v1 = 36;
	tlist[66].v2 = 48;
	tlist[66].v3 = 37;

	tlist[67].v1 = 48;
	tlist[67].v2 = 49;
	tlist[67].v3 = 37;

	tlist[68].v1 = 37;
	tlist[68].v2 = 49;
	tlist[68].v3 = 38;

	tlist[69].v1 = 49;
	tlist[69].v2 = 50;
	tlist[69].v3 = 38;

	tlist[70].v1 = 38;
	tlist[70].v2 = 50;
	tlist[70].v3 = 39;

	tlist[71].v1 = 50;
	tlist[71].v2 = 51;
	tlist[71].v3 = 39;

	tlist[72].v1 = 39;
	tlist[72].v2 = 51;
	tlist[72].v3 = 40;

	tlist[73].v1 = 51;
	tlist[73].v2 = 52;
	tlist[73].v3 = 40;

	tlist[74].v1 = 40;
	tlist[74].v2 = 52;
	tlist[74].v3 = 41;

	tlist[75].v1 = 52;
	tlist[75].v2 = 53;
	tlist[75].v3 = 41;

	tlist[76].v1 = 41;
	tlist[76].v2 = 53;
	tlist[76].v3 = 42;

	tlist[77].v1 = 53;
	tlist[77].v2 = 54;
	tlist[77].v3 = 42;

	tlist[78].v1 = 42;
	tlist[78].v2 = 54;
	tlist[78].v3 = 43;

	tlist[79].v1 = 54;
	tlist[79].v2 = 55;
	tlist[79].v3 = 43;

	tlist[80].v1 = 43;
	tlist[80].v2 = 55;
	tlist[80].v3 = 44;

	tlist[81].v1 = 55;
	tlist[81].v2 = 56;
	tlist[81].v3 = 44;

	tlist[82].v1 = 44;
	tlist[82].v2 = 56;
	tlist[82].v3 = 45;

	tlist[83].v1 = 56;
	tlist[83].v2 = 57;
	tlist[83].v3 = 45;

	tlist[84].v1 = 45;
	tlist[84].v2 = 57;
	tlist[84].v3 = 46;

	tlist[85].v1 = 57;
	tlist[85].v2 = 58;
	tlist[85].v3 = 46;

	tlist[86].v1 = 46;
	tlist[86].v2 = 58;
	tlist[86].v3 = 47;

	tlist[87].v1 = 58;
	tlist[87].v2 = 59;
	tlist[87].v3 = 47;

	tlist[88].v1 = 48;
	tlist[88].v2 = 60;
	tlist[88].v3 = 49;

	tlist[89].v1 = 60;
	tlist[89].v2 = 61;
	tlist[89].v3 = 49;

	tlist[90].v1 = 49;
	tlist[90].v2 = 61;
	tlist[90].v3 = 50;

	tlist[91].v1 = 61;
	tlist[91].v2 = 62;
	tlist[91].v3 = 50;

	tlist[92].v1 = 50;
	tlist[92].v2 = 62;
	tlist[92].v3 = 51;

	tlist[93].v1 = 62;
	tlist[93].v2 = 63;
	tlist[93].v3 = 51;

	tlist[94].v1 = 51;
	tlist[94].v2 = 63;
	tlist[94].v3 = 52;

	tlist[95].v1 = 63;
	tlist[95].v2 = 64;
	tlist[95].v3 = 52;

	tlist[96].v1 = 52;
	tlist[96].v2 = 64;
	tlist[96].v3 = 53;

	tlist[97].v1 = 64;
	tlist[97].v2 = 65;
	tlist[97].v3 = 53;

	tlist[98].v1 = 53;
	tlist[98].v2 = 65;
	tlist[98].v3 = 54;

	tlist[99].v1 = 65;
	tlist[99].v2 = 66;
	tlist[99].v3 = 54;

	tlist[100].v1 = 54;
	tlist[100].v2 = 66;
	tlist[100].v3 = 55;

	tlist[101].v1 = 66;
	tlist[101].v2 = 67;
	tlist[101].v3 = 55;

	tlist[102].v1 = 55;
	tlist[102].v2 = 67;
	tlist[102].v3 = 56;

	tlist[103].v1 = 67;
	tlist[103].v2 = 68;
	tlist[103].v3 = 56;

	tlist[104].v1 = 56;
	tlist[104].v2 = 68;
	tlist[104].v3 = 57;

	tlist[105].v1 = 68;
	tlist[105].v2 = 69;
	tlist[105].v3 = 57;

	tlist[106].v1 = 57;
	tlist[106].v2 = 69;
	tlist[106].v3 = 58;

	tlist[107].v1 = 69;
	tlist[107].v2 = 70;
	tlist[107].v3 = 58;

	tlist[108].v1 = 58;
	tlist[108].v2 = 70;
	tlist[108].v3 = 59;

	tlist[109].v1 = 70;
	tlist[109].v2 = 71;
	tlist[109].v3 = 59;

	tlist[110].v1 = 60;
	tlist[110].v2 = 72;
	tlist[110].v3 = 61;

	tlist[111].v1 = 72;
	tlist[111].v2 = 73;
	tlist[111].v3 = 61;

	tlist[112].v1 = 61;
	tlist[112].v2 = 73;
	tlist[112].v3 = 62;

	tlist[113].v1 = 73;
	tlist[113].v2 = 74;
	tlist[113].v3 = 62;

	tlist[114].v1 = 62;
	tlist[114].v2 = 74;
	tlist[114].v3 = 63;

	tlist[115].v1 = 74;
	tlist[115].v2 = 75;
	tlist[115].v3 = 63;

	tlist[116].v1 = 63;
	tlist[116].v2 = 75;
	tlist[116].v3 = 64;

	tlist[117].v1 = 75;
	tlist[117].v2 = 76;
	tlist[117].v3 = 64;

	tlist[118].v1 = 64;
	tlist[118].v2 = 76;
	tlist[118].v3 = 65;

	tlist[119].v1 = 76;
	tlist[119].v2 = 77;
	tlist[119].v3 = 65;

	tlist[120].v1 = 65;
	tlist[120].v2 = 77;
	tlist[120].v3 = 66;

	tlist[121].v1 = 77;
	tlist[121].v2 = 78;
	tlist[121].v3 = 66;

	tlist[122].v1 = 66;
	tlist[122].v2 = 78;
	tlist[122].v3 = 67;

	tlist[123].v1 = 78;
	tlist[123].v2 = 79;
	tlist[123].v3 = 67;

	tlist[124].v1 = 67;
	tlist[124].v2 = 79;
	tlist[124].v3 = 68;

	tlist[125].v1 = 79;
	tlist[125].v2 = 80;
	tlist[125].v3 = 68;

	tlist[126].v1 = 68;
	tlist[126].v2 = 80;
	tlist[126].v3 = 69;

	tlist[127].v1 = 80;
	tlist[127].v2 = 81;
	tlist[127].v3 = 69;

	tlist[128].v1 = 69;
	tlist[128].v2 = 81;
	tlist[128].v3 = 70;

	tlist[129].v1 = 81;
	tlist[129].v2 = 82;
	tlist[129].v3 = 70;

	tlist[130].v1 = 70;
	tlist[130].v2 = 82;
	tlist[130].v3 = 71;

	tlist[131].v1 = 82;
	tlist[131].v2 = 83;
	tlist[131].v3 = 71;

	tlist[132].v1 = 72;
	tlist[132].v2 = 84;
	tlist[132].v3 = 73;

	tlist[133].v1 = 84;
	tlist[133].v2 = 85;
	tlist[133].v3 = 73;

	tlist[134].v1 = 73;
	tlist[134].v2 = 85;
	tlist[134].v3 = 74;

	tlist[135].v1 = 85;
	tlist[135].v2 = 86;
	tlist[135].v3 = 74;

	tlist[136].v1 = 74;
	tlist[136].v2 = 86;
	tlist[136].v3 = 75;

	tlist[137].v1 = 86;
	tlist[137].v2 = 87;
	tlist[137].v3 = 75;

	tlist[138].v1 = 75;
	tlist[138].v2 = 87;
	tlist[138].v3 = 76;

	tlist[139].v1 = 87;
	tlist[139].v2 = 88;
	tlist[139].v3 = 76;

	tlist[140].v1 = 76;
	tlist[140].v2 = 88;
	tlist[140].v3 = 77;

	tlist[141].v1 = 88;
	tlist[141].v2 = 89;
	tlist[141].v3 = 77;

	tlist[142].v1 = 77;
	tlist[142].v2 = 89;
	tlist[142].v3 = 78;

	tlist[143].v1 = 89;
	tlist[143].v2 = 90;
	tlist[143].v3 = 78;

	tlist[144].v1 = 78;
	tlist[144].v2 = 90;
	tlist[144].v3 = 79;

	tlist[145].v1 = 90;
	tlist[145].v2 = 91;
	tlist[145].v3 = 79;

	tlist[146].v1 = 79;
	tlist[146].v2 = 91;
	tlist[146].v3 = 80;

	tlist[147].v1 = 91;
	tlist[147].v2 = 92;
	tlist[147].v3 = 80;

	tlist[148].v1 = 80;
	tlist[148].v2 = 92;
	tlist[148].v3 = 81;

	tlist[149].v1 = 92;
	tlist[149].v2 = 93;
	tlist[149].v3 = 81;

	tlist[150].v1 = 81;
	tlist[150].v2 = 93;
	tlist[150].v3 = 82;

	tlist[151].v1 = 93;
	tlist[151].v2 = 94;
	tlist[151].v3 = 82;

	tlist[152].v1 = 82;
	tlist[152].v2 = 94;
	tlist[152].v3 = 83;

	tlist[153].v1 = 94;
	tlist[153].v2 = 95;
	tlist[153].v3 = 83;

	tlist[154].v1 = 84;
	tlist[154].v2 = 96;
	tlist[154].v3 = 85;

	tlist[155].v1 = 96;
	tlist[155].v2 = 97;
	tlist[155].v3 = 85;

	tlist[156].v1 = 85;
	tlist[156].v2 = 97;
	tlist[156].v3 = 86;

	tlist[157].v1 = 97;
	tlist[157].v2 = 98;
	tlist[157].v3 = 86;

	tlist[158].v1 = 86;
	tlist[158].v2 = 98;
	tlist[158].v3 = 87;

	tlist[159].v1 = 98;
	tlist[159].v2 = 99;
	tlist[159].v3 = 87;

	tlist[160].v1 = 87;
	tlist[160].v2 = 99;
	tlist[160].v3 = 88;

	tlist[161].v1 = 99;
	tlist[161].v2 = 100;
	tlist[161].v3 = 88;

	tlist[162].v1 = 88;
	tlist[162].v2 = 100;
	tlist[162].v3 = 89;

	tlist[163].v1 = 100;
	tlist[163].v2 = 101;
	tlist[163].v3 = 89;

	tlist[164].v1 = 89;
	tlist[164].v2 = 101;
	tlist[164].v3 = 90;

	tlist[165].v1 = 101;
	tlist[165].v2 = 102;
	tlist[165].v3 = 90;

	tlist[166].v1 = 90;
	tlist[166].v2 = 102;
	tlist[166].v3 = 91;

	tlist[167].v1 = 102;
	tlist[167].v2 = 103;
	tlist[167].v3 = 91;

	tlist[168].v1 = 91;
	tlist[168].v2 = 103;
	tlist[168].v3 = 92;

	tlist[169].v1 = 103;
	tlist[169].v2 = 104;
	tlist[169].v3 = 92;

	tlist[170].v1 = 92;
	tlist[170].v2 = 104;
	tlist[170].v3 = 93;

	tlist[171].v1 = 104;
	tlist[171].v2 = 105;
	tlist[171].v3 = 93;

	tlist[172].v1 = 93;
	tlist[172].v2 = 105;
	tlist[172].v3 = 94;

	tlist[173].v1 = 105;
	tlist[173].v2 = 106;
	tlist[173].v3 = 94;

	tlist[174].v1 = 94;
	tlist[174].v2 = 106;
	tlist[174].v3 = 95;

	tlist[175].v1 = 106;
	tlist[175].v2 = 107;
	tlist[175].v3 = 95;

	tlist[176].v1 = 96;
	tlist[176].v2 = 108;
	tlist[176].v3 = 97;

	tlist[177].v1 = 108;
	tlist[177].v2 = 109;
	tlist[177].v3 = 97;

	tlist[178].v1 = 97;
	tlist[178].v2 = 109;
	tlist[178].v3 = 98;

	tlist[179].v1 = 109;
	tlist[179].v2 = 110;
	tlist[179].v3 = 98;

	tlist[180].v1 = 98;
	tlist[180].v2 = 110;
	tlist[180].v3 = 99;

	tlist[181].v1 = 110;
	tlist[181].v2 = 111;
	tlist[181].v3 = 99;

	tlist[182].v1 = 99;
	tlist[182].v2 = 111;
	tlist[182].v3 = 100;

	tlist[183].v1 = 111;
	tlist[183].v2 = 112;
	tlist[183].v3 = 100;

	tlist[184].v1 = 100;
	tlist[184].v2 = 112;
	tlist[184].v3 = 101;

	tlist[185].v1 = 112;
	tlist[185].v2 = 113;
	tlist[185].v3 = 101;

	tlist[186].v1 = 101;
	tlist[186].v2 = 113;
	tlist[186].v3 = 102;

	tlist[187].v1 = 113;
	tlist[187].v2 = 114;
	tlist[187].v3 = 102;

	tlist[188].v1 = 102;
	tlist[188].v2 = 114;
	tlist[188].v3 = 103;

	tlist[189].v1 = 114;
	tlist[189].v2 = 115;
	tlist[189].v3 = 103;

	tlist[190].v1 = 103;
	tlist[190].v2 = 115;
	tlist[190].v3 = 104;

	tlist[191].v1 = 115;
	tlist[191].v2 = 116;
	tlist[191].v3 = 104;

	tlist[192].v1 = 104;
	tlist[192].v2 = 116;
	tlist[192].v3 = 105;

	tlist[193].v1 = 116;
	tlist[193].v2 = 117;
	tlist[193].v3 = 105;

	tlist[194].v1 = 105;
	tlist[194].v2 = 117;
	tlist[194].v3 = 106;

	tlist[195].v1 = 117;
	tlist[195].v2 = 118;
	tlist[195].v3 = 106;

	tlist[196].v1 = 106;
	tlist[196].v2 = 118;
	tlist[196].v3 = 107;

	tlist[197].v1 = 118;
	tlist[197].v2 = 119;
	tlist[197].v3 = 107;

	tlist[198].v1 = 108;
	tlist[198].v2 = 120;
	tlist[198].v3 = 109;

	tlist[199].v1 = 120;
	tlist[199].v2 = 121;
	tlist[199].v3 = 109;

	tlist[200].v1 = 109;
	tlist[200].v2 = 121;
	tlist[200].v3 = 110;

	tlist[201].v1 = 121;
	tlist[201].v2 = 122;
	tlist[201].v3 = 110;

	tlist[202].v1 = 110;
	tlist[202].v2 = 122;
	tlist[202].v3 = 111;

	tlist[203].v1 = 122;
	tlist[203].v2 = 123;
	tlist[203].v3 = 111;

	tlist[204].v1 = 111;
	tlist[204].v2 = 123;
	tlist[204].v3 = 112;

	tlist[205].v1 = 123;
	tlist[205].v2 = 124;
	tlist[205].v3 = 112;

	tlist[206].v1 = 112;
	tlist[206].v2 = 124;
	tlist[206].v3 = 113;

	tlist[207].v1 = 124;
	tlist[207].v2 = 125;
	tlist[207].v3 = 113;

	tlist[208].v1 = 113;
	tlist[208].v2 = 125;
	tlist[208].v3 = 114;

	tlist[209].v1 = 125;
	tlist[209].v2 = 126;
	tlist[209].v3 = 114;

	tlist[210].v1 = 114;
	tlist[210].v2 = 126;
	tlist[210].v3 = 115;

	tlist[211].v1 = 126;
	tlist[211].v2 = 127;
	tlist[211].v3 = 115;

	tlist[212].v1 = 115;
	tlist[212].v2 = 127;
	tlist[212].v3 = 116;

	tlist[213].v1 = 127;
	tlist[213].v2 = 128;
	tlist[213].v3 = 116;

	tlist[214].v1 = 116;
	tlist[214].v2 = 128;
	tlist[214].v3 = 117;

	tlist[215].v1 = 128;
	tlist[215].v2 = 129;
	tlist[215].v3 = 117;

	tlist[216].v1 = 117;
	tlist[216].v2 = 129;
	tlist[216].v3 = 118;

	tlist[217].v1 = 129;
	tlist[217].v2 = 130;
	tlist[217].v3 = 118;

	tlist[218].v1 = 118;
	tlist[218].v2 = 130;
	tlist[218].v3 = 119;

	tlist[219].v1 = 130;
	tlist[219].v2 = 131;
	tlist[219].v3 = 119;

	pVerts[0].x = 0.55f;
	pVerts[0].y = 0.0f;;
	pVerts[0].z = 0.0f;;
	pVerts[0].s = 0.0f;;
	pVerts[0].t = 0.0f;;
	pVerts[0].nx = 1.0f;;
	pVerts[0].ny = 0.0f;;
	pVerts[0].nz = 0.0f;;

	pVerts[1].x = 0.444959f;
	pVerts[1].y = 0.323282f;
	pVerts[1].z = 0.0f;;
	pVerts[1].s = 0.1f;
	pVerts[1].t = 0.0f;;
	pVerts[1].nx = 0.809017f;
	pVerts[1].ny = 0.587785f;
	pVerts[1].nz = 0.0f;;

	pVerts[2].x = 0.521353f;
	pVerts[2].y = 0.0f;;
	pVerts[2].z = 0.088168f;
	pVerts[2].s = 0.0f;;
	pVerts[2].t = 0.1f;
	pVerts[2].nx = 0.809017f;
	pVerts[2].ny = 0.0f;;
	pVerts[2].nz = 0.587785f;

	pVerts[3].x = 0.421783f;
	pVerts[3].y = 0.306443f;
	pVerts[3].z = 0.088168f;
	pVerts[3].s = 0.1f;
	pVerts[3].t = 0.1f;
	pVerts[3].nx = 0.654509f;
	pVerts[3].ny = 0.475528f;
	pVerts[3].nz = 0.587785f;

	pVerts[4].x = 0.446353f;
	pVerts[4].y = 0.0f;;
	pVerts[4].z = 0.142658f;
	pVerts[4].s = 0.0f;;
	pVerts[4].t = 0.2f;
	pVerts[4].nx = 0.309017f;
	pVerts[4].ny = 0.0f;;
	pVerts[4].nz = 0.951057f;

	pVerts[5].x = 0.361107f;
	pVerts[5].y = 0.262359f;
	pVerts[5].z = 0.142658f;
	pVerts[5].s = 0.1f;
	pVerts[5].t = 0.2f;
	pVerts[5].nx = 0.25f;
	pVerts[5].ny = 0.181636f;
	pVerts[5].nz = 0.951057f;

	pVerts[6].x = 0.353647f;
	pVerts[6].y = 0.0f;;
	pVerts[6].z = 0.142658f;
	pVerts[6].s = 0.0f;;
	pVerts[6].t = 0.3f;
	pVerts[6].nx = -0.309017f;
	pVerts[6].ny = 0.0f;;
	pVerts[6].nz = 0.951057f;

	pVerts[7].x = 0.286107f;
	pVerts[7].y = 0.207869f;
	pVerts[7].z = 0.142658f;
	pVerts[7].s = 0.1f;
	pVerts[7].t = 0.3f;
	pVerts[7].nx = -0.25f;
	pVerts[7].ny = -0.181636f;
	pVerts[7].nz = 0.951057f;

	pVerts[8].x = 0.278647f;
	pVerts[8].y = 0.0f;;
	pVerts[8].z = 0.088168f;
	pVerts[8].s = 0.0f;;
	pVerts[8].t = 0.4f;
	pVerts[8].nx = -0.809017f;
	pVerts[8].ny = 0.0f;;
	pVerts[8].nz = 0.587785f;

	pVerts[9].x = 0.225431f;
	pVerts[9].y = 0.163785f;
	pVerts[9].z = 0.088168f;
	pVerts[9].s = 0.1f;
	pVerts[9].t = 0.4f;
	pVerts[9].nx = -0.654509f;
	pVerts[9].ny = -0.475528f;
	pVerts[9].nz = 0.587785f;

	pVerts[10].x = 0.25f;
	pVerts[10].y = 0.0f;;
	pVerts[10].z = 0.0f;;
	pVerts[10].s = 0.0f;;
	pVerts[10].t = 0.5f;
	pVerts[10].nx = -1.0f;;
	pVerts[10].ny = 0.0f;;
	pVerts[10].nz = 0.0f;;

	pVerts[11].x = 0.202254f;
	pVerts[11].y = 0.146946f;
	pVerts[11].z = 0.0f;;
	pVerts[11].s = 0.1f;
	pVerts[11].t = 0.5f;
	pVerts[11].nx = -0.809017f;
	pVerts[11].ny = -0.587785f;
	pVerts[11].nz = 0.0f;;

	pVerts[12].x = 0.278647f;
	pVerts[12].y = 0.0f;;
	pVerts[12].z = -0.088168f;
	pVerts[12].s = 0.0f;;
	pVerts[12].t = 0.6f;
	pVerts[12].nx = -0.809017f;
	pVerts[12].ny = 0.0f;;
	pVerts[12].nz = -0.587785f;

	pVerts[13].x = 0.225431f;
	pVerts[13].y = 0.163785f;
	pVerts[13].z = -0.088168f;
	pVerts[13].s = 0.1f;
	pVerts[13].t = 0.6f;
	pVerts[13].nx = -0.654508f;
	pVerts[13].ny = -0.475528f;
	pVerts[13].nz = -0.587785f;

	pVerts[14].x = 0.353647f;
	pVerts[14].y = 0.0f;;
	pVerts[14].z = -0.142658f;
	pVerts[14].s = 0.0f;;
	pVerts[14].t = 0.7f;
	pVerts[14].nx = -0.309017f;
	pVerts[14].ny = 0.0f;;
	pVerts[14].nz = -0.951057f;

	pVerts[15].x = 0.286107f;
	pVerts[15].y = 0.207869f;
	pVerts[15].z = -0.142658f;
	pVerts[15].s = 0.1f;
	pVerts[15].t = 0.7f;
	pVerts[15].nx = -0.25f;
	pVerts[15].ny = -0.181636f;
	pVerts[15].nz = -0.951057f;

	pVerts[16].x = 0.446353f;
	pVerts[16].y = 0.0f;;
	pVerts[16].z = -0.142658f;
	pVerts[16].s = 0.0f;;
	pVerts[16].t = 0.8f;
	pVerts[16].nx = 0.309017f;
	pVerts[16].ny = 0.0f;;
	pVerts[16].nz = -0.951057f;

	pVerts[17].x = 0.361107f;
	pVerts[17].y = 0.262359f;
	pVerts[17].z = -0.142658f;
	pVerts[17].s = 0.1f;
	pVerts[17].t = 0.8f;
	pVerts[17].nx = 0.25f;
	pVerts[17].ny = 0.181636f;
	pVerts[17].nz = -0.951057f;

	pVerts[18].x = 0.521353f;
	pVerts[18].y = 0.0f;;
	pVerts[18].z = -0.088168f;
	pVerts[18].s = 0.0f;;
	pVerts[18].t = 0.9f;
	pVerts[18].nx = 0.809017f;
	pVerts[18].ny = 0.0f;;
	pVerts[18].nz = -0.587785f;

	pVerts[19].x = 0.421783f;
	pVerts[19].y = 0.306443f;
	pVerts[19].z = -0.088168f;
	pVerts[19].s = 0.1f;
	pVerts[19].t = 0.9f;
	pVerts[19].nx = 0.654509f;
	pVerts[19].ny = 0.475528f;
	pVerts[19].nz = -0.587785f;

	pVerts[20].x = 0.55f;
	pVerts[20].y = 0.0f;;
	pVerts[20].z = 0.0f;;
	pVerts[20].s = 0.0f;;
	pVerts[20].t = 1.0f;;
	pVerts[20].nx = 1.0f;;
	pVerts[20].ny = 0.0f;;
	pVerts[20].nz = 0.0f;;

	pVerts[21].x = 0.444959f;
	pVerts[21].y = 0.323282f;
	pVerts[21].z = 0.0f;;
	pVerts[21].s = 0.1f;
	pVerts[21].t = 1.0f;;
	pVerts[21].nx = 0.809017f;
	pVerts[21].ny = 0.587785f;
	pVerts[21].nz = 0.0f;;

	pVerts[22].x = 0.521353f;
	pVerts[22].y = 0.0f;;
	pVerts[22].z = 0.088168f;
	pVerts[22].s = 0.0f;;
	pVerts[22].t = 1.1f;
	pVerts[22].nx = 0.809017f;
	pVerts[22].ny = 0.0f;;
	pVerts[22].nz = 0.587785f;

	pVerts[23].x = 0.421783f;
	pVerts[23].y = 0.306443f;
	pVerts[23].z = 0.088168f;
	pVerts[23].s = 0.1f;
	pVerts[23].t = 1.1f;
	pVerts[23].nx = 0.654508f;
	pVerts[23].ny = 0.475528f;
	pVerts[23].nz = 0.587785f;

	pVerts[24].x = 0.169959f;
	pVerts[24].y = 0.523081f;
	pVerts[24].z = 0.0f;;
	pVerts[24].s = 0.2f;
	pVerts[24].t = 0.0f;;
	pVerts[24].nx = 0.309017f;
	pVerts[24].ny = 0.951057f;
	pVerts[24].nz = 0.0f;;

	pVerts[25].x = 0.161107f;
	pVerts[25].y = 0.495836f;
	pVerts[25].z = 0.088168f;
	pVerts[25].s = 0.2f;
	pVerts[25].t = 0.1f;
	pVerts[25].nx = 0.25f;
	pVerts[25].ny = 0.769421f;
	pVerts[25].nz = 0.587785f;

	pVerts[26].x = 0.137931f;
	pVerts[26].y = 0.424507f;
	pVerts[26].z = 0.142658f;
	pVerts[26].s = 0.2f;
	pVerts[26].t = 0.2f;
	pVerts[26].nx = 0.095491f;
	pVerts[26].ny = 0.293893f;
	pVerts[26].nz = 0.951057f;

	pVerts[27].x = 0.109283f;
	pVerts[27].y = 0.336339f;
	pVerts[27].z = 0.142658f;
	pVerts[27].s = 0.2f;
	pVerts[27].t = 0.3f;
	pVerts[27].nx = -0.095492f;
	pVerts[27].ny = -0.293893f;
	pVerts[27].nz = 0.951057f;

	pVerts[28].x = 0.086107f;
	pVerts[28].y = 0.265009f;
	pVerts[28].z = 0.088168f;
	pVerts[28].s = 0.2f;
	pVerts[28].t = 0.4f;
	pVerts[28].nx = -0.25f;
	pVerts[28].ny = -0.769421f;
	pVerts[28].nz = 0.587785f;

	pVerts[29].x = 0.077254f;
	pVerts[29].y = 0.237764f;
	pVerts[29].z = 0.0f;;
	pVerts[29].s = 0.2f;
	pVerts[29].t = 0.5f;
	pVerts[29].nx = -0.309017f;
	pVerts[29].ny = -0.951057f;
	pVerts[29].nz = 0.0f;;

	pVerts[30].x = 0.086107f;
	pVerts[30].y = 0.265009f;
	pVerts[30].z = -0.088168f;
	pVerts[30].s = 0.2f;
	pVerts[30].t = 0.6f;
	pVerts[30].nx = -0.25f;
	pVerts[30].ny = -0.769421f;
	pVerts[30].nz = -0.587785f;

	pVerts[31].x = 0.109283f;
	pVerts[31].y = 0.336339f;
	pVerts[31].z = -0.142658f;
	pVerts[31].s = 0.2f;
	pVerts[31].t = 0.7f;
	pVerts[31].nx = -0.095491f;
	pVerts[31].ny = -0.293893f;
	pVerts[31].nz = -0.951057f;

	pVerts[32].x = 0.137931f;
	pVerts[32].y = 0.424507f;
	pVerts[32].z = -0.142658f;
	pVerts[32].s = 0.2f;
	pVerts[32].t = 0.8f;
	pVerts[32].nx = 0.095492f;
	pVerts[32].ny = 0.293893f;
	pVerts[32].nz = -0.951057f;

	pVerts[33].x = 0.161107f;
	pVerts[33].y = 0.495836f;
	pVerts[33].z = -0.088168f;
	pVerts[33].s = 0.2f;
	pVerts[33].t = 0.9f;
	pVerts[33].nx = 0.25f;
	pVerts[33].ny = 0.769421f;
	pVerts[33].nz = -0.587785f;

	pVerts[34].x = 0.169959f;
	pVerts[34].y = 0.523081f;
	pVerts[34].z = 0.0f;;
	pVerts[34].s = 0.2f;
	pVerts[34].t = 1.0f;;
	pVerts[34].nx = 0.309017f;
	pVerts[34].ny = 0.951057f;
	pVerts[34].nz = 0.0f;;

	pVerts[35].x = 0.161107f;
	pVerts[35].y = 0.495836f;
	pVerts[35].z = 0.088168f;
	pVerts[35].s = 0.2f;
	pVerts[35].t = 1.1f;
	pVerts[35].nx = 0.25f;
	pVerts[35].ny = 0.769421f;
	pVerts[35].nz = 0.587785f;

	pVerts[36].x = -0.169959f;
	pVerts[36].y = 0.523081f;
	pVerts[36].z = 0.0f;;
	pVerts[36].s = 0.3f;
	pVerts[36].t = 0.0f;;
	pVerts[36].nx = -0.309017f;
	pVerts[36].ny = 0.951056f;
	pVerts[36].nz = 0.0f;;

	pVerts[37].x = -0.161107f;
	pVerts[37].y = 0.495836f;
	pVerts[37].z = 0.088168f;
	pVerts[37].s = 0.3f;
	pVerts[37].t = 0.1f;
	pVerts[37].nx = -0.25f;
	pVerts[37].ny = 0.769421f;
	pVerts[37].nz = 0.587785f;

	pVerts[38].x = -0.137931f;
	pVerts[38].y = 0.424506f;
	pVerts[38].z = 0.142658f;
	pVerts[38].s = 0.3f;
	pVerts[38].t = 0.2f;
	pVerts[38].nx = -0.095492f;
	pVerts[38].ny = 0.293893f;
	pVerts[38].nz = 0.951057f;

	pVerts[39].x = -0.109283f;
	pVerts[39].y = 0.336339f;
	pVerts[39].z = 0.142658f;
	pVerts[39].s = 0.3f;
	pVerts[39].t = 0.3f;
	pVerts[39].nx = 0.095492f;
	pVerts[39].ny = -0.293893f;
	pVerts[39].nz = 0.951057f;

	pVerts[40].x = -0.086107f;
	pVerts[40].y = 0.265009f;
	pVerts[40].z = 0.088168f;
	pVerts[40].s = 0.3f;
	pVerts[40].t = 0.4f;
	pVerts[40].nx = 0.25f;
	pVerts[40].ny = -0.769421f;
	pVerts[40].nz = 0.587785f;

	pVerts[41].x = -0.077254f;
	pVerts[41].y = 0.237764f;
	pVerts[41].z = 0.0f;;
	pVerts[41].s = 0.3f;
	pVerts[41].t = 0.5f;
	pVerts[41].nx = 0.309017f;
	pVerts[41].ny = -0.951056f;
	pVerts[41].nz = 0.0f;;

	pVerts[42].x = -0.086107f;
	pVerts[42].y = 0.265009f;
	pVerts[42].z = -0.088168f;
	pVerts[42].s = 0.3f;
	pVerts[42].t = 0.6f;
	pVerts[42].nx = 0.25f;
	pVerts[42].ny = -0.769421f;
	pVerts[42].nz = -0.587785f;

	pVerts[43].x = -0.109283f;
	pVerts[43].y = 0.336339f;
	pVerts[43].z = -0.142658f;
	pVerts[43].s = 0.3f;
	pVerts[43].t = 0.7f;
	pVerts[43].nx = 0.095491f;
	pVerts[43].ny = -0.293893f;
	pVerts[43].nz = -0.951057f;

	pVerts[44].x = -0.137931f;
	pVerts[44].y = 0.424507f;
	pVerts[44].z = -0.142658f;
	pVerts[44].s = 0.3f;
	pVerts[44].t = 0.8f;
	pVerts[44].nx = -0.095492f;
	pVerts[44].ny = 0.293893f;
	pVerts[44].nz = -0.951057f;

	pVerts[45].x = -0.161107f;
	pVerts[45].y = 0.495836f;
	pVerts[45].z = -0.088168f;
	pVerts[45].s = 0.3f;
	pVerts[45].t = 0.9f;
	pVerts[45].nx = -0.25f;
	pVerts[45].ny = 0.769421f;
	pVerts[45].nz = -0.587785f;

	pVerts[46].x = -0.169959f;
	pVerts[46].y = 0.523081f;
	pVerts[46].z = 0.0f;;
	pVerts[46].s = 0.3f;
	pVerts[46].t = 1.0f;;
	pVerts[46].nx = -0.309017f;
	pVerts[46].ny = 0.951056f;
	pVerts[46].nz = 0.0f;;

	pVerts[47].x = -0.161107f;
	pVerts[47].y = 0.495836f;
	pVerts[47].z = 0.088168f;
	pVerts[47].s = 0.3f;
	pVerts[47].t = 1.1f;
	pVerts[47].nx = -0.25f;
	pVerts[47].ny = 0.769421f;
	pVerts[47].nz = 0.587785f;

	pVerts[48].x = -0.444959f;
	pVerts[48].y = 0.323282f;
	pVerts[48].z = 0.0f;;
	pVerts[48].s = 0.4f;
	pVerts[48].t = 0.0f;;
	pVerts[48].nx = -0.809017f;
	pVerts[48].ny = 0.587785f;
	pVerts[48].nz = 0.0f;;

	pVerts[49].x = -0.421783f;
	pVerts[49].y = 0.306443f;
	pVerts[49].z = 0.088168f;
	pVerts[49].s = 0.4f;
	pVerts[49].t = 0.1f;
	pVerts[49].nx = -0.654509f;
	pVerts[49].ny = 0.475528f;
	pVerts[49].nz = 0.587785f;

	pVerts[50].x = -0.361107f;
	pVerts[50].y = 0.262359f;
	pVerts[50].z = 0.142658f;
	pVerts[50].s = 0.4f;
	pVerts[50].t = 0.2f;
	pVerts[50].nx = -0.25f;
	pVerts[50].ny = 0.181636f;
	pVerts[50].nz = 0.951057f;

	pVerts[51].x = -0.286107f;
	pVerts[51].y = 0.207869f;
	pVerts[51].z = 0.142658f;
	pVerts[51].s = 0.4f;
	pVerts[51].t = 0.3f;
	pVerts[51].nx = 0.25f;
	pVerts[51].ny = -0.181636f;
	pVerts[51].nz = 0.951057f;

	pVerts[52].x = -0.225431f;
	pVerts[52].y = 0.163785f;
	pVerts[52].z = 0.088168f;
	pVerts[52].s = 0.4f;
	pVerts[52].t = 0.4f;
	pVerts[52].nx = 0.654509f;
	pVerts[52].ny = -0.475528f;
	pVerts[52].nz = 0.587785f;

	pVerts[53].x = -0.202254f;
	pVerts[53].y = 0.146946f;
	pVerts[53].z = 0.0f;;
	pVerts[53].s = 0.4f;
	pVerts[53].t = 0.5f;
	pVerts[53].nx = 0.809017f;
	pVerts[53].ny = -0.587785f;
	pVerts[53].nz = 0.0f;;

	pVerts[54].x = -0.225431f;
	pVerts[54].y = 0.163785f;
	pVerts[54].z = -0.088168f;
	pVerts[54].s = 0.4f;
	pVerts[54].t = 0.6f;
	pVerts[54].nx = 0.654509f;
	pVerts[54].ny = -0.475528f;
	pVerts[54].nz = -0.587785f;

	pVerts[55].x = -0.286107f;
	pVerts[55].y = 0.207869f;
	pVerts[55].z = -0.142658f;
	pVerts[55].s = 0.4f;
	pVerts[55].t = 0.7f;
	pVerts[55].nx = 0.25f;
	pVerts[55].ny = -0.181636f;
	pVerts[55].nz = -0.951057f;

	pVerts[56].x = -0.361107f;
	pVerts[56].y = 0.262359f;
	pVerts[56].z = -0.142658f;
	pVerts[56].s = 0.4f;
	pVerts[56].t = 0.8f;
	pVerts[56].nx = -0.25f;
	pVerts[56].ny = 0.181636f;
	pVerts[56].nz = -0.951057f;

	pVerts[57].x = -0.421783f;
	pVerts[57].y = 0.306443f;
	pVerts[57].z = -0.088168f;
	pVerts[57].s = 0.4f;
	pVerts[57].t = 0.9f;
	pVerts[57].nx = -0.654509f;
	pVerts[57].ny = 0.475528f;
	pVerts[57].nz = -0.587785f;

	pVerts[58].x = -0.444959f;
	pVerts[58].y = 0.323282f;
	pVerts[58].z = 0.0f;;
	pVerts[58].s = 0.4f;
	pVerts[58].t = 1.0f;;
	pVerts[58].nx = -0.809017f;
	pVerts[58].ny = 0.587785f;
	pVerts[58].nz = 0.0f;;

	pVerts[59].x = -0.421783f;
	pVerts[59].y = 0.306443f;
	pVerts[59].z = 0.088168f;
	pVerts[59].s = 0.4f;
	pVerts[59].t = 1.1f;
	pVerts[59].nx = -0.654508f;
	pVerts[59].ny = 0.475528f;
	pVerts[59].nz = 0.587785f;

	pVerts[60].x = -0.55f;
	pVerts[60].y = 0.0f;;
	pVerts[60].z = 0.0f;;
	pVerts[60].s = 0.5f;
	pVerts[60].t = 0.0f;;
	pVerts[60].nx = -1.0f;;
	pVerts[60].ny = 0.0f;;
	pVerts[60].nz = 0.0f;;

	pVerts[61].x = -0.521353f;
	pVerts[61].y = 0.0f;;
	pVerts[61].z = 0.088168f;
	pVerts[61].s = 0.5f;
	pVerts[61].t = 0.1f;
	pVerts[61].nx = -0.809017f;
	pVerts[61].ny = 0.0f;;
	pVerts[61].nz = 0.587785f;

	pVerts[62].x = -0.446353f;
	pVerts[62].y = 0.0f;;
	pVerts[62].z = 0.142658f;
	pVerts[62].s = 0.5f;
	pVerts[62].t = 0.2f;
	pVerts[62].nx = -0.309017f;
	pVerts[62].ny = 0.0f;;
	pVerts[62].nz = 0.951057f;

	pVerts[63].x = -0.353647f;
	pVerts[63].y = 0.0f;;
	pVerts[63].z = 0.142658f;
	pVerts[63].s = 0.5f;
	pVerts[63].t = 0.3f;
	pVerts[63].nx = 0.309017f;
	pVerts[63].ny = 0.0f;;
	pVerts[63].nz = 0.951057f;

	pVerts[64].x = -0.278647f;
	pVerts[64].y = 0.0f;;
	pVerts[64].z = 0.088168f;
	pVerts[64].s = 0.5f;
	pVerts[64].t = 0.4f;
	pVerts[64].nx = 0.809017f;
	pVerts[64].ny = 0.0f;;
	pVerts[64].nz = 0.587785f;

	pVerts[65].x = -0.25f;
	pVerts[65].y = 0.0f;;
	pVerts[65].z = 0.0f;;
	pVerts[65].s = 0.5f;
	pVerts[65].t = 0.5f;
	pVerts[65].nx = 1.0f;;
	pVerts[65].ny = 0.0f;;
	pVerts[65].nz = 0.0f;;

	pVerts[66].x = -0.278647f;
	pVerts[66].y = 0.0f;;
	pVerts[66].z = -0.088168f;
	pVerts[66].s = 0.5f;
	pVerts[66].t = 0.6f;
	pVerts[66].nx = 0.809017f;
	pVerts[66].ny = 0.0f;;
	pVerts[66].nz = -0.587785f;

	pVerts[67].x = -0.353647f;
	pVerts[67].y = 0.0f;;
	pVerts[67].z = -0.142658f;
	pVerts[67].s = 0.5f;
	pVerts[67].t = 0.7f;
	pVerts[67].nx = 0.309017f;
	pVerts[67].ny = 0.0f;;
	pVerts[67].nz = -0.951057f;

	pVerts[68].x = -0.446353f;
	pVerts[68].y = 0.0f;;
	pVerts[68].z = -0.142658f;
	pVerts[68].s = 0.5f;
	pVerts[68].t = 0.8f;
	pVerts[68].nx = -0.309017f;
	pVerts[68].ny = 0.0f;;
	pVerts[68].nz = -0.951057f;

	pVerts[69].x = -0.521353f;
	pVerts[69].y = 0.0f;;
	pVerts[69].z = -0.088168f;
	pVerts[69].s = 0.5f;
	pVerts[69].t = 0.9f;
	pVerts[69].nx = -0.809017f;
	pVerts[69].ny = 0.0f;;
	pVerts[69].nz = -0.587785f;

	pVerts[70].x = -0.55f;
	pVerts[70].y = 0.0f;;
	pVerts[70].z = 0.0f;;
	pVerts[70].s = 0.5f;
	pVerts[70].t = 1.0f;;
	pVerts[70].nx = -1.0f;;
	pVerts[70].ny = 0.0f;;
	pVerts[70].nz = 0.0f;;

	pVerts[71].x = -0.521353f;
	pVerts[71].y = 0.0f;;
	pVerts[71].z = 0.088168f;
	pVerts[71].s = 0.5f;
	pVerts[71].t = 1.1f;
	pVerts[71].nx = -0.809017f;
	pVerts[71].ny = 0.0f;;
	pVerts[71].nz = 0.587785f;

	pVerts[72].x = -0.444959f;
	pVerts[72].y = -0.323282f;
	pVerts[72].z = 0.0f;;
	pVerts[72].s = 0.6f;
	pVerts[72].t = 0.0f;;
	pVerts[72].nx = -0.809017f;
	pVerts[72].ny = -0.587785f;
	pVerts[72].nz = 0.0f;;

	pVerts[73].x = -0.421783f;
	pVerts[73].y = -0.306443f;
	pVerts[73].z = 0.088168f;
	pVerts[73].s = 0.6f;
	pVerts[73].t = 0.1f;
	pVerts[73].nx = -0.654508f;
	pVerts[73].ny = -0.475528f;
	pVerts[73].nz = 0.587785f;

	pVerts[74].x = -0.361107f;
	pVerts[74].y = -0.262359f;
	pVerts[74].z = 0.142658f;
	pVerts[74].s = 0.6f;
	pVerts[74].t = 0.2f;
	pVerts[74].nx = -0.25f;
	pVerts[74].ny = -0.181636f;
	pVerts[74].nz = 0.951057f;

	pVerts[75].x = -0.286107f;
	pVerts[75].y = -0.207869f;
	pVerts[75].z = 0.142658f;
	pVerts[75].s = 0.6f;
	pVerts[75].t = 0.3f;
	pVerts[75].nx = 0.25f;
	pVerts[75].ny = 0.181636f;
	pVerts[75].nz = 0.951057f;

	pVerts[76].x = -0.225431f;
	pVerts[76].y = -0.163785f;
	pVerts[76].z = 0.088168f;
	pVerts[76].s = 0.6f;
	pVerts[76].t = 0.4f;
	pVerts[76].nx = 0.654509f;
	pVerts[76].ny = 0.475528f;
	pVerts[76].nz = 0.587785f;

	pVerts[77].x = -0.202254f;
	pVerts[77].y = -0.146946f;
	pVerts[77].z = 0.0f;;
	pVerts[77].s = 0.6f;
	pVerts[77].t = 0.5f;
	pVerts[77].nx = 0.809017f;
	pVerts[77].ny = 0.587785f;
	pVerts[77].nz = 0.0f;;

	pVerts[78].x = -0.225431f;
	pVerts[78].y = -0.163785f;
	pVerts[78].z = -0.088168f;
	pVerts[78].s = 0.6f;
	pVerts[78].t = 0.6f;
	pVerts[78].nx = 0.654508f;
	pVerts[78].ny = 0.475528f;
	pVerts[78].nz = -0.587785f;

	pVerts[79].x = -0.286107f;
	pVerts[79].y = -0.207869f;
	pVerts[79].z = -0.142658f;
	pVerts[79].s = 0.6f;
	pVerts[79].t = 0.7f;
	pVerts[79].nx = 0.25f;
	pVerts[79].ny = 0.181636f;
	pVerts[79].nz = -0.951057f;

	pVerts[80].x = -0.361107f;
	pVerts[80].y = -0.262359f;
	pVerts[80].z = -0.142658f;
	pVerts[80].s = 0.6f;
	pVerts[80].t = 0.8f;
	pVerts[80].nx = -0.25f;
	pVerts[80].ny = -0.181636f;
	pVerts[80].nz = -0.951057f;

	pVerts[81].x = -0.421783f;
	pVerts[81].y = -0.306443f;
	pVerts[81].z = -0.088168f;
	pVerts[81].s = 0.6f;
	pVerts[81].t = 0.9f;
	pVerts[81].nx = -0.654509f;
	pVerts[81].ny = -0.475528f;
	pVerts[81].nz = -0.587785f;

	pVerts[82].x = -0.444959f;
	pVerts[82].y = -0.323282f;
	pVerts[82].z = 0.0f;;
	pVerts[82].s = 0.6f;
	pVerts[82].t = 1.0f;;
	pVerts[82].nx = -0.809017f;
	pVerts[82].ny = -0.587785f;
	pVerts[82].nz = 0.0f;;

	pVerts[83].x = -0.421783f;
	pVerts[83].y = -0.306443f;
	pVerts[83].z = 0.088168f;
	pVerts[83].s = 0.6f;
	pVerts[83].t = 1.1f;
	pVerts[83].nx = -0.654508f;
	pVerts[83].ny = -0.475528f;
	pVerts[83].nz = 0.587785f;

	pVerts[84].x = -0.169959f;
	pVerts[84].y = -0.523081f;
	pVerts[84].z = 0.0f;;
	pVerts[84].s = 0.7f;
	pVerts[84].t = 0.0f;;
	pVerts[84].nx = -0.309017f;
	pVerts[84].ny = -0.951057f;
	pVerts[84].nz = 0.0f;;

	pVerts[85].x = -0.161107f;
	pVerts[85].y = -0.495836f;
	pVerts[85].z = 0.088168f;
	pVerts[85].s = 0.7f;
	pVerts[85].t = 0.1f;
	pVerts[85].nx = -0.25f;
	pVerts[85].ny = -0.769421f;
	pVerts[85].nz = 0.587785f;

	pVerts[86].x = -0.13793f;
	pVerts[86].y = -0.424507f;
	pVerts[86].z = 0.142658f;
	pVerts[86].s = 0.7f;
	pVerts[86].t = 0.2f;
	pVerts[86].nx = -0.095491f;
	pVerts[86].ny = -0.293893f;
	pVerts[86].nz = 0.951057f;

	pVerts[87].x = -0.109283f;
	pVerts[87].y = -0.336339f;
	pVerts[87].z = 0.142658f;
	pVerts[87].s = 0.7f;
	pVerts[87].t = 0.3f;
	pVerts[87].nx = 0.095491f;
	pVerts[87].ny = 0.293893f;
	pVerts[87].nz = 0.951057f;

	pVerts[88].x = -0.086107f;
	pVerts[88].y = -0.265009f;
	pVerts[88].z = 0.088168f;
	pVerts[88].s = 0.7f;
	pVerts[88].t = 0.4f;
	pVerts[88].nx = 0.25f;
	pVerts[88].ny = 0.769421f;
	pVerts[88].nz = 0.587785f;

	pVerts[89].x = -0.077254f;
	pVerts[89].y = -0.237764f;
	pVerts[89].z = 0.0f;;
	pVerts[89].s = 0.7f;
	pVerts[89].t = 0.5f;
	pVerts[89].nx = 0.309017f;
	pVerts[89].ny = 0.951057f;
	pVerts[89].nz = 0.0f;;

	pVerts[90].x = -0.086107f;
	pVerts[90].y = -0.265009f;
	pVerts[90].z = -0.088168f;
	pVerts[90].s = 0.7f;
	pVerts[90].t = 0.6f;
	pVerts[90].nx = 0.25f;
	pVerts[90].ny = 0.769421f;
	pVerts[90].nz = -0.587785f;

	pVerts[91].x = -0.109283f;
	pVerts[91].y = -0.336339f;
	pVerts[91].z = -0.142658f;
	pVerts[91].s = 0.7f;
	pVerts[91].t = 0.7f;
	pVerts[91].nx = 0.095491f;
	pVerts[91].ny = 0.293893f;
	pVerts[91].nz = -0.951057f;

	pVerts[92].x = -0.13793f;
	pVerts[92].y = -0.424507f;
	pVerts[92].z = -0.142658f;
	pVerts[92].s = 0.7f;
	pVerts[92].t = 0.8f;
	pVerts[92].nx = -0.095492f;
	pVerts[92].ny = -0.293893f;
	pVerts[92].nz = -0.951057f;

	pVerts[93].x = -0.161107f;
	pVerts[93].y = -0.495836f;
	pVerts[93].z = -0.088168f;
	pVerts[93].s = 0.7f;
	pVerts[93].t = 0.9f;
	pVerts[93].nx = -0.25f;
	pVerts[93].ny = -0.769421f;
	pVerts[93].nz = -0.587785f;

	pVerts[94].x = -0.169959f;
	pVerts[94].y = -0.523081f;
	pVerts[94].z = 0.0f;;
	pVerts[94].s = 0.7f;
	pVerts[94].t = 1.0f;;
	pVerts[94].nx = -0.309017f;
	pVerts[94].ny = -0.951057f;
	pVerts[94].nz = 0.0f;;

	pVerts[95].x = -0.161107f;
	pVerts[95].y = -0.495836f;
	pVerts[95].z = 0.088168f;
	pVerts[95].s = 0.7f;
	pVerts[95].t = 1.1f;
	pVerts[95].nx = -0.25f;
	pVerts[95].ny = -0.769421f;
	pVerts[95].nz = 0.587785f;

	pVerts[96].x = 0.169959f;
	pVerts[96].y = -0.523081f;
	pVerts[96].z = 0.0f;;
	pVerts[96].s = 0.8f;
	pVerts[96].t = 0.0f;;
	pVerts[96].nx = 0.309017f;
	pVerts[96].ny = -0.951056f;
	pVerts[96].nz = 0.0f;;

	pVerts[97].x = 0.161107f;
	pVerts[97].y = -0.495836f;
	pVerts[97].z = 0.088168f;
	pVerts[97].s = 0.8f;
	pVerts[97].t = 0.1f;
	pVerts[97].nx = 0.25f;
	pVerts[97].ny = -0.769421f;
	pVerts[97].nz = 0.587785f;

	pVerts[98].x = 0.137931f;
	pVerts[98].y = -0.424506f;
	pVerts[98].z = 0.142658f;
	pVerts[98].s = 0.8f;
	pVerts[98].t = 0.2f;
	pVerts[98].nx = 0.095492f;
	pVerts[98].ny = -0.293893f;
	pVerts[98].nz = 0.951057f;

	pVerts[99].x = 0.109283f;
	pVerts[99].y = -0.336339f;
	pVerts[99].z = 0.142658f;
	pVerts[99].s = 0.8f;
	pVerts[99].t = 0.3f;
	pVerts[99].nx = -0.095492f;
	pVerts[99].ny = 0.293893f;
	pVerts[99].nz = 0.951057f;

	pVerts[100].x = 0.086107f;
	pVerts[100].y = -0.265009f;
	pVerts[100].z = 0.088168f;
	pVerts[100].s = 0.8f;
	pVerts[100].t = 0.4f;
	pVerts[100].nx = -0.25f;
	pVerts[100].ny = 0.769421f;
	pVerts[100].nz = 0.587785f;

	pVerts[101].x = 0.077254f;
	pVerts[101].y = -0.237764f;
	pVerts[101].z = 0.0f;;
	pVerts[101].s = 0.8f;
	pVerts[101].t = 0.5f;
	pVerts[101].nx = -0.309017f;
	pVerts[101].ny = 0.951056f;
	pVerts[101].nz = 0.0f;;

	pVerts[102].x = 0.086107f;
	pVerts[102].y = -0.265009f;
	pVerts[102].z = -0.088168f;
	pVerts[102].s = 0.8f;
	pVerts[102].t = 0.6f;
	pVerts[102].nx = -0.25f;
	pVerts[102].ny = 0.769421f;
	pVerts[102].nz = -0.587785f;

	pVerts[103].x = 0.109283f;
	pVerts[103].y = -0.336339f;
	pVerts[103].z = -0.142658f;
	pVerts[103].s = 0.8f;
	pVerts[103].t = 0.7f;
	pVerts[103].nx = -0.095492f;
	pVerts[103].ny = 0.293893f;
	pVerts[103].nz = -0.951057f;

	pVerts[104].x = 0.137931f;
	pVerts[104].y = -0.424507f;
	pVerts[104].z = -0.142658f;
	pVerts[104].s = 0.8f;
	pVerts[104].t = 0.8f;
	pVerts[104].nx = 0.095492f;
	pVerts[104].ny = -0.293893f;
	pVerts[104].nz = -0.951057f;

	pVerts[105].x = 0.161107f;
	pVerts[105].y = -0.495836f;
	pVerts[105].z = -0.088168f;
	pVerts[105].s = 0.8f;
	pVerts[105].t = 0.9f;
	pVerts[105].nx = 0.25f;
	pVerts[105].ny = -0.769421f;
	pVerts[105].nz = -0.587785f;

	pVerts[106].x = 0.169959f;
	pVerts[106].y = -0.523081f;
	pVerts[106].z = 0.0f;;
	pVerts[106].s = 0.8f;
	pVerts[106].t = 1.0f;;
	pVerts[106].nx = 0.309017f;
	pVerts[106].ny = -0.951056f;
	pVerts[106].nz = 0.0f;;

	pVerts[107].x = 0.161107f;
	pVerts[107].y = -0.495836f;
	pVerts[107].z = 0.088168f;
	pVerts[107].s = 0.8f;
	pVerts[107].t = 1.1f;
	pVerts[107].nx = 0.25f;
	pVerts[107].ny = -0.769421f;
	pVerts[107].nz = 0.587785f;

	pVerts[108].x = 0.444959f;
	pVerts[108].y = -0.323282f;
	pVerts[108].z = 0.0f;;
	pVerts[108].s = 0.9f;
	pVerts[108].t = 0.0f;;
	pVerts[108].nx = 0.809017f;
	pVerts[108].ny = -0.587785f;
	pVerts[108].nz = 0.0f;;

	pVerts[109].x = 0.421783f;
	pVerts[109].y = -0.306443f;
	pVerts[109].z = 0.088168f;
	pVerts[109].s = 0.9f;
	pVerts[109].t = 0.1f;
	pVerts[109].nx = 0.654509f;
	pVerts[109].ny = -0.475528f;
	pVerts[109].nz = 0.587785f;

	pVerts[110].x = 0.361107f;
	pVerts[110].y = -0.262359f;
	pVerts[110].z = 0.142658f;
	pVerts[110].s = 0.9f;
	pVerts[110].t = 0.2f;
	pVerts[110].nx = 0.25f;
	pVerts[110].ny = -0.181636f;
	pVerts[110].nz = 0.951057f;

	pVerts[111].x = 0.286107f;
	pVerts[111].y = -0.207869f;
	pVerts[111].z = 0.142658f;
	pVerts[111].s = 0.9f;
	pVerts[111].t = 0.3f;
	pVerts[111].nx = -0.25f;
	pVerts[111].ny = 0.181636f;
	pVerts[111].nz = 0.951057f;

	pVerts[112].x = 0.225431f;
	pVerts[112].y = -0.163785f;
	pVerts[112].z = 0.088168f;
	pVerts[112].s = 0.9f;
	pVerts[112].t = 0.4f;
	pVerts[112].nx = -0.654509f;
	pVerts[112].ny = 0.475528f;
	pVerts[112].nz = 0.587785f;

	pVerts[113].x = 0.202254f;
	pVerts[113].y = -0.146946f;
	pVerts[113].z = 0.0f;;
	pVerts[113].s = 0.9f;
	pVerts[113].t = 0.5f;
	pVerts[113].nx = -0.809017f;
	pVerts[113].ny = 0.587785f;
	pVerts[113].nz = 0.0f;;

	pVerts[114].x = 0.225431f;
	pVerts[114].y = -0.163785f;
	pVerts[114].z = -0.088168f;
	pVerts[114].s = 0.9f;
	pVerts[114].t = 0.6f;
	pVerts[114].nx = -0.654509f;
	pVerts[114].ny = 0.475528f;
	pVerts[114].nz = -0.587785f;

	pVerts[115].x = 0.286107f;
	pVerts[115].y = -0.207869f;
	pVerts[115].z = -0.142658f;
	pVerts[115].s = 0.9f;
	pVerts[115].t = 0.7f;
	pVerts[115].nx = -0.25f;
	pVerts[115].ny = 0.181636f;
	pVerts[115].nz = -0.951057f;

	pVerts[116].x = 0.361107f;
	pVerts[116].y = -0.262359f;
	pVerts[116].z = -0.142658f;
	pVerts[116].s = 0.9f;
	pVerts[116].t = 0.8f;
	pVerts[116].nx = 0.25f;
	pVerts[116].ny = -0.181636f;
	pVerts[116].nz = -0.951057f;

	pVerts[117].x = 0.421783f;
	pVerts[117].y = -0.306443f;
	pVerts[117].z = -0.088168f;
	pVerts[117].s = 0.9f;
	pVerts[117].t = 0.9f;
	pVerts[117].nx = 0.654509f;
	pVerts[117].ny = -0.475528f;
	pVerts[117].nz = -0.587785f;

	pVerts[118].x = 0.444959f;
	pVerts[118].y = -0.323282f;
	pVerts[118].z = 0.0f;;
	pVerts[118].s = 0.9f;
	pVerts[118].t = 1.0f;;
	pVerts[118].nx = 0.809017f;
	pVerts[118].ny = -0.587785f;
	pVerts[118].nz = 0.0f;;

	pVerts[119].x = 0.421783f;
	pVerts[119].y = -0.306443f;
	pVerts[119].z = 0.088168f;
	pVerts[119].s = 0.9f;
	pVerts[119].t = 1.1f;
	pVerts[119].nx = 0.654508f;
	pVerts[119].ny = -0.475528f;
	pVerts[119].nz = 0.587785f;

	pVerts[120].x = 0.55f;
	pVerts[120].y = 0.0f;;
	pVerts[120].z = 0.0f;;
	pVerts[120].s = 1.0f;;
	pVerts[120].t = 0.0f;;
	pVerts[120].nx = 1.0f;;
	pVerts[120].ny = 0.0f;;
	pVerts[120].nz = 0.0f;;

	pVerts[121].x = 0.521353f;
	pVerts[121].y = 0.0f;;
	pVerts[121].z = 0.088168f;
	pVerts[121].s = 1.0f;;
	pVerts[121].t = 0.1f;
	pVerts[121].nx = 0.809017f;
	pVerts[121].ny = 0.0f;;
	pVerts[121].nz = 0.587785f;

	pVerts[122].x = 0.446353f;
	pVerts[122].y = 0.0f;;
	pVerts[122].z = 0.142658f;
	pVerts[122].s = 1.0f;;
	pVerts[122].t = 0.2f;
	pVerts[122].nx = 0.309017f;
	pVerts[122].ny = 0.0f;;
	pVerts[122].nz = 0.951057f;

	pVerts[123].x = 0.353647f;
	pVerts[123].y = 0.0f;;
	pVerts[123].z = 0.142658f;
	pVerts[123].s = 1.0f;;
	pVerts[123].t = 0.3f;
	pVerts[123].nx = -0.309017f;
	pVerts[123].ny = 0.0f;;
	pVerts[123].nz = 0.951057f;

	pVerts[124].x = 0.278647f;
	pVerts[124].y = 0.0f;;
	pVerts[124].z = 0.088168f;
	pVerts[124].s = 1.0f;;
	pVerts[124].t = 0.4f;
	pVerts[124].nx = -0.809017f;
	pVerts[124].ny = 0.0f;;
	pVerts[124].nz = 0.587785f;

	pVerts[125].x = 0.25f;
	pVerts[125].y = 0.0f;;
	pVerts[125].z = 0.0f;;
	pVerts[125].s = 1.0f;;
	pVerts[125].t = 0.5f;
	pVerts[125].nx = -1.0f;;
	pVerts[125].ny = 0.0f;;
	pVerts[125].nz = 0.0f;;

	pVerts[126].x = 0.278647f;
	pVerts[126].y = 0.0f;;
	pVerts[126].z = -0.088168f;
	pVerts[126].s = 1.0f;;
	pVerts[126].t = 0.6f;
	pVerts[126].nx = -0.809017f;
	pVerts[126].ny = 0.0f;;
	pVerts[126].nz = -0.587785f;

	pVerts[127].x = 0.353647f;
	pVerts[127].y = 0.0f;;
	pVerts[127].z = -0.142658f;
	pVerts[127].s = 1.0f;;
	pVerts[127].t = 0.7f;
	pVerts[127].nx = -0.309017f;
	pVerts[127].ny = 0.0f;;
	pVerts[127].nz = -0.951057f;

	pVerts[128].x = 0.446353f;
	pVerts[128].y = 0.0f;;
	pVerts[128].z = -0.142658f;
	pVerts[128].s = 1.0f;;
	pVerts[128].t = 0.8f;
	pVerts[128].nx = 0.309017f;
	pVerts[128].ny = 0.0f;;
	pVerts[128].nz = -0.951057f;

	pVerts[129].x = 0.521353f;
	pVerts[129].y = 0.0f;;
	pVerts[129].z = -0.088168f;
	pVerts[129].s = 1.0f;;
	pVerts[129].t = 0.9f;
	pVerts[129].nx = 0.809017f;
	pVerts[129].ny = 0.0f;;
	pVerts[129].nz = -0.587785f;

	pVerts[130].x = 0.55f;
	pVerts[130].y = 0.0f;;
	pVerts[130].z = 0.0f;;
	pVerts[130].s = 1.0f;;
	pVerts[130].t = 1.0f;;
	pVerts[130].nx = 1.0f;;
	pVerts[130].ny = 0.0f;;
	pVerts[130].nz = 0.0f;;

	pVerts[131].x = 0.521353f;
	pVerts[131].y = 0.0f;;
	pVerts[131].z = 0.088168f;
	pVerts[131].s = 1.0f;;
	pVerts[131].t = 1.1f;
	pVerts[131].nx = 0.809017f;
	pVerts[131].ny = 0.0f;;
	pVerts[131].nz = 0.587785f;

	//----------- WRITE ------------------------------------------
	// 1. Get the existing header

	// open file
	ferror = File::open( fh, "torus.cdm", FILE_READ );
	assert( ferror == FILE_SUCCESS );

	gObjFileHdr torusHdr;

	// write data of header to torusHdr
	ferror = File::read( fh, &torusHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

	// close
	ferror = File::close( fh );

		// 2. Write the data to the file
	
	// create file if not done already, start with header
	ferror = File::open( fh, "torus.cdm", FILE_READ_WRITE );
	assert (ferror == FILE_SUCCESS);

	// write the header
	ferror = File::write( fh, &torusHdr, sizeof(gObjFileHdr) );
	assert (ferror == FILE_SUCCESS);

		// update header with numVerts, VertBufferOffset
	torusHdr.numVerts = 132;
	ferror = File::tell( fh, torusHdr.vertBufferOffset );
	assert (ferror == FILE_SUCCESS);

		// write the vertex data
	ferror = File::write( fh, pVerts, 132 * sizeof(MyVertex_stride) );
	assert (ferror == FILE_SUCCESS);

		// update header with numTriList, triListBufferOffset
	torusHdr.numTriList = 220;
	ferror = File::tell( fh, torusHdr.triListBufferOffset );
	assert (ferror == FILE_SUCCESS);

		// write the triList data
	ferror = File::write( fh, tlist, 220 * sizeof(MyTriList) );
	assert (ferror == FILE_SUCCESS);

		// fix header with updated data
	ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
	assert (ferror == FILE_SUCCESS);

	ferror = File::write( fh, &torusHdr, sizeof(gObjFileHdr) );
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex_stride) * 132, pVerts, GL_STATIC_DRAW);
		
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList) * 220, tlist, GL_STATIC_DRAW);
}