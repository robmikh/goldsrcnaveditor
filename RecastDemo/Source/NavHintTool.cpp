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
#include "NavHintTool.h"
#include "Sample.h"
#include "InputGeom.h"
#include "MeshLoaderObj.h"

#include "NavProfiles.h"

#ifdef WIN32
#	define snprintf _snprintf
#endif

// Uncomment this to dump all the requests in stdout.
#define DUMP_REQS

NavHintTool::NavHintTool() :
	m_sample(0),
	m_currentHintType(0)
{

}

void NavHintTool::handleMenu()
{
	imguiLabel("Hint Type");
	imguiIndent();

	vector<NavHintDefinition> AllHintTypes = GetAllNavHintDefinitions();

	for (auto it = AllHintTypes.begin(); it != AllHintTypes.end(); it++)
	{
		if (imguiCheck(it->HintName.c_str(), (m_currentHintType & it->HintId)))
		{
			m_currentHintType ^= it->HintId;
		}
	}

}

void NavHintTool::init(Sample* sample)
{
	m_sample = sample;
	m_currentHintType = 0;
}

void NavHintTool::reset()
{
	m_currentHintType = 0;
}

void NavHintTool::handleClick(const float* s, const float* p, bool shift)
{
	if (!m_sample) { return; }

	InputGeom* geom = m_sample->getInputGeom();

	if (!geom) { return; }

	if (shift)
	{
		NavHint* NavHints = geom->getNavHints();
		int NearestIndex = -1;
		float MinDist = FLT_MAX;

		for (int i = 0; i < geom->getMaxNavHints(); i++)
		{
			if (NavHints[i].hintType != 0 && NavHints[i].NavMeshIndex == m_sample->getCurrentNavMeshIndex())
			{
				float ThisDist = rcVdistSqr(p, NavHints[i].position);
				if (ThisDist < rcSqr(32.0f) && (NearestIndex < 0 || ThisDist < MinDist))
				{
					NearestIndex = i;
					MinDist = ThisDist;
				}
			}
		}

		if (NearestIndex > -1)
		{
			geom->removeNavHint(NearestIndex);
		}
	}
	else
	{
		geom->addNavHint(m_sample->getCurrentNavMeshIndex(), p, m_currentHintType);
	}

}

void NavHintTool::handleToggle()
{

}

void NavHintTool::handleStep()
{

}

void NavHintTool::handleUpdate(const float /*dt*/)
{

}

void NavHintTool::handleRender()
{
	if (!m_sample) { return; }

	InputGeom* geom = m_sample->getInputGeom();

	if (!geom) { return; }

	duDebugDraw& dd = m_sample->getDebugDraw();
	NavHint* NavHints = geom->getNavHints();

	int col = duRGBA(255, 192, 0, 192);

	for (int i = 0; i < geom->getNavHintCount(); i++)
	{
		if (NavHints[i].hintType != 0 && NavHints[i].NavMeshIndex == m_sample->getCurrentNavMeshIndex())
		{
			float bmin[3] = { NavHints[i].position[0] - 16.0f, NavHints[i].position[1], NavHints[i].position[2] - 16.0f };
			float bmax[3] = { NavHints[i].position[0] + 16.0f, NavHints[i].position[1] + 32.0f, NavHints[i].position[2] + 16.0f };

			duDebugDrawCylinder(&dd, bmin[0], bmin[1], bmin[2], bmax[0], bmax[1], bmax[2], col);

		}
	}
}

void NavHintTool::handleRenderOverlay(double* proj, double* model, int* view)
{
	if (!m_sample) { return; }

	InputGeom* geom = m_sample->getInputGeom();

	if (!geom) { return; }

	NavHint* NavHints = geom->getNavHints();

	GLdouble x, y, z;

	int NumNavHints = 0;

	for (int i = 0; i < geom->getNavHintCount(); i++)
	{
		if (NavHints[i].hintType != 0)
		{
			if (NavHints[i].NavMeshIndex == m_sample->getCurrentNavMeshIndex())
			{

				if (gluProject((GLdouble)NavHints[i].position[0], (GLdouble)NavHints[i].position[1], (GLdouble)NavHints[i].position[2],
					model, proj, view, &x, &y, &z))
				{
					int yOffset = 25;

					vector<NavHintDefinition> AllNavHints = GetAllNavHintDefinitions();

					for (auto it = AllNavHints.begin(); it != AllNavHints.end(); it++)
					{
						if (NavHints[i].hintType & it->HintId)
						{
							imguiDrawText((int)x, (int)(y - yOffset), IMGUI_ALIGN_CENTER, it->HintName.c_str(), imguiRGBA(0, 0, 0, 220));
							yOffset += 15;
						}
					}
				}				
			}
			NumNavHints++;
		}
	}

	const int h = view[3];
	imguiDrawText(280, h - 40, IMGUI_ALIGN_LEFT, "LMB: Add new hint.  SHIFT+LMB: Delete nav hint", imguiRGBA(255, 255, 255, 192));

	char ConCount[64];
	sprintf(ConCount, "Total Hint Count (All Meshes): %d / %d", NumNavHints, geom->getMaxNavHints());
	imguiDrawText(280, h - 60, IMGUI_ALIGN_LEFT, ConCount, imguiRGBA(255, 255, 255, 192));

	
}

bool NavHintTool::hideIllusionary()
{
	return true;
}