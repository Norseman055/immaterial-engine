#include "GraphicsManager.h"
#include "PyramidObject.h"
#include "AnimControllerMan.h"

// temp hack
PCSNode *pRootNode = nullptr;

void setBonePose( GraphicsObject *node );
void walkAnimNode( GraphicsObject *node );

extern PyramidModel *myPyramid;

void SetTeddyHierarchy() {
	// ____________________ ANIMATION TESTING _______________________

	myPyramid = new PyramidModel;
	myPyramid->createVAO();

	// ________________________ TEDDY TEST _____________________________
	auto *tree = GraphicsObjMan::GetMainTree();
	pRootNode = tree->getRoot();

	auto *pBip01 = new PyramidObject( "Bip01", 0 );
	tree->insert( pBip01, pRootNode );

	auto *pBip01_Pelvis = new PyramidObject( "Bip01_Pelvis", 1 );
	tree->insert( pBip01_Pelvis, pBip01 );

	auto *pBip01_Spine = new PyramidObject( "Bip01_Spine", 2 );
	tree->insert( pBip01_Spine, pBip01_Pelvis );

	auto *pBip01_L_Thigh = new PyramidObject( "Bip01_L_Thigh", 3 );
	tree->insert( pBip01_L_Thigh, pBip01_Spine );

	auto *pBip01_L_Calf = new PyramidObject( "Bip01_L_Calf", 4 );
	tree->insert( pBip01_L_Calf, pBip01_L_Thigh );

	auto *pBip01_L_Foot = new PyramidObject( "Bip01_L_Foot", 5 );
	tree->insert( pBip01_L_Foot, pBip01_L_Calf );

	auto *pBip01_R_Thigh = new PyramidObject( "Bip01_R_Thigh", 6 );
	tree->insert( pBip01_R_Thigh, pBip01_Spine );

	auto *pBip01_R_Calf = new PyramidObject( "Bip01_R_Calf", 7 );
	tree->insert( pBip01_R_Calf, pBip01_R_Thigh );

	auto *pBip01_R_Foot = new PyramidObject( "Bip01_R_Foot", 8 );
	tree->insert( pBip01_R_Foot, pBip01_R_Calf );

	auto *pBip01_Spine1 = new PyramidObject( "Bip01_Spine1", 9 );
	tree->insert( pBip01_Spine1, pBip01_Spine );

	auto *pBip01_Neck = new PyramidObject( "Bip01_Neck", 10 );
	tree->insert( pBip01_Neck, pBip01_Spine1 );

	auto *pBip01_L_Clavicle = new PyramidObject( "Bip01_L_Clavicle", 11 );
	tree->insert( pBip01_L_Clavicle, pBip01_Neck );

	auto *pBip01_L_UpperArm = new PyramidObject( "Bip01_L_UpperArm", 12 );
	tree->insert( pBip01_L_UpperArm, pBip01_L_Clavicle );

	auto *pBip01_L_Forearm = new PyramidObject( "Bip01_L_Forearm", 13 );
	tree->insert( pBip01_L_Forearm, pBip01_L_UpperArm );

	auto *pBip01_L_Hand = new PyramidObject( "Bip01_L_Hand", 14 );
	tree->insert( pBip01_L_Hand, pBip01_L_Forearm );

	auto *pBip01_R_Clavicle = new PyramidObject( "Bip01_R_Clavicle", 15 );
	tree->insert( pBip01_R_Clavicle, pBip01_Neck );

	auto *pBip01_R_UpperArm = new PyramidObject( "Bip01_R_UpperArm", 16 );
	tree->insert( pBip01_R_UpperArm, pBip01_R_Clavicle );

	auto *pBip01_R_Forearm = new PyramidObject( "Bip01_R_Forearm", 17 );
	tree->insert( pBip01_R_Forearm, pBip01_R_UpperArm );

	auto *pBip01_R_Hand = new PyramidObject( "Bip01_R_Hand", 18 );
	tree->insert( pBip01_R_Hand, pBip01_R_Forearm );

	auto *pBip01_Head = new PyramidObject( "Bip01_Head", 19 );
	tree->insert( pBip01_Head, pBip01_Neck );

	pBip01->setLightColor( Vect( 0.5f, 1.5f, 0.5f ) );
	pBip01_L_Foot->setLightColor( Vect( 1.5f, 0.5f, 0.5f ) );
	pBip01_R_Foot->setLightColor( Vect( 0.5f, 0.5f, 1.5f ) );
}

