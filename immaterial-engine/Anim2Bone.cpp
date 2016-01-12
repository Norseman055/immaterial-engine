
#include "OpenGL.h"
#include "DEBUGGING.h"
#include "Constants.h"

#include "GraphicsManager.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "MathEngine.h"
#include "PyramidModel.h"
#include "PyramidObject.h"
#include "Anim.h"
#include "AnimationManager.h"
#include "AnimControllerMan.h"

// temp hack
PCSNode *pRootNode = 0;

void setBonePose( GraphicsObject *node );
void walk_anim_node( GraphicsObject *node );

extern PyramidModel *myPyramid;

void SetTeddyHierarchy()
{
	// ____________________ ANIMATION TESTING _______________________

	myPyramid = new PyramidModel();
	myPyramid->createVAO();
	
	// ________________________ TEDDY TEST _____________________________
	PCSTree *tree = GraphicsObjMan::getMainTree();
	pRootNode = tree->getRoot();

	PyramidObject *pBip01 = new PyramidObject;
	pBip01->setIndex(0);
	pBip01->setName("Bip01");
	tree->insert( pBip01, pRootNode );

	PyramidObject *pBip01_Pelvis = new PyramidObject;
	pBip01_Pelvis->setIndex(1);
	pBip01_Pelvis->setName("Bip01_Pelvis");
	tree->insert( pBip01_Pelvis, pBip01 );

	PyramidObject *pBip01_Spine = new PyramidObject;
	pBip01_Spine->setIndex(2);
	pBip01_Spine->setName("Bip01_Spine");
	tree->insert( pBip01_Spine, pBip01_Pelvis );

	PyramidObject *pBip01_L_Thigh = new PyramidObject;
	pBip01_L_Thigh->setIndex(3);
	pBip01_L_Thigh->setName("Bip01_L_Thigh");
	tree->insert( pBip01_L_Thigh, pBip01_Spine );

	PyramidObject *pBip01_L_Calf = new PyramidObject;
	pBip01_L_Calf->setIndex(4);
	pBip01_L_Calf->setName("Bip01_L_Calf");
	tree->insert( pBip01_L_Calf, pBip01_L_Thigh );

	PyramidObject *pBip01_L_Foot = new PyramidObject;
	pBip01_L_Foot->setIndex(5);
	pBip01_L_Foot->setName("Bip01_L_Foot");
	tree->insert( pBip01_L_Foot, pBip01_L_Calf );

	PyramidObject *pBip01_R_Thigh = new PyramidObject;
	pBip01_R_Thigh->setIndex(6);
	pBip01_R_Thigh->setName("Bip01_R_Thigh");
	tree->insert( pBip01_R_Thigh, pBip01_Spine );

	PyramidObject *pBip01_R_Calf = new PyramidObject;
	pBip01_R_Calf->setIndex(7);
	pBip01_R_Calf->setName("Bip01_R_Calf");
	tree->insert( pBip01_R_Calf, pBip01_R_Thigh );

	PyramidObject *pBip01_R_Foot = new PyramidObject;
	pBip01_R_Foot->setIndex(8);
	pBip01_R_Foot->setName("Bip01_R_Foot");
	tree->insert( pBip01_R_Foot, pBip01_R_Calf );

	PyramidObject *pBip01_Spine1 = new PyramidObject;
	pBip01_Spine1->setIndex(9);
	pBip01_Spine1->setName("Bip01_Spine1");
	tree->insert( pBip01_Spine1, pBip01_Spine );

	PyramidObject *pBip01_Neck = new PyramidObject;
	pBip01_Neck->setIndex(10);
	pBip01_Neck->setName("Bip01_Neck");
	tree->insert( pBip01_Neck, pBip01_Spine1 );

	PyramidObject *pBip01_L_Clavicle = new PyramidObject;
	pBip01_L_Clavicle->setIndex(11);
	pBip01_L_Clavicle->setName("Bip01_L_Clavicle");
	tree->insert( pBip01_L_Clavicle, pBip01_Neck );

	PyramidObject *pBip01_L_UpperArm = new PyramidObject;
	pBip01_L_UpperArm->setIndex(12);
	pBip01_L_UpperArm->setName("Bip01_L_UpperArm");
	tree->insert( pBip01_L_UpperArm, pBip01_L_Clavicle );

	PyramidObject *pBip01_L_Forearm = new PyramidObject;
	pBip01_L_Forearm->setIndex(13);
	pBip01_L_Forearm->setName("Bip01_L_Forearm");
	tree->insert( pBip01_L_Forearm, pBip01_L_UpperArm );

	PyramidObject *pBip01_L_Hand = new PyramidObject;
	pBip01_L_Hand->setIndex(14);
	pBip01_L_Hand->setName("Bip01_L_Hand");
	tree->insert( pBip01_L_Hand, pBip01_L_Forearm );

	PyramidObject *pBip01_R_Clavicle = new PyramidObject;
	pBip01_R_Clavicle->setIndex(15);
	pBip01_R_Clavicle->setName("Bip01_R_Clavicle");
	tree->insert( pBip01_R_Clavicle, pBip01_Neck );

	PyramidObject *pBip01_R_UpperArm = new PyramidObject;
	pBip01_R_UpperArm->setIndex(16);
	pBip01_R_UpperArm->setName("Bip01_R_UpperArm");
	tree->insert( pBip01_R_UpperArm, pBip01_R_Clavicle );

	PyramidObject *pBip01_R_Forearm = new PyramidObject;
	pBip01_R_Forearm->setIndex(17);
	pBip01_R_Forearm->setName("Bip01_R_Forearm");
	tree->insert( pBip01_R_Forearm, pBip01_R_UpperArm );

	PyramidObject *pBip01_R_Hand = new PyramidObject;
	pBip01_R_Hand->setIndex(18);
	pBip01_R_Hand->setName("Bip01_R_Hand");
	tree->insert( pBip01_R_Hand, pBip01_R_Forearm );

	PyramidObject *pBip01_Head = new PyramidObject;
	pBip01_Head->setIndex(19);
	pBip01_Head->setName("Bip01_Head");
	tree->insert( pBip01_Head, pBip01_Neck );

	pBip01->setLightColor( Vect(0.5f, 1.5f, 0.5f) );
	pBip01_L_Foot->setLightColor( Vect(1.5f, 0.5f, 0.5f) );
	pBip01_R_Foot->setLightColor( Vect(0.5f, 0.5f, 1.5f) );
}

