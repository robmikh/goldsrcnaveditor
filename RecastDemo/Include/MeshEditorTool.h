
#ifndef MESHEDITORTOOL_H
#define MESHEDITORTOOL_H

#include "Sample.h"
#include "Recast.h"

class MeshEditorTool : public SampleTool
{
	Sample* m_sample;
	int m_ChosenSurfaceType = 62;
	bool m_ApplyByBrush = true;

public:
	MeshEditorTool();

	virtual int type() { return TOOL_MESH_EDITOR; }
	virtual void init(Sample* sample);
	virtual void reset();
	virtual void handleMenu();
	virtual void handleClick(const float* s, const float* p, bool shift);
	virtual void handleToggle();
	virtual void handleStep();
	virtual void handleUpdate(const float dt);
	virtual void handleRender();
	virtual void handleRenderOverlay(double* proj, double* model, int* view);
};

#endif // NAVMESHTESTERTOOL_H