void SetHumanoidHierarchy() {
	// ____________________ ANIMATION TESTING _______________________
	myPyramid = new PyramidModel;
	myPyramid->createVAO();

	// __________________________HUMANOID TEST__________________
	auto *tree = GraphicsObjMan::GetMainTree();
	pRootNode = tree->getRoot();

	auto *pHips = new PyramidObject( "Hips", 0 );
	tree->insert( pHips, pRootNode );

	auto *pSpine_Dummy = new PyramidObject( "Spine_Dummy", 1 );
	tree->insert( pSpine_Dummy, pHips );

	auto *pSpine = new PyramidObject( "Spine", 2 );
	tree->insert( pSpine, pSpine_Dummy );

	auto *pSpine1 = new PyramidObject( "Spine1", 3 );
	tree->insert( pSpine1, pSpine );

	auto *pSpine2 = new PyramidObject( "Spine2", 4 );
	tree->insert( pSpine2, pSpine1 );

	auto *pNeck = new PyramidObject( "Neck", 5 );
	tree->insert( pNeck, pSpine2 );

	auto *pHead = new PyramidObject( "Head", 6 );
	tree->insert( pHead, pNeck );

	auto *pHead_End = new PyramidObject( "Head_End", 7 );
	tree->insert( pHead_End, pHead );

	auto *pLeftShoulder_Dummy = new PyramidObject( "LeftShoulder_Dummy", 8 );
	tree->insert( pLeftShoulder_Dummy, pSpine2 );

	auto *pLeftShoulder = new PyramidObject( "LeftShoulder", 9 );
	tree->insert( pLeftShoulder, pLeftShoulder_Dummy );

	auto *pLeftArm_Dummy = new PyramidObject( "LeftArm_Dummy", 10 );
	tree->insert( pLeftArm_Dummy, pLeftShoulder );

	auto *pLeftArm = new PyramidObject( "LeftArm", 11 );
	tree->insert( pLeftArm, pLeftArm_Dummy );

	auto *pLeftArmRoll = new PyramidObject( "LeftArmRoll", 12 );
	tree->insert( pLeftArmRoll, pLeftArm );

	auto *pLeftForeArm = new PyramidObject( "LeftForeArm", 13 );
	tree->insert( pLeftForeArm, pLeftArmRoll );

	auto *pLeftForeArmRoll = new PyramidObject( "LeftForeArmRoll", 14 );
	tree->insert( pLeftForeArmRoll, pLeftForeArm );

	auto *pLeftHand = new PyramidObject( "LeftHand", 15 );
	tree->insert( pLeftHand, pLeftForeArmRoll );

	auto *pLeftThumb1 = new PyramidObject( "LeftThumb1", 16 );
	tree->insert( pLeftThumb1, pLeftHand );

	auto *pLeftThumb2 = new PyramidObject( "LeftThumb2", 17 );
	tree->insert( pLeftThumb2, pLeftThumb1 );

	auto *pLeftThumb3 = new PyramidObject( "LeftThumb3", 18 );
	tree->insert( pLeftThumb3, pLeftThumb2 );

	auto *pLeftThumb_End = new PyramidObject( "LeftThumb_End", 19 );
	tree->insert( pLeftThumb_End, pLeftThumb3 );

	auto *pLeftIndex1 = new PyramidObject( "LeftIndex1", 20 );
	tree->insert( pLeftIndex1, pLeftHand );

	auto *pLeftIndex2 = new PyramidObject( "LeftIndex2", 21 );
	tree->insert( pLeftIndex2, pLeftIndex1 );

	auto *pLeftIndex3 = new PyramidObject( "LeftIndex3", 22 );
	tree->insert( pLeftIndex3, pLeftIndex2 );

	auto *pLeftIndex_End = new PyramidObject( "LeftIndex_End", 23 );
	tree->insert( pLeftIndex_End, pLeftIndex3 );

	auto *pLeftMiddle1 = new PyramidObject( "LeftMiddle1", 24 );
	tree->insert( pLeftMiddle1, pLeftHand );

	auto *pLeftMiddle2 = new PyramidObject( "LeftMiddle2", 25 );
	tree->insert( pLeftMiddle2, pLeftMiddle1 );

	auto *pLeftMiddle3 = new PyramidObject( "LeftMiddle3", 26 );
	tree->insert( pLeftMiddle3, pLeftMiddle2 );

	auto *pLeftMiddle_End = new PyramidObject( "LeftMiddle_End", 27 );
	tree->insert( pLeftMiddle_End, pLeftMiddle3 );

	auto *pLeftRing1 = new PyramidObject( "LeftRing1", 28 );
	tree->insert( pLeftRing1, pLeftHand );

	auto *pLeftRing2 = new PyramidObject( "LeftRing2", 29 );
	tree->insert( pLeftRing2, pLeftRing1 );

	auto *pLeftRing3 = new PyramidObject( "LeftRing3", 30 );
	tree->insert( pLeftRing3, pLeftRing2 );

	auto *pLeftRing_End = new PyramidObject( "LeftRing_End", 31 );
	tree->insert( pLeftRing_End, pLeftRing3 );

	auto *pLeftPinky1 = new PyramidObject( "LeftPinky1", 32 );
	tree->insert( pLeftPinky1, pLeftHand );

	auto *pLeftPinky2 = new PyramidObject( "LeftPinky2", 33 );
	tree->insert( pLeftPinky2, pLeftPinky1 );

	auto *pLeftPinky3 = new PyramidObject( "LeftPinky3", 34 );
	tree->insert( pLeftPinky3, pLeftPinky2 );

	auto *pLeftPinky_End = new PyramidObject( "LeftPinky_End", 35 );
	tree->insert( pLeftPinky_End, pLeftPinky3 );

	auto *pRightShoulder_Dummy = new PyramidObject( "RightShoulder_Dummy", 36 );
	tree->insert( pRightShoulder_Dummy, pSpine2 );

	auto *pRightShoulder = new PyramidObject( "RightShoulder", 37 );
	tree->insert( pRightShoulder, pRightShoulder_Dummy );

	auto *pRightArm_Dummy = new PyramidObject( "RightArm_Dummy", 38 );
	tree->insert( pRightArm_Dummy, pRightShoulder );

	auto *pRightArm = new PyramidObject( "RightArm", 39 );
	tree->insert( pRightArm, pRightArm_Dummy );

	auto *pRightArmRoll = new PyramidObject( "RightArmRoll", 40 );
	tree->insert( pRightArmRoll, pRightArm );

	auto *pRightForeArm = new PyramidObject( "RightForeArm", 41 );
	tree->insert( pRightForeArm, pRightArmRoll );

	auto *pRightForeArmRoll = new PyramidObject( "RightForeArmRoll", 42 );
	tree->insert( pRightForeArmRoll, pRightForeArm );

	auto *pRightHand = new PyramidObject( "RightHand", 43 );
	tree->insert( pRightHand, pRightForeArmRoll );

	auto *pRightThumb1 = new PyramidObject( "RightThumb1", 44 );
	tree->insert( pRightThumb1, pRightHand );

	auto *pRightThumb2 = new PyramidObject( "RightThumb2", 45 );
	tree->insert( pRightThumb2, pRightThumb1 );

	auto *pRightThumb3 = new PyramidObject( "RightThumb3", 46 );
	tree->insert( pRightThumb3, pRightThumb2 );

	auto *pRightThumb_End = new PyramidObject( "RightThumb_End", 47 );
	tree->insert( pRightThumb_End, pRightThumb3 );

	auto *pRightIndex1 = new PyramidObject( "RightIndex1", 48 );
	tree->insert( pRightIndex1, pRightHand );

	auto *pRightIndex2 = new PyramidObject( "RightIndex2", 49 );
	tree->insert( pRightIndex2, pRightIndex1 );

	auto *pRightIndex3 = new PyramidObject( "RightIndex3", 50 );
	tree->insert( pRightIndex3, pRightIndex2 );

	auto *pRightIndex_End = new PyramidObject( "RightIndex_End", 51 );
	tree->insert( pRightIndex_End, pRightIndex3 );

	auto *pRightMiddle1 = new PyramidObject( "RightMiddle1", 52 );
	tree->insert( pRightMiddle1, pRightHand );

	auto *pRightMiddle2 = new PyramidObject( "RightMiddle2", 53 );
	tree->insert( pRightMiddle2, pRightMiddle1 );

	auto *pRightMiddle3 = new PyramidObject( "RightMiddle3", 54 );
	tree->insert( pRightMiddle3, pRightMiddle2 );

	auto *pRightMiddle_End = new PyramidObject( "RightMiddle_End", 55 );
	tree->insert( pRightMiddle_End, pRightMiddle3 );

	auto *pRightRing1 = new PyramidObject( "RightRing1", 56 );
	tree->insert( pRightRing1, pRightHand );

	auto *pRightRing2 = new PyramidObject( "RightRing2", 57 );
	tree->insert( pRightRing2, pRightRing1 );

	auto *pRightRing3 = new PyramidObject( "RightRing3", 58 );
	tree->insert( pRightRing3, pRightRing2 );

	auto *pRightRing_End = new PyramidObject( "RightRing_End", 59 );
	tree->insert( pRightRing_End, pRightRing3 );

	auto *pRightPinky1 = new PyramidObject( "RightPinky1", 60 );
	tree->insert( pRightPinky1, pRightHand );

	auto *pRightPinky2 = new PyramidObject( "RightPinky2", 61 );
	tree->insert( pRightPinky2, pRightPinky1 );

	auto *pRightPinky3 = new PyramidObject( "RightPinky3", 62 );
	tree->insert( pRightPinky3, pRightPinky2 );

	auto *pRightPinky_End = new PyramidObject( "RightPinky_End", 63 );
	tree->insert( pRightPinky_End, pRightPinky3 );

	auto *pLeftHips_Dummy = new PyramidObject( "LeftHips_Dummy", 64 );
	tree->insert( pLeftHips_Dummy, pHips );

	auto *pLeftUpLeg = new PyramidObject( "LeftUpLeg", 65 );
	tree->insert( pLeftUpLeg, pLeftHips_Dummy );

	auto *pLeftUpLegRoll = new PyramidObject( "LeftUpLegRoll", 66 );
	tree->insert( pLeftUpLegRoll, pLeftUpLeg );

	auto *pLeftLeg = new PyramidObject( "LeftLeg", 67 );
	tree->insert( pLeftLeg, pLeftUpLegRoll );

	auto *pLeftLegRoll = new PyramidObject( "LeftLegRoll", 68 );
	tree->insert( pLeftLegRoll, pLeftLeg );

	auto *pLeftFoot = new PyramidObject( "LeftFoot", 69 );
	tree->insert( pLeftFoot, pLeftLegRoll );

	auto *pLeftToes = new PyramidObject( "LeftToes", 70 );
	tree->insert( pLeftToes, pLeftFoot );

	auto *pLeftToes_End = new PyramidObject( "LeftToes_End", 71 );
	tree->insert( pLeftToes_End, pLeftToes );

	auto *pRightHips_Dummy = new PyramidObject( "RightHips_Dummy", 72 );
	tree->insert( pRightHips_Dummy, pHips );

	auto *pRightUpLeg = new PyramidObject( "RightUpLeg", 73 );
	tree->insert( pRightUpLeg, pRightHips_Dummy );

	auto *pRightUpLegRoll = new PyramidObject( "RightUpLegRoll", 74 );
	tree->insert( pRightUpLegRoll, pRightUpLeg );

	auto *pRightLeg = new PyramidObject( "RightLeg", 75 );
	tree->insert( pRightLeg, pRightUpLegRoll );

	auto *pRightLegRoll = new PyramidObject( "RightLegRoll", 76 );
	tree->insert( pRightLegRoll, pRightLeg );

	auto *pRightFoot = new PyramidObject( "RightFoot", 77 );
	tree->insert( pRightFoot, pRightLegRoll );

	auto *pRightToes = new PyramidObject( "RightToes", 78 );
	tree->insert( pRightToes, pRightFoot );

	auto *pRightToes_End = new PyramidObject( "RightToes_End", 79 );
	tree->insert( pRightToes_End, pRightToes );
}