void SetHumanoidHierarchy()
{
	// ____________________ ANIMATION TESTING _______________________
	myPyramid = new PyramidModel();
	myPyramid->createVAO();

	// __________________________HUMANOID TEST__________________
	PCSTree *tree = GraphicsObjMan::getMainTree();
	pRootNode = tree->getRoot();

	PyramidObject *pHips = new PyramidObject;
	pHips->setIndex(0);
//	pHips->setScale(0.1f);
	pHips->setName("Hips");
	tree->insert( pHips, pRootNode );

	PyramidObject *pSpine_Dummy = new PyramidObject;
	pSpine_Dummy->setIndex(1);
	pSpine_Dummy->setName("Spine_Dummy");
	tree->insert( pSpine_Dummy, pHips );

	PyramidObject *pSpine = new PyramidObject;
	pSpine->setIndex(2);
	pSpine->setName("Spine");
	tree->insert( pSpine, pSpine_Dummy );

	PyramidObject *pSpine1 = new PyramidObject;
	pSpine1->setIndex(3);
	pSpine1->setName("Spine1");
	tree->insert( pSpine1, pSpine );

	PyramidObject *pSpine2 = new PyramidObject;
	pSpine2->setIndex(4);
	pSpine2->setName("Spine2");
	tree->insert( pSpine2, pSpine1 );

	PyramidObject *pNeck = new PyramidObject;
	pNeck->setIndex(5);
	pNeck->setName("Neck");
	tree->insert( pNeck, pSpine2 );

	PyramidObject *pHead = new PyramidObject;
	pHead->setIndex(6);
	pHead->setName("Head");
	tree->insert( pHead, pNeck );

	PyramidObject *pHead_End = new PyramidObject;
	pHead_End->setIndex(7);
	pHead_End->setName("Head_End");
	tree->insert( pHead_End, pHead );

	PyramidObject *pLeftShoulder_Dummy = new PyramidObject;
	pLeftShoulder_Dummy->setIndex(8);
	pLeftShoulder_Dummy->setName("LeftShoulder_Dummy");
	tree->insert( pLeftShoulder_Dummy, pSpine2 );

	PyramidObject *pLeftShoulder = new PyramidObject;
	pLeftShoulder->setIndex(9);
	pLeftShoulder->setName("LeftShoulder");
	tree->insert( pLeftShoulder, pLeftShoulder_Dummy );

	PyramidObject *pLeftArm_Dummy = new PyramidObject;
	pLeftArm_Dummy->setIndex(10);
	pLeftArm_Dummy->setName("LeftArm_Dummy");
	tree->insert( pLeftArm_Dummy, pLeftShoulder );

	PyramidObject *pLeftArm = new PyramidObject;
	pLeftArm->setIndex(11);
	pLeftArm->setName("LeftArm");
	tree->insert( pLeftArm, pLeftArm_Dummy );

	PyramidObject *pLeftArmRoll = new PyramidObject;
	pLeftArmRoll->setIndex(12);
	pLeftArmRoll->setName("LeftArmRoll");
	tree->insert( pLeftArmRoll, pLeftArm );

	PyramidObject *pLeftForeArm = new PyramidObject;
	pLeftForeArm->setIndex(13);
	pLeftForeArm->setName("LeftForeArm");
	tree->insert( pLeftForeArm, pLeftArmRoll );

	PyramidObject *pLeftForeArmRoll = new PyramidObject;
	pLeftForeArmRoll->setIndex(14);
	pLeftForeArmRoll->setName("LeftForeArmRoll");
	tree->insert( pLeftForeArmRoll, pLeftForeArm );

	PyramidObject *pLeftHand = new PyramidObject;
	pLeftHand->setIndex(15);
	pLeftHand->setName("LeftHand");
	tree->insert( pLeftHand, pLeftForeArmRoll );

	PyramidObject *pLeftThumb1 = new PyramidObject;
	pLeftThumb1->setIndex(16);
	pLeftThumb1->setName("LeftThumb1");
	tree->insert( pLeftThumb1, pLeftHand );

	PyramidObject *pLeftThumb2 = new PyramidObject;
	pLeftThumb2->setIndex(17);
	pLeftThumb2->setName("LeftThumb2");
	tree->insert( pLeftThumb2, pLeftThumb1 );

	PyramidObject *pLeftThumb3 = new PyramidObject;
	pLeftThumb3->setIndex(18);
	pLeftThumb3->setName("LeftThumb3");
	tree->insert( pLeftThumb3, pLeftThumb2 );

	PyramidObject *pLeftThumb_End = new PyramidObject;
	pLeftThumb_End->setIndex(19);
	pLeftThumb_End->setName("LeftThumb_End");
	tree->insert( pLeftThumb_End, pLeftThumb3 );

	PyramidObject *pLeftIndex1 = new PyramidObject;
	pLeftIndex1->setIndex(20);
	pLeftIndex1->setName("LeftIndex1");
	tree->insert( pLeftIndex1, pLeftHand );

	PyramidObject *pLeftIndex2 = new PyramidObject;
	pLeftIndex2->setIndex(21);
	pLeftIndex2->setName("LeftIndex2");
	tree->insert( pLeftIndex2, pLeftIndex1 );

	PyramidObject *pLeftIndex3 = new PyramidObject;
	pLeftIndex3->setIndex(22);
	pLeftIndex3->setName("LeftIndex3");
	tree->insert( pLeftIndex3, pLeftIndex2 );

	PyramidObject *pLeftIndex_End = new PyramidObject;
	pLeftIndex_End->setIndex(23);
	pLeftIndex_End->setName("LeftIndex_End");
	tree->insert( pLeftIndex_End, pLeftIndex3 );

	PyramidObject *pLeftMiddle1 = new PyramidObject;
	pLeftMiddle1->setIndex(24);
	pLeftMiddle1->setName("LeftMiddle1");
	tree->insert( pLeftMiddle1, pLeftHand );

	PyramidObject *pLeftMiddle2 = new PyramidObject;
	pLeftMiddle2->setIndex(25);
	pLeftMiddle2->setName("LeftMiddle2");
	tree->insert( pLeftMiddle2, pLeftMiddle1 );

	PyramidObject *pLeftMiddle3 = new PyramidObject;
	pLeftMiddle3->setIndex(26);
	pLeftMiddle3->setName("LeftMiddle3");
	tree->insert( pLeftMiddle3, pLeftMiddle2 );

	PyramidObject *pLeftMiddle_End = new PyramidObject;
	pLeftMiddle_End->setIndex(27);
	pLeftMiddle_End->setName("LeftMiddle_End");
	tree->insert( pLeftMiddle_End, pLeftMiddle3 );

	PyramidObject *pLeftRing1 = new PyramidObject;
	pLeftRing1->setIndex(28);
	pLeftRing1->setName("LeftRing1");
	tree->insert( pLeftRing1, pLeftHand );

	PyramidObject *pLeftRing2 = new PyramidObject;
	pLeftRing2->setIndex(29);
	pLeftRing2->setName("LeftRing2");
	tree->insert( pLeftRing2, pLeftRing1 );

	PyramidObject *pLeftRing3 = new PyramidObject;
	pLeftRing3->setIndex(30);
	pLeftRing3->setName("LeftRing3");
	tree->insert( pLeftRing3, pLeftRing2 );

	PyramidObject *pLeftRing_End = new PyramidObject;
	pLeftRing_End->setIndex(31);
	pLeftRing_End->setName("LeftRing_End");
	tree->insert( pLeftRing_End, pLeftRing3 );

	PyramidObject *pLeftPinky1 = new PyramidObject;
	pLeftPinky1->setIndex(32);
	pLeftPinky1->setName("LeftPinky1");
	tree->insert( pLeftPinky1, pLeftHand );

	PyramidObject *pLeftPinky2 = new PyramidObject;
	pLeftPinky2->setIndex(33);
	pLeftPinky2->setName("LeftPinky2");
	tree->insert( pLeftPinky2, pLeftPinky1 );

	PyramidObject *pLeftPinky3 = new PyramidObject;
	pLeftPinky3->setIndex(34);
	pLeftPinky3->setName("LeftPinky3");
	tree->insert( pLeftPinky3, pLeftPinky2 );

	PyramidObject *pLeftPinky_End = new PyramidObject;
	pLeftPinky_End->setIndex(35);
	pLeftPinky_End->setName("LeftPinky_End");
	tree->insert( pLeftPinky_End, pLeftPinky3 );

	PyramidObject *pRightShoulder_Dummy = new PyramidObject;
	pRightShoulder_Dummy->setIndex(36);
	pRightShoulder_Dummy->setName("RightShoulder_Dummy");
	tree->insert( pRightShoulder_Dummy, pSpine2 );

	PyramidObject *pRightShoulder = new PyramidObject;
	pRightShoulder->setIndex(37);
	pRightShoulder->setName("RightShoulder");
	tree->insert( pRightShoulder, pRightShoulder_Dummy );

	PyramidObject *pRightArm_Dummy = new PyramidObject;
	pRightArm_Dummy->setIndex(38);
	pRightArm_Dummy->setName("RightArm_Dummy");
	tree->insert( pRightArm_Dummy, pRightShoulder );

	PyramidObject *pRightArm = new PyramidObject;
	pRightArm->setIndex(39);
	pRightArm->setName("RightArm");
	tree->insert( pRightArm, pRightArm_Dummy );

	PyramidObject *pRightArmRoll = new PyramidObject;
	pRightArmRoll->setIndex(40);
	pRightArmRoll->setName("RightArmRoll");
	tree->insert( pRightArmRoll, pRightArm );

	PyramidObject *pRightForeArm = new PyramidObject;
	pRightForeArm->setIndex(41);
	pRightForeArm->setName("RightForeArm");
	tree->insert( pRightForeArm, pRightArmRoll );

	PyramidObject *pRightForeArmRoll = new PyramidObject;
	pRightForeArmRoll->setIndex(42);
	pRightForeArmRoll->setName("RightForeArmRoll");
	tree->insert( pRightForeArmRoll, pRightForeArm );

	PyramidObject *pRightHand = new PyramidObject;
	pRightHand->setIndex(43);
	pRightHand->setName("RightHand");
	tree->insert( pRightHand, pRightForeArmRoll );

	PyramidObject *pRightThumb1 = new PyramidObject;
	pRightThumb1->setIndex(44);
	pRightThumb1->setName("RightThumb1");
	tree->insert( pRightThumb1, pRightHand );

	PyramidObject *pRightThumb2 = new PyramidObject;
	pRightThumb2->setIndex(45);
	pRightThumb2->setName("RightThumb2");
	tree->insert( pRightThumb2, pRightThumb1 );

	PyramidObject *pRightThumb3 = new PyramidObject;
	pRightThumb3->setIndex(46);
	pRightThumb3->setName("RightThumb3");
	tree->insert( pRightThumb3, pRightThumb2 );

	PyramidObject *pRightThumb_End = new PyramidObject;
	pRightThumb_End->setIndex(47);
	pRightThumb_End->setName("RightThumb_End");
	tree->insert( pRightThumb_End, pRightThumb3 );

	PyramidObject *pRightIndex1 = new PyramidObject;
	pRightIndex1->setIndex(48);
	pRightIndex1->setName("RightIndex1");
	tree->insert( pRightIndex1, pRightHand );

	PyramidObject *pRightIndex2 = new PyramidObject;
	pRightIndex2->setIndex(49);
	pRightIndex2->setName("RightIndex2");
	tree->insert( pRightIndex2, pRightIndex1 );

	PyramidObject *pRightIndex3 = new PyramidObject;
	pRightIndex3->setIndex(50);
	pRightIndex3->setName("RightIndex3");
	tree->insert( pRightIndex3, pRightIndex2 );

	PyramidObject *pRightIndex_End = new PyramidObject;
	pRightIndex_End->setIndex(51);
	pRightIndex_End->setName("RightIndex_End");
	tree->insert( pRightIndex_End, pRightIndex3 );

	PyramidObject *pRightMiddle1 = new PyramidObject;
	pRightMiddle1->setIndex(52);
	pRightMiddle1->setName("RightMiddle1");
	tree->insert( pRightMiddle1, pRightHand );

	PyramidObject *pRightMiddle2 = new PyramidObject;
	pRightMiddle2->setIndex(53);
	pRightMiddle2->setName("RightMiddle2");
	tree->insert( pRightMiddle2, pRightMiddle1 );

	PyramidObject *pRightMiddle3 = new PyramidObject;
	pRightMiddle3->setIndex(54);
	pRightMiddle3->setName("RightMiddle3");
	tree->insert( pRightMiddle3, pRightMiddle2 );

	PyramidObject *pRightMiddle_End = new PyramidObject;
	pRightMiddle_End->setIndex(55);
	pRightMiddle_End->setName("RightMiddle_End");
	tree->insert( pRightMiddle_End, pRightMiddle3 );

	PyramidObject *pRightRing1 = new PyramidObject;
	pRightRing1->setIndex(56);
	pRightRing1->setName("RightRing1");
	tree->insert( pRightRing1, pRightHand );

	PyramidObject *pRightRing2 = new PyramidObject;
	pRightRing2->setIndex(57);
	pRightRing2->setName("RightRing2");
	tree->insert( pRightRing2, pRightRing1 );

	PyramidObject *pRightRing3 = new PyramidObject;
	pRightRing3->setIndex(58);
	pRightRing3->setName("RightRing3");
	tree->insert( pRightRing3, pRightRing2 );

	PyramidObject *pRightRing_End = new PyramidObject;
	pRightRing_End->setIndex(59);
	pRightRing_End->setName("RightRing_End");
	tree->insert( pRightRing_End, pRightRing3 );

	PyramidObject *pRightPinky1 = new PyramidObject;
	pRightPinky1->setIndex(60);
	pRightPinky1->setName("RightPinky1");
	tree->insert( pRightPinky1, pRightHand );

	PyramidObject *pRightPinky2 = new PyramidObject;
	pRightPinky2->setIndex(61);
	pRightPinky2->setName("RightPinky2");
	tree->insert( pRightPinky2, pRightPinky1 );

	PyramidObject *pRightPinky3 = new PyramidObject;
	pRightPinky3->setIndex(62);
	pRightPinky3->setName("RightPinky3");
	tree->insert( pRightPinky3, pRightPinky2 );

	PyramidObject *pRightPinky_End = new PyramidObject;
	pRightPinky_End->setIndex(63);
	pRightPinky_End->setName("RightPinky_End");
	tree->insert( pRightPinky_End, pRightPinky3 );

	PyramidObject *pLeftHips_Dummy = new PyramidObject;
	pLeftHips_Dummy->setIndex(64);
	pLeftHips_Dummy->setName("LeftHips_Dummy");
	tree->insert( pLeftHips_Dummy, pHips );

	PyramidObject *pLeftUpLeg = new PyramidObject;
	pLeftUpLeg->setIndex(65);
	pLeftUpLeg->setName("LeftUpLeg");
	tree->insert( pLeftUpLeg, pLeftHips_Dummy );

	PyramidObject *pLeftUpLegRoll = new PyramidObject;
	pLeftUpLegRoll->setIndex(66);
	pLeftUpLegRoll->setName("LeftUpLegRoll");
	tree->insert( pLeftUpLegRoll, pLeftUpLeg );

	PyramidObject *pLeftLeg = new PyramidObject;
	pLeftLeg->setIndex(67);
	pLeftLeg->setName("LeftLeg");
	tree->insert( pLeftLeg, pLeftUpLegRoll );

	PyramidObject *pLeftLegRoll = new PyramidObject;
	pLeftLegRoll->setIndex(68);
	pLeftLegRoll->setName("LeftLegRoll");
	tree->insert( pLeftLegRoll, pLeftLeg );

	PyramidObject *pLeftFoot = new PyramidObject;
	pLeftFoot->setIndex(69);
	pLeftFoot->setName("LeftFoot");
	tree->insert( pLeftFoot, pLeftLegRoll );

	PyramidObject *pLeftToes = new PyramidObject;
	pLeftToes->setIndex(70);
	pLeftToes->setName("LeftToes");
	tree->insert( pLeftToes, pLeftFoot );

	PyramidObject *pLeftToes_End = new PyramidObject;
	pLeftToes_End->setIndex(71);
	pLeftToes_End->setName("LeftToes_End");
	tree->insert( pLeftToes_End, pLeftToes );

	PyramidObject *pRightHips_Dummy = new PyramidObject;
	pRightHips_Dummy->setIndex(72);
	pRightHips_Dummy->setName("RightHips_Dummy");
	tree->insert( pRightHips_Dummy, pHips );

	PyramidObject *pRightUpLeg = new PyramidObject;
	pRightUpLeg->setIndex(73);
	pRightUpLeg->setName("RightUpLeg");
	tree->insert( pRightUpLeg, pRightHips_Dummy );

	PyramidObject *pRightUpLegRoll = new PyramidObject;
	pRightUpLegRoll->setIndex(74);
	pRightUpLegRoll->setName("RightUpLegRoll");
	tree->insert( pRightUpLegRoll, pRightUpLeg );

	PyramidObject *pRightLeg = new PyramidObject;
	pRightLeg->setIndex(75);
	pRightLeg->setName("RightLeg");
	tree->insert( pRightLeg, pRightUpLegRoll );

	PyramidObject *pRightLegRoll = new PyramidObject;
	pRightLegRoll->setIndex(76);
	pRightLegRoll->setName("RightLegRoll");
	tree->insert( pRightLegRoll, pRightLeg );

	PyramidObject *pRightFoot = new PyramidObject;
	pRightFoot->setIndex(77);
	pRightFoot->setName("RightFoot");
	tree->insert( pRightFoot, pRightLegRoll );

	PyramidObject *pRightToes = new PyramidObject;
	pRightToes->setIndex(78);
	pRightToes->setName("RightToes");
	tree->insert( pRightToes, pRightFoot );

	PyramidObject *pRightToes_End = new PyramidObject;
	pRightToes_End->setIndex(79);
	pRightToes_End->setName("RightToes_End");
	tree->insert( pRightToes_End, pRightToes );
}