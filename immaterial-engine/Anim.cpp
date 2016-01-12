
#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "AnimationManager.h"



//void ProcessAnimation( Time tCurr )
//{
//	AnimNode *node = AnimationMan::GetAnimation("walk");
//	Frame_Bucket *pTmp = node->getData();
//
//	// Get the result bone array
//	// Remember the first Frame is the result
//	Bone *bResult = pTmp->pBone;
//
//	// First one is the result, skip it
//	pTmp = pTmp->nextBucket;
// 
//	// Find which key frames
//	while( tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0 )
//	{
//		pTmp = pTmp->nextBucket;
//	}
//
//	// pTmp is the "B" key frame
//	// pTmp->prev is the "A" key frame
//	Frame_Bucket *pA = pTmp->prevBucket;
//	Frame_Bucket *pB = pTmp;
//
//	// find the "S" of the time
//	float tS = (tCurr - pA->KeyTime)  /(pB->KeyTime - pA->KeyTime);
//	
//	// interpolate to 
//	Bone *bA = pA->pBone;
//	Bone *bB = pB->pBone;
//
//   // Interpolate to tS time, for all bones
//	for( int i = 0; i < node->numBones; i++ )
//	{
//		// interpolate ahoy!
//		VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
//		QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
//		VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
//		
//		// advance the pointer
//		bA++;
//		bB++;
//		bResult++;
//   }
//};