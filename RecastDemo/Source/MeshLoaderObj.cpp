//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "MeshLoaderObj.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#include "BSP.h"

rcMeshLoaderObj::rcMeshLoaderObj() :
	m_scale(1.0f),
	m_verts(0),
	m_tris(0),
	m_normals(0),
	m_vertCount(0),
	m_triCount(0),
	m_surfTypeCount(0),
	m_surfTypes(0)
{
}

rcMeshLoaderObj::~rcMeshLoaderObj()
{
	delete[] m_verts;
	delete[] m_normals;
	delete[] m_tris;
	delete[] m_surfTypes;
}
		
void rcMeshLoaderObj::addVertex(float x, float y, float z, int& cap)
{
	if (m_vertCount+1 > cap)
	{
		cap = !cap ? 8 : cap*2;
		float* nv = new float[cap*3];
		if (m_vertCount)
			memcpy(nv, m_verts, m_vertCount*3*sizeof(float));
		delete [] m_verts;
		m_verts = nv;
	}
	float* dst = &m_verts[m_vertCount*3];
	*dst++ = x*m_scale;
	*dst++ = y*m_scale;
	*dst++ = z*m_scale;
	m_vertCount++;
}

void rcMeshLoaderObj::addTriangle(int a, int b, int c, int& cap, int surfaceType)
{
	if (m_triCount + 1 > cap)
	{
		cap = !cap ? 8 : cap * 2;
		int* nv = new int[cap * 3];
		int* nst = new int[cap];

		if (m_triCount)
		{
			memcpy(nv, m_tris, m_triCount * 3 * sizeof(int));
			memcpy(nst, m_surfTypes, m_triCount * sizeof(int));
		}

		delete[] m_tris;
		delete[] m_surfTypes;
		m_tris = nv;
		m_surfTypes = nst;
	}
	int* dst = &m_tris[m_triCount * 3];
	*dst++ = a;
	*dst++ = b;
	*dst++ = c;

	m_surfTypes[m_triCount] = surfaceType;

	m_triCount++;
	m_surfTypeCount++;
}

static char* parseRow(char* buf, char* bufEnd, char* row, int len)
{
	bool start = true;
	bool done = false;
	int n = 0;
	while (!done && buf < bufEnd)
	{
		char c = *buf;
		buf++;
		// multirow
		switch (c)
		{
			case '\\':
				break;
			case '\n':
				if (start) break;
				done = true;
				break;
			case '\r':
				break;
			case '\t':
			case ' ':
				if (start) break;
				// else falls through
			default:
				start = false;
				row[n++] = c;
				if (n >= len-1)
					done = true;
				break;
		}
	}
	row[n] = '\0';
	return buf;
}

static int parseFace(char* row, int* data, int n, int vcnt)
{
	int j = 0;
	while (*row != '\0')
	{
		// Skip initial white space
		while (*row != '\0' && (*row == ' ' || *row == '\t'))
			row++;
		char* s = row;
		// Find vertex delimiter and terminated the string there for conversion.
		while (*row != '\0' && *row != ' ' && *row != '\t')
		{
			if (*row == '/') *row = '\0';
			row++;
		}
		if (*s == '\0')
			continue;
		int vi = atoi(s);
		data[j++] = vi < 0 ? vi+vcnt : vi-1;
		if (j >= n) return j;
	}
	return j;
}

