// TODO - saturted lighting. Until this is properly implented I will just use a shoddy implentation by modifiyng brickcolors - see 
// https://github.com/MrLantus/Jan102006Decomp/issues/1
#include "v8datamodel/Lighting.h"
#include "util/RunStateOwner.h"

namespace RBX
{
	const char* sLighting = "Lighting";

	static const Reflection::PropDescriptor<Lighting, G3D::Color3> desc_AmbientTop("TopAmbient", "Appearance", &Lighting::getAmbientTop, &Lighting::setAmbientTop, Reflection::PropertyDescriptor::STANDARD);
	static const Reflection::PropDescriptor<Lighting, G3D::Color3> desc_AmbientBottom("BottomAmbient", "Appearance", &Lighting::getAmbientBottom, &Lighting::setAmbientBottom, Reflection::PropertyDescriptor::STANDARD);
	static const Reflection::PropDescriptor<Lighting, G3D::Color3> desc_LightColor("SpotLight", "Appearance", &Lighting::getLightColor, &Lighting::setLightColor, Reflection::PropertyDescriptor::STANDARD);
	static const Reflection::PropDescriptor<Lighting, G3D::Color3> desc_ClearColor("ClearColor", "Appearance", &Lighting::getClearColor3, &Lighting::setClearColor3, Reflection::PropertyDescriptor::STANDARD);
	
	Lighting::Lighting()
		: Base(),
		  ambientTop(207/255.0f, 207/255.0f, 207/255.0f),
		  ambientBottom(205/255.0f, 205/255.0f, 205/255.0f),
		  clearColor(G3D::Color3::white())
	{
		setName("Lighting");
	}

	Lighting::~Lighting()
	{
	}

	void Lighting::setClearColor(G3D::Color4 newClearColor)
	{
		if (newClearColor != clearColor)
		{
			clearColor = newClearColor;
			raisePropertyChanged(desc_ClearColor);
			fireLightingChanged(false);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::setLightColor(G3D::Color3 newLightColor)
	{
		if (newLightColor != skyParameters.lightColor)
		{
			skyParameters.lightColor = newLightColor;
			raisePropertyChanged(desc_LightColor);
			fireLightingChanged(false);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::setAmbientBottom(G3D::Color3 newAmbientBottom)
	{
		if (newAmbientBottom != ambientBottom)
		{
			ambientBottom = newAmbientBottom;
			raisePropertyChanged(desc_AmbientBottom);
			fireLightingChanged(false);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::setAmbientTop(G3D::Color3 newAmbientTop)
	{
		if (newAmbientTop != ambientTop)
		{
			ambientTop = newAmbientTop;
			raisePropertyChanged(desc_AmbientTop);
			fireLightingChanged(false);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::onChildAdded(Instance* instance)
	{
		Sky* newSky = fastDynamicCast<Sky>(instance);

		if (newSky)
		{
			sky = shared_from(newSky);
			fireLightingChanged(true);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::onChildRemoving(Instance* instance)
	{
		{
			fireLightingChanged(true);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	void Lighting::onChildChanged(Instance* instance, const PropertyChanged& propEvent)
	{
		Instance::onChildChanged(instance, propEvent);
		if (sky.get() == instance)
		{
			fireLightingChanged(true);
			RunService* runService = ServiceProvider::findServiceProvider(this)->find<RunService>();
			if (runService)
				runService->invalidateRunViews();
		}
	}

	bool Lighting::askAddChild(const Instance* instance) const
	{
		return fastDynamicCast<const Sky>(instance) != NULL;
	}
}
