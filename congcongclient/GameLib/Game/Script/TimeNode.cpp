#include "TimeNode.h"
#include "TimeManager.h"

//using namespace script;

TimeNode::TimeNode()
	:m_bUseTimeNode(false)
{

}
TimeNode::~TimeNode()
{
	if (m_bUseTimeNode)
	{
		TimeManager::Instance().removeByPoint(this);
	}
}
void TimeNode::useTimeNode()
{
	m_bUseTimeNode = true;
}