bool rcMeshLoaderObj::load(const std::string& filename)
{
	char* buf = 0;
	FILE* fp = fopen(filename.c_str(), "rb");
	if (!fp)
		return false;
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		return false;
	}
	long bufSize = ftell(fp);
	if (bufSize < 0)
	{
		fclose(fp);
		return false;
	}
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fclose(fp);
		return false;
	}
	buf = new char[bufSize];
	if (!buf)
	{
		fclose(fp);
		return false;
	}
	size_t readLen = fread(buf, bufSize, 1, fp);
	fclose(fp);

	if (readLen != 1)
	{
		delete[] buf;
		return false;
	}

	char* src = buf;
	char* srcEnd = buf + bufSize;
	char row[512];
	int face[32];
	float x,y,z;
	int nv;
	int vcap = 0;
	int tcap = 0;
	
	while (src < srcEnd)
	{
		// Parse one row
		row[0] = '\0';
		src = parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
		// Skip comments
		if (row[0] == '#') continue;
		if (row[0] == 'v' && row[1] != 'n' && row[1] != 't')
		{
			// Vertex pos
			sscanf(row+1, "%f %f %f", &x, &y, &z);
			addVertex(x, y, z, vcap);
		}
		if (row[0] == 'f')
		{
			// Faces
			nv = parseFace(row+1, face, 32, m_vertCount);
			for (int i = 2; i < nv; ++i)
			{
				const int a = face[0];
				const int b = face[i-1];
				const int c = face[i];
				if (a < 0 || a >= m_vertCount || b < 0 || b >= m_vertCount || c < 0 || c >= m_vertCount)
					continue;
				addTriangle(a, b, c, tcap, 0);
			}
		}
	}

	delete [] buf;

	// Calculate normals.
	m_normals = new float[m_triCount*3];
	for (int i = 0; i < m_triCount*3; i += 3)
	{
		const float* v0 = &m_verts[m_tris[i]*3];
		const float* v1 = &m_verts[m_tris[i+1]*3];
		const float* v2 = &m_verts[m_tris[i+2]*3];
		float e0[3], e1[3];
		for (int j = 0; j < 3; ++j)
		{
			e0[j] = v1[j] - v0[j];
			e1[j] = v2[j] - v0[j];
		}
		float* n = &m_normals[i];
		n[0] = e0[1]*e1[2] - e0[2]*e1[1];
		n[1] = e0[2]*e1[0] - e0[0]*e1[2];
		n[2] = e0[0]*e1[1] - e0[1]*e1[0];
		float d = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
		if (d > 0)
		{
			d = 1.0f/d;
			n[0] *= d;
			n[1] *= d;
			n[2] *= d;
		}
	}
	
	m_filename = filename;
	return true;
}

char* GetEntityDefClassName(BSPENTITYDEF* entity)
{
	return entity->properties[entity->numProperties].propertyValue;
}

char* GetEntityPropertyValueByName(BSPENTITYDEF* entity, const char* name)
{
	for (int i = 0; i < entity->numProperties; i++)
	{
		if (strcmp(entity->properties[i].propertyName, name) == 0)
		{
			return entity->properties[i].propertyValue;
		}
	}

	return "";
}

char* GetEntityDefClassname(BSPENTITYDEF* entity)
{
	return entity->properties[entity->numProperties - 1].propertyValue;
}

int GetEntityModelRef(BSPENTITYDEF* entity)
{

	for (int i = 0; i < entity->numProperties; i++)
	{
		if (strcmp(entity->properties[i].propertyName, "model") == 0)
		{
			return atoi(entity->properties[i].propertyValue + 1);
		}
	}

	return -1;
}

