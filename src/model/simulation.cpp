#include <simulation.h>
#include <document.h>

namespace model
{

class Simulation::RenderVisitor
{
private:
	EntityList m_entities;

public:
	void start(float depth)
	{
	}

	void end(const QVector2D& to)
	{
	}

	void startOperation(const QVector2D& to, float intensity)
	{
	}

	void endOperation(float depth)
	{
	}

	void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate, float depthFeedRate)
	{
		// TODO depth motion ?
		polyline.forEachBulge([this, depth, planeFeedRate, depthFeedRate](const geometry::Bulge& bulge){
			convertBulgeToMotion()
		});
	}
};

Simulation::Simulation(Document &document)
{
	
}

}

