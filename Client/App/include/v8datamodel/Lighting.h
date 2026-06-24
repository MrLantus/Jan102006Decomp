// TODO - saturted lighting. Until this is properly implented I will just use a shoddy implentation by modifiyng brickcolors - see 
// https://github.com/MrLantus/Jan102006Decomp/issues/1
#pragma once
#include <GLG3D/LightingParameters.h>
#include "v8tree/Service.h"
#include "v8datamodel/Sky.h"

namespace RBX
{
	class Sky;

	extern const char* sLighting;
	class Lighting : public DescribedCreatable<Lighting, Instance, &sLighting>,
					 public Service
	{
	private:
		G3D::Color3 ambientTop;
		G3D::Color3 ambientBottom;
		G3D::Color4 clearColor;
	public:
		boost::shared_ptr<Sky> sky;

	public:
		static Reflection::SignalDesc<Lighting, void(bool)> event_LightingChanged;

	public:
		//Lighting(const Lighting&);
		Lighting();
	public:
		G3D::Color4 getClearColor() const;
		void setClearColor(G3D::Color4 newClearColor);
		G3D::Color3 getLightColor() const
		{
			return skyParameters.lightColor;
		}
		void setLightColor(G3D::Color3 newLightColor);
		G3D::Color3 getAmbientTop() const
		{
			return ambientTop;
		}
		void setAmbientTop(G3D::Color3 newAmbientTop);
		G3D::Color3 getAmbientBottom() const
		{
			return ambientBottom;
		}
		void setAmbientBottom(G3D::Color3 newAmbientBottom);
		void setClearColor3(G3D::Color3 color)
		{
			setClearColor(G3D::Color4(color));
		}
	protected:
		virtual void onChildAdded(Instance* instance);
		virtual void onChildRemoving(Instance* instance);
		virtual void onChildChanged(Instance* instance, const PropertyChanged& propEvent);
		virtual bool askAddChild(const Instance* instance) const;
	private:
		void fireLightingChanged(bool value)
		{
			event_LightingChanged.fire(this, value);
		}
	public:
		virtual ~Lighting();
	public:
		//Lighting& operator=(const Lighting&);
	};
}
