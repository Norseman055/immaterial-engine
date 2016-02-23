#pragma once

#include "Texture.h"
#include "NodeLink.h"

#define TEXT_ASSET_NAME_SIZE 32

class TextureNode : public NodeLink<Texture> {
public:
	TextureNode( const char * inAssetName, Texture* const texture )
		: NodeLink( texture ) {
		if ( strlen( inAssetName ) < TEXT_ASSET_NAME_SIZE ) {
			memcpy( this->assetName, inAssetName, strlen( inAssetName ) );
			this->assetName[strlen( inAssetName )] = '\0';
		} else {
			memcpy( this->assetName, inAssetName, TEXT_ASSET_NAME_SIZE - 1 );
			this->assetName[TEXT_ASSET_NAME_SIZE - 1] = '\0';
		}
	}

private:
	char assetName[TEXT_ASSET_NAME_SIZE];
};