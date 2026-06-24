#include "v8datamodel/LocalPlayerHopper.h"

namespace RBX
{
	LocalPlayerHopperItem::LocalPlayerHopperItem() {}

	void LocalPlayerHopperItem::onClick(const GuiEvent& event)
	{
		LocalPlayerHopper* parent = static_cast<LocalPlayerHopper*>(getParent());
		RBXASSERT(fastDynamicCast<LocalPlayerHopper>(getParent()) == parent);

		parent->onClick(this);
	}

	G3D::Vector2 LocalPlayerHopperItem::getSize() const
	{
		float width = GuiRoot::toPixelSize(G3D::Vector2(10.0f, 10.0f)).x;
		return G3D::Vector2(width, width);
	}

	LocalPlayerHopper::LocalPlayerHopper()
		: lastRemovedIndex(-1)
	{
		yLocation = Rect::BOTTOM;
	}

	LocalPlayerHopper::~LocalPlayerHopper()
	{
		clearAll();
	}

	void LocalPlayerHopper::onEvent(const Instance* source, ChildAdded event)
	{
		Instance* child = event.child.get();

		if (Network::Player* player = fastDynamicCast<Network::Player>(child))
		{
			if (player == Network::Players::findLocalPlayer(this))
			{
				onLocalPlayerAdded(player);
			}
		}
		else
		{
			if (PlayerHopper* PlayerHopper = fastDynamicCast<PlayerHopper>(child))
			{
				onLocalPlayerHopperAdded(PlayerHopper);
			}
			else if (PlayerHopperItem* PlayerHopperItem = fastDynamicCast<PlayerHopperItem>(child))
			{
				insertPlayerHopperItem(PlayerHopperItem);
			}
		}
	}

	void LocalPlayerHopper::onEvent(const Instance* source, ChildRemoved event)
	{
		Instance* child = event.child.get();

		if (Network::Player* player = fastDynamicCast<Network::Player>(child))
		{
			if (player == localPlayer.get())
			{
				clearLocalPlayer();
			}
		}
		else
		{
			if (PlayerHopper* PlayerHopper = fastDynamicCast<PlayerHopper>(child))
			{
				RBXASSERT(localPlayerHopper);
				clearLocalPlayerHopper();
			}
			else if (PlayerHopperItem* PlayerHopperItem = fastDynamicCast<PlayerHopperItem>(child))
			{
				removePlayerHopperItem(PlayerHopperItem);
			}
		}
	}

	void LocalPlayerHopper::onEvent(const Instance* source, DescendentAdded event)
	{
		RBXASSERT(source == localCharacter.get());

		PlayerHopperItem* PlayerHopperItem = fastDynamicCast<PlayerHopperItem>(event.instance.get());
		if (PlayerHopperItem)
			insertPlayerHopperItem(PlayerHopperItem);
	}

	void LocalPlayerHopper::onEvent(const Instance* source, DescendentRemoving event)
	{
		RBXASSERT(source == localCharacter.get());

		PlayerHopperItem* PlayerHopperItem = fastDynamicCast<PlayerHopperItem>(event.instance.get());
		if (PlayerHopperItem)
			removePlayerHopperItem(PlayerHopperItem);
	}

	void LocalPlayerHopper::onClick(LocalPlayerHopperItem* clickedItem)
	{
		pendingClick = shared_from(clickedItem->getItem());
	}
}
