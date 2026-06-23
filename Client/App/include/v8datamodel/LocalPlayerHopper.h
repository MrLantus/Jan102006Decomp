#pragma once
#include "v8datamodel/Hopper.h"
#include "Network/Player.h"

namespace RBX
{
	extern const char* sLocalPlayerHopperItem;
	class LocalPlayerHopperItem : public DescribedCreatable<LocalPlayerHopperItem, Widget, &sLocalPlayerHopperItem>
	{
	private:
		boost::shared_ptr<PlayerHopperItem> myItem;
		GuiDrawImage guiImageDraw;

	private:
		virtual bool askSetParent(const Instance* instance) const
		{
			return true;
		}

		virtual G3D::Vector2 getSize() const;
		virtual void render2d(Adorn*);
		virtual void onClick(const GuiEvent& event);
		int getBinId() const;
	public:
		LocalPlayerHopperItem();

		void setItem(PlayerHopperItem* item)
		{
			myItem = shared_from(item);
		}

		PlayerHopperItem* getItem()
		{
			return myItem.get();
		}
	};

	extern const char* sLocalPlayerHopper;
	class LocalPlayerHopper : public DescribedCreatable<LocalPlayerHopper, RelativePanel, &sLocalPlayerHopper>,
						  public Service,
						  public Listener<Instance, ChildAdded>,
						  public Listener<Instance, ChildRemoved>,
						  public Listener<Instance, DescendentAdded>,
						  public Listener<Instance, DescendentRemoving>,
						  public Listener<Network::Player, Network::CharacterAdded>,
						  public Listener<Network::Player, Network::CharacterRemoving>
	{
	private:
		boost::shared_ptr<Network::Players> players;
		boost::shared_ptr<Network::Player> localPlayer;
		boost::shared_ptr<PlayerHopper> localPlayerHopper;
		boost::shared_ptr<Instance> localCharacter;
		boost::shared_ptr<PlayerHopperItem> pendingClick;
		int lastRemovedIndex;

	private:
		virtual bool askSetParent(const Instance* instance) const
		{
			return true;
		}
		virtual void onServiceProvider(const ServiceProvider*, const ServiceProvider*);
		virtual void onEvent(const Network::Player* source, Network::CharacterRemoving event);
		virtual void onEvent(const Network::Player* source, Network::CharacterAdded event);
		virtual void onEvent(const Instance* source, DescendentRemoving event);
		virtual void onEvent(const Instance* source, DescendentAdded event);
		virtual void onEvent(const Instance* source, ChildRemoved event);
		virtual void onEvent(const Instance* source, ChildAdded event);
		void onLocalCharacterAdded(Instance*);
		void onLocalPlayerAdded(Network::Player*);
		void onLocalPlayerHopperAdded(PlayerHopper*);
		void clearAll();
		void clearLocalCharacter();
		void clearLocalPlayer();
		void clearLocalPlayerHopper();
		void insertPlayerHopperItem(PlayerHopperItem*);
		void removePlayerHopperItem(PlayerHopperItem*);
	public:
		LocalPlayerHopper();
		virtual ~LocalPlayerHopper();
		void onHeartbeat();
		void onClick(LocalPlayerHopperItem* clickedItem);
		bool onKey(int);
		void cycleUp();
		void cycleDown();

		virtual XmlElement* write()
		{
			return NULL;
		}
	};
}
