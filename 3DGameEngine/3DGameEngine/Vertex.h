#pragma once
#include "../Utilities/Math.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;
	Vector2 uv2;

	Vertex() {};
	Vertex(const Vector3& posCoords, const Vector3& colorVal, const Vector3& normCoords,
		const Vector3& binormCoords, const Vector3& tgtCoords, const Vector2& uvCoords,
		const Vector2& uv2Coords)
		: pos(posCoords), color(colorVal), norm(normCoords), binorm(binormCoords),
		tgt(tgtCoords), uv(uvCoords), uv2(uv2Coords)
	{}
};