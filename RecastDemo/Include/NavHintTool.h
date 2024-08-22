#ifndef NAVHINTTOOL_H
#define NAVHINTTOOL_H

#include "Sample.h"
#include "Recast.h"


class NavHintTool : public SampleTool
{
	Sample* m_sample;
	unsigned int m_currentHintType;

public:
	NavHintTool();

	virtual int type() { return TOOL_NAV_HINTS; }
	virtual void init(Sample* sample);
	virtual void reset();
	virtual void handleMenu();
	virtual void handleClick(const float* s, const float* p, bool shift);
	virtual void handleToggle();
	virtual void handleStep();
	virtual void handleUpdate(const float dt);
	virtual void handleRender();
	virtual void handleRenderOverlay(double* proj, double* model, int* view);
	virtual bool hideIllusionary();
};

#endif // NAVHINTTOOL_H