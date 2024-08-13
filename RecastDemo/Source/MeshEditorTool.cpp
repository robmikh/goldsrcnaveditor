#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_opengl.h"
#ifdef __APPLE__
#	include <OpenGL/glu.h>
#else
#	include <GL/glu.h>
#endif
#include "imgui.h"
#include "MeshEditorTool.h"
#include "Sample.h"
#include "InputGeom.h"
#include "MeshLoaderObj.h"
#include "NavProfiles.h"

#include "BSP.h"

#ifdef WIN32
#	define snprintf _snprintf
#endif

// Uncomment this to dump all the requests in stdout.
#define DUMP_REQS

MeshEditorTool::MeshEditorTool() :
	m_sample(0),
	m_ChosenSurfaceType(MT_MODEL_SOLID),
	m_ApplyByBrush(true)
{
	
}

void MeshEditorTool::handleMenu()
{
	imguiLabel("Application Scope");
	imguiIndent();
	
	if (imguiCheck("By Brush", m_ApplyByBrush))
	{
		m_ApplyByBrush = true;
	}
	if (imguiCheck("By Triangle", !m_ApplyByBrush))
	{
		m_ApplyByBrush = false;
	}

	imguiSeparator();

	imguiLabel("Selected Surface Type");
	imguiIndent();

	if (imguiCheck("Automatic", m_ChosenSurfaceType == MT_MODEL_SOLID))
	{
		m_ChosenSurfaceType = MT_MODEL_SOLID;
	}
	if (imguiCheck("Non-Solid", m_ChosenSurfaceType == MT_MODEL_ILLUSIONARY))
	{
		m_ChosenSurfaceType = MT_MODEL_ILLUSIONARY;
	}

	vector<NavAreaDefinition> AllAreas = GetAllNavAreaDefinitions();

	for (auto it = AllAreas.begin(); it != AllAreas.end(); it++)
	{
		if (imguiCheck(it->AreaName.c_str(), m_ChosenSurfaceType == it->NavAreaIndex))
		{
			m_ChosenSurfaceType = it->NavAreaIndex;
		}
	}

	imguiSeparator();

}

void MeshEditorTool::init(Sample* sample)
{
	m_sample = sample;
	m_ChosenSurfaceType = MT_MODEL_SOLID;
}

void MeshEditorTool::reset()
{
	m_ChosenSurfaceType = MT_MODEL_SOLID;
}

void MeshEditorTool::handleClick(const float* s, const float* p, bool shift)
{
	const float* verts = m_sample->getInputGeom()->getMesh()->getVerts();
	const int* tris = m_sample->getInputGeom()->getMesh()->getTris();
	const int ntris = m_sample->getInputGeom()->getMesh()->getTriCount();
	const int* surfTypes = m_sample->getInputGeom()->getMesh()->getSurfaceTypes();

	float tmin = 1.0f;
	int NewTriNum = 0;
	bool hit = false;

	float NewPos[3];
	rcVsub(NewPos, p, s);
	rcVnormalize(NewPos);
	float dist = rcVdist(s, p);
	rcVmad(NewPos, p, NewPos, dist + 6.0f);

	for (int j = 0; j < ntris * 3; j += 3)
	{
		int thisSurfType = surfTypes[j / 3];

		if (m_sample->getInputGeom()->getHideIllusionary() && thisSurfType == MT_MODEL_ILLUSIONARY) { continue; }

		float t = 1;
		if (InputGeom::intersectSegmentTriangle(s, NewPos,
			&verts[tris[j] * 3],
			&verts[tris[j + 1] * 3],
			&verts[tris[j + 2] * 3], t))
		{
			if (t < tmin)
			{
				tmin = t;
				NewTriNum = (int)ceilf((float)j / 3.0f);
			}
			hit = true;
		}
	}

	if (hit)
	{
		if (m_ApplyByBrush)
		{
			m_sample->SetModelSurfaceType(NewTriNum, m_ChosenSurfaceType);
		}
		else
		{
			m_sample->SetTriangleSurfaceType(NewTriNum, m_ChosenSurfaceType);
		}
		
	}

}

void MeshEditorTool::handleToggle()
{

}

void MeshEditorTool::handleStep()
{

}

void MeshEditorTool::handleUpdate(const float /*dt*/)
{

}

void MeshEditorTool::handleRender()
{

}

void MeshEditorTool::handleRenderOverlay(double* proj, double* model, int* view)
{
	rcIgnoreUnused(model);
	rcIgnoreUnused(proj);

	// Tool help
	const int h = view[3];

	imguiDrawText(280, h - 40, IMGUI_ALIGN_LEFT, "LMB: Click a triangle to apply the selected surface type to it", imguiRGBA(255, 255, 255, 192));
}