bool rcMeshLoaderObj::loadBSP(const std::string& filename)
{

	TRIANGLEDATA* triData = (TRIANGLEDATA*)malloc(sizeof(TRIANGLEDATA));
	memset(triData, 0, sizeof(TRIANGLEDATA));

	BSPHEADER fileHeader; // The BSP file header

	BSPFACE* faces; // Faces array
	BSPEDGE* edges; // Edges array
	BSPSURFEDGE* surfedges; // Surfedges array
	BSPMODEL* models; // Models array
	BSPENTITYDEF* entityDefs;
	char* entitiesText; // Entities Array

	// Open the BSP and OBJ files

	FILE* bspfile = fopen(filename.c_str(), "rb");

	if (bspfile)
	{

		fread(&fileHeader, sizeof(fileHeader), 1, bspfile);

		// Determine the number of vertices/edges/faces/surfedges and create the arrays as required
		triData->nverts = fileHeader.lump[LUMP_VERTICES].nLength / sizeof(BSPVERTEX);
		int numSurfEdges = fileHeader.lump[LUMP_SURFEDGES].nLength / sizeof(BSPSURFEDGE);
		int numEdges = fileHeader.lump[LUMP_EDGES].nLength / sizeof(BSPEDGE);
		int numFaces = fileHeader.lump[LUMP_FACES].nLength / sizeof(BSPFACE);
		int numModels = fileHeader.lump[LUMP_MODELS].nLength / sizeof(BSPMODEL);

		int vertsLength = triData->nverts * sizeof(BSPVERTEX);
		int surfEdgesLength = numSurfEdges * sizeof(BSPSURFEDGE);
		int edgesLength = numEdges * sizeof(BSPEDGE);
		int facesLength = numFaces * sizeof(BSPFACE);
		int modelsLength = numModels * sizeof(BSPMODEL);
		int trisLength = MAX_MAP_TRIS * 3 * sizeof(int);
		int entitiesLength = fileHeader.lump[LUMP_ENTITIES].nLength;

		int numTris = 0;

		memset(&triData->verts, 0, vertsLength);
		memset(&triData->tris, 0, trisLength);

		edges = (BSPEDGE*)malloc(edgesLength);
		memset(edges, 0, edgesLength);

		faces = (BSPFACE*)malloc(facesLength);
		memset(faces, 0, facesLength);

		surfedges = (BSPSURFEDGE*)malloc(surfEdgesLength);
		memset(surfedges, 0, surfEdgesLength);

		models = (BSPMODEL*)malloc(modelsLength);
		memset(models, 0, modelsLength);

		entitiesText = (char*)malloc(entitiesLength);
		memset(entitiesText, 0, entitiesLength);

		entityDefs = (BSPENTITYDEF*)malloc(MAX_MAP_ENTITIES * sizeof(BSPENTITYDEF));
		memset(entityDefs, 0, sizeof(BSPENTITYDEF) * MAX_MAP_ENTITIES);


		// Read in the vertices/edges/surfedges/faces
		fseek(bspfile, fileHeader.lump[LUMP_VERTICES].nOffset, SEEK_SET);
		fread(triData->verts, vertsLength, 1, bspfile);

		fseek(bspfile, fileHeader.lump[LUMP_EDGES].nOffset, SEEK_SET);
		fread(edges, edgesLength, 1, bspfile);

		fseek(bspfile, fileHeader.lump[LUMP_SURFEDGES].nOffset, SEEK_SET);
		fread(surfedges, surfEdgesLength, 1, bspfile);

		fseek(bspfile, fileHeader.lump[LUMP_FACES].nOffset, SEEK_SET);
		fread(faces, facesLength, 1, bspfile);

		fseek(bspfile, fileHeader.lump[LUMP_MODELS].nOffset, SEEK_SET);
		fread(models, modelsLength, 1, bspfile);

		fseek(bspfile, fileHeader.lump[LUMP_ENTITIES].nOffset, SEEK_SET);
		fread(entitiesText, entitiesLength, 1, bspfile);

		// First count how many entities we have
		int nEntities = 0;

		int propertyCounter = 0;
		char* propPos;
		propPos = strchr(entitiesText, '"');

		while (propPos != NULL)
		{
			int startName = (int)(propPos - entitiesText + 1);
			propPos = strchr(propPos + 1, '"');
			int endName = (int)(propPos - entitiesText);

			memcpy(entityDefs[nEntities].properties[propertyCounter].propertyName, &entitiesText[startName], endName - startName);
			entityDefs[nEntities].properties[propertyCounter].propertyName[endName - startName] = '\0';

			propPos = strchr(propPos + 1, '"');
			int startValue = (int)(propPos - entitiesText + 1);
			propPos = strchr(propPos + 1, '"');
			int endValue = (int)(propPos - entitiesText);

			memcpy(entityDefs[nEntities].properties[propertyCounter].propertyValue, &entitiesText[startValue], endValue - startValue);
			entityDefs[nEntities].properties[propertyCounter].propertyValue[endValue - startValue] = '\0';

			if (strcmp(entityDefs[nEntities].properties[propertyCounter].propertyName, "classname") == 0)
			{
				entityDefs[nEntities].numProperties = ++propertyCounter;
				nEntities++;
				propertyCounter = 0;
			}
			else
			{
				propertyCounter++;
			}

			propPos = strchr(propPos + 1, '"');
		}

		// Swap the Y and Z axis and mirror the Z axis (OpenGL and recast/detour use right-handed coordinates)
		for (int i = 0; i < triData->nverts; i++)
		{
			float newY = triData->verts[(i * 3) + 2];
			float newZ = -triData->verts[(i * 3) + 1];

			triData->verts[(i * 3) + 1] = newY;
			triData->verts[(i * 3) + 2] = newZ;
		}

		int validFaces[MAX_MAP_FACES];
		int faceTypes[MAX_MAP_FACES];
		int modelFaceIndices[MAX_MAP_FACES];
		NAVMODEL validModels[MAX_MAP_MODELS];
		memset(validFaces, 0, sizeof(validFaces));
		memset(faceTypes, 0, sizeof(faceTypes));
		memset(validModels, 0, sizeof(validModels));

		int modelCounter = 1;

		int solidFaceCounter = 0;

		m_triModels = (int*)malloc(sizeof(int) * MAX_MAP_TRIS);
		memset(m_triModels, 0, sizeof(int) * MAX_MAP_TRIS);

		validModels[0].model = models[0];
		validModels[0].modelType = MT_MODEL_SOLID;

		int lcap = 0;

		for (int i = 0; i < nEntities; i++)
		{

			if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_wall"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_SOLID;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_ladder"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_SOLID;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_seethrough"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_SOLID;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_conveyor"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_SOLID;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_illusionary"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_water"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_door"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_door_rotating"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_seethroughdoor"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_button"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_breakable"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
			else if (FStrEq(GetEntityDefClassname(&entityDefs[i]), "func_train"))
			{
				validModels[modelCounter].model = models[GetEntityModelRef(&entityDefs[i])];
				validModels[modelCounter++].modelType = MT_MODEL_ILLUSIONARY;
			}
		}

		m_modelCount = modelCounter;

		for (int x = 0; x < modelCounter; x++)
		{
			for (int i = 0; i < validModels[x].model.nFaces; i++)
			{
				faceTypes[solidFaceCounter] = validModels[x].modelType;
				modelFaceIndices[solidFaceCounter] = x;
				validFaces[solidFaceCounter++] = validModels[x].model.iFirstFace + i;
			}
		}

		//m_verts = (float*)malloc(sizeof(float) * MAX_MAP_TRIS);

		int triCounter = 0;
		int surftypeCounter = 0;

		// Loop through faces
		for (int i = 0; i < solidFaceCounter; i++)
		{
			// Find the first edge in the face and extract all the consecutive edges into the sEdges array
			uint32_t iFirstEdge = (faces + validFaces[i])->iFirstEdge;
			int faceType = faceTypes[i];
			int modelIndex = modelFaceIndices[i];
			uint16_t nEdges = (faces + validFaces[i])->nEdges;
			size_t sEdgesLength = nEdges * sizeof(BSPSURFEDGE);
			size_t sIndicesLength = nEdges * sizeof(uint16_t);

			BSPSURFEDGE* sEdges = (BSPSURFEDGE*)malloc(sEdgesLength);
			memset(sEdges, 0, sEdgesLength);

			for (int x = 0; x < nEdges; x++)
			{
				sEdges[x] = surfedges[iFirstEdge + x];
			}

			// Make room for the total number of indices
			uint16_t* sIndices = (uint16_t*)malloc(sIndicesLength);

			if (!sIndices)
			{
				return false;
			}

			memset(sIndices, 0, sIndicesLength);

			int counter = 0;

			// Take the edges and turn them into indices
			for (int x = 0; x < (nEdges - 1); x++)
			{
				int index = sEdges[x];
				// If this is the first edge, then take both vertices...
				if (x == 0)
				{
					if (index >= 0)
					{
						sIndices[counter++] = (edges + index)->iVertex[0];
						sIndices[counter++] = (edges + index)->iVertex[1];
					}
					else
					{
						// Negative surfedges mean reverse the order
						int newIndex = abs(index);
						sIndices[counter++] = (edges + newIndex)->iVertex[1];
						sIndices[counter++] = (edges + newIndex)->iVertex[0];
					}
				}
				else
				{
					// ...Otherwise just take the second vertex in the edge
					if (index >= 0)
					{
						sIndices[counter++] = (edges + index)->iVertex[1];
					}
					else
					{
						// Or the first if it's reversed
						int newIndex = abs(index);
						sIndices[counter++] = (edges + newIndex)->iVertex[0];
					}
				}
			}


			// Thankfully all faces in a HL BSP are convex, so triangulate by fanning out from the first vertex
			int indexCounter = 1;
			int totalIndices = (nEdges - 2);

			numTris += totalIndices;

			for (int x = 0; x < totalIndices; x++)
			{
				triData->surfaceType[surftypeCounter] = faceType;

				if (faceType == MT_MODEL_ILLUSIONARY)
				{
					bool bBoop = true;
				}


				m_triModels[surftypeCounter] = modelIndex;
				triData->tris[triCounter++] = sIndices[0];
				triData->tris[triCounter++] = sIndices[++indexCounter];
				triData->tris[triCounter++] = sIndices[indexCounter - 1];

				surftypeCounter++;

			}

			// Clean up
			free(sIndices);
			free(sEdges);
		}

		triData->ntris = numTris;

		int vcap = 0;
		int tcap = 0;



		for (int i = 0; i < triData->nverts; i++)
		{
			addVertex(triData->verts[(i * 3)], triData->verts[(i * 3) + 1], triData->verts[(i * 3) + 2], vcap);
		}

		for (int i = 0; i < triData->ntris; i++)
		{
			addTriangle(triData->tris[(i * 3)], triData->tris[(i * 3) + 1], triData->tris[(i * 3) + 2], tcap, triData->surfaceType[i]);
		}

		// Calculate normals.
		m_normals = new float[m_triCount * 3];
		for (int i = 0; i < m_triCount * 3; i += 3)
		{
			const float* v0 = &m_verts[m_tris[i] * 3];
			const float* v1 = &m_verts[m_tris[i + 1] * 3];
			const float* v2 = &m_verts[m_tris[i + 2] * 3];
			float e0[3], e1[3];
			for (int j = 0; j < 3; ++j)
			{
				e0[j] = v1[j] - v0[j];
				e1[j] = v2[j] - v0[j];
			}
			float* n = &m_normals[i];
			n[0] = e0[1] * e1[2] - e0[2] * e1[1];
			n[1] = e0[2] * e1[0] - e0[0] * e1[2];
			n[2] = e0[0] * e1[1] - e0[1] * e1[0];
			float d = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
			if (d > 0)
			{
				d = 1.0f / d;
				n[0] *= d;
				n[1] *= d;
				n[2] *= d;
			}
		}

		m_filename = filename;

		std::string base_filename = m_filename.substr(m_filename.find_last_of("/\\") + 1);
		std::string::size_type const p(base_filename.find_last_of('.'));
		m_mapname = base_filename.substr(0, p);

		fclose(bspfile);
		free(faces);
		free(edges);
		free(surfedges);
		free(triData);

		return true;

	}

	return false;

}