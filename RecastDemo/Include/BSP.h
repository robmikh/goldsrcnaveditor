//
// CS Bot - Neoptolemus' CS 1.6 bot, based on Botman's HPB bot template
//
// BSP.h
//

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifndef BSP_H
#define BSP_H


#define LUMP_ENTITIES      0
#define LUMP_PLANES        1
#define LUMP_TEXTURES      2
#define LUMP_VERTICES      3
#define LUMP_VISIBILITY    4
#define LUMP_NODES         5
#define LUMP_TEXINFO       6
#define LUMP_FACES         7
#define LUMP_LIGHTING      8
#define LUMP_CLIPNODES     9
#define LUMP_LEAVES       10
#define LUMP_MARKSURFACES 11
#define LUMP_EDGES        12
#define LUMP_SURFEDGES    13
#define LUMP_MODELS       14
#define HEADER_LUMPS      15

#define MAX_MAP_HULLS        4

#define MAX_MAP_MODELS       2048
#define MAX_MAP_BRUSHES      4096
#define MAX_MAP_ENTITIES     2048
#define MAX_MAP_ENTSTRING    (128*1024)

#define MAX_MAP_PLANES       32767
#define MAX_MAP_NODES        32767
#define MAX_MAP_CLIPNODES    32767
#define MAX_MAP_LEAFS        8192
#define MAX_MAP_VERTS        131070//65535
#define MAX_MAP_FACES        65535
#define MAX_MAP_MARKSURFACES 65535
#define MAX_MAP_TEXINFO      8192
#define MAX_MAP_EDGES        256000
#define MAX_MAP_SURFEDGES    512000
#define MAX_MAP_TEXTURES     512
#define MAX_MAP_MIPTEX       0x200000
#define MAX_MAP_LIGHTING     0x200000
#define MAX_MAP_VISIBILITY   0x200000
#define MAX_MAP_TRIS         131070//65535

#define MAX_MAP_PORTALS     65536

#define MAX_KEY     32
#define MAX_VALUE   1024

#define PLANE_X 0     // Plane is perpendicular to given axis
#define PLANE_Y 1
#define PLANE_Z 2
#define PLANE_ANYX 3  // Non-axial plane is snapped to the nearest
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

#define MT_MODEL_SOLID 61
#define MT_MODEL_ILLUSIONARY 62

#define CS_PLAYER_HEIGHT 74.0f // Player height in GoldSrc units
#define CS_PLAYER_RADIUS 18.0f // Player width/2 in GoldSrc units
#define CS_PLAYER_CLIMB 18.0f  // Max step size in GoldSrc units


typedef struct _VECTOR3D
{
	float x, y, z;
} VECTOR3D;

typedef VECTOR3D BSPVERTEX;

typedef struct _BSPLUMP
{
	int32_t nOffset; // File offset to data
	int32_t nLength; // Length of data
} BSPLUMP;


typedef struct _BSPHEADER
{
	int32_t nVersion;           // Must be 30 for a valid HL BSP file
	BSPLUMP lump[HEADER_LUMPS]; // Stores the directory of lumps
} BSPHEADER;


typedef struct _BSPPLANE
{
	VECTOR3D vNormal; // The planes normal vector
	float fDist;      // Plane equation is: vNormal * X = fDist
	int32_t nType;    // Plane type, see #defines
} BSPPLANE;

typedef int32_t BSPSURFEDGE;

typedef struct _BSPEDGE
{
	uint16_t iVertex[2]; // Indices into vertex array
} BSPEDGE;

typedef struct _BSPFACE
{
	uint16_t iPlane;          // Plane the face is parallel to
	uint16_t nPlaneSide;      // Set if different normals orientation
	uint32_t iFirstEdge;      // Index of the first surfedge
	uint16_t nEdges;          // Number of consecutive surfedges
	uint16_t iTextureInfo;    // Index of the texture info structure
	uint8_t nStyles[4];       // Specify lighting styles
	uint32_t nLightmapOffset; // Offsets into the raw lightmap data
} BSPFACE;

typedef struct _BSPMODEL
{
	float nMins[3], nMaxs[3];          // Defines bounding box
	VECTOR3D vOrigin;                  // Coordinates to move the // coordinate system
	int32_t iHeadnodes[MAX_MAP_HULLS]; // Index into nodes array
	int32_t nVisLeafs;                 // ???
	int32_t iFirstFace, nFaces;        // Index and count into faces
} BSPMODEL;

typedef struct _NAVMODEL
{
	BSPMODEL model;
	int modelType;
} NAVMODEL;

typedef struct _TRIANGLEDATA
{
	float verts[MAX_MAP_VERTS * 3];
	int nverts;
	int tris[MAX_MAP_VERTS * 3];
	int surfaceType[MAX_MAP_VERTS * 3];
	int ntris;
} TRIANGLEDATA;

typedef struct _BSPENTITYPROPERTY
{
	char propertyName[1024];
	char propertyValue[1024];
} BSPENTITYPROPERTY;

typedef struct _BSPENTITYDEF
{
	int startPos;
	int endPos;
	char data[1024];
	int numProperties;
	BSPENTITYPROPERTY properties[10];
} BSPENTITYDEF;

inline bool FStrEq(const char*sz1, const char*sz2)

{
	return (strcmp(sz1, sz2) == 0);
}


#endif // BSP_H