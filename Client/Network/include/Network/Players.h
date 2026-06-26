#pragma once
#include "Network/Player.h"
#include "v8tree/Service.h"
#include "v8datamodel/ModelInstance.h"
#include <list>
#include <queue>

class RakPeerInterface;
struct Packet;

template<class Class>
class PluginInterfaceAdapter;

namespace RBX
{
	namespace Network
	{
		class Player;
		struct CharacterAdded;

		extern const char* sPlayers;
		class Players : public DescribedNonCreatable<Players, Instance, &sPlayers>,
						public Service,
						public Notifier<Players, ChatMessage>,
						public Notifier<Players, SuperSafeChanged>,
						public Listener<Player, CharacterAdded>
		{
		private:
			class Plugin;

		private:
			boost::scoped_ptr<Plugin> plugin;
			CopyOnWrite<std::vector<boost::shared_ptr<Instance>>> players;
			boost::shared_ptr<Player> localPlayer;
			RakPeerInterface* peer;
			int maxPlayers;

		public:
			static Reflection::RefPropDescriptor<Players, Player> propLocalPlayer;
		  
		private:
			virtual void onEvent(const Player*, CharacterAdded);
			virtual void onChildChanged(Instance*, const PropertyChanged&);
		public:
			Players();
			virtual ~Players();
		public:
			boost::shared_ptr<Instance> createLocalPlayer(int);
			Player* getLocalPlayer() const
			{
				return localPlayer.get();
			}
			int getNumPlayers() const;
			int numPlayers() const
			{
				return (int)players->size();
			}
			int getMaxPlayers() const
			{
				return maxPlayers;
			}
			void setMaxPlayers(int value);
			boost::shared_ptr<const std::vector<boost::shared_ptr<Instance>>> getPlayers()
			{
				return players.read();
			}
			bool OnReceive(RakPeerInterface*, Packet*);
			void setConnection(RakPeerInterface* peer);
			boost::shared_ptr<Instance> playerFromCharacter(boost::shared_ptr<Instance> character);
			boost::shared_ptr<Instance> getPlayerByID(int userID);
		protected:
			virtual bool askAddChild(const Instance* instance) const;
			virtual void onChildAdded(Instance*);
			virtual void onChildRemoving(Instance*);
		  
		public:
			static Player* getPlayerFromCharacter(Instance* character);
			static ModelInstance* findLocalCharacter(const Instance* context);
			static Player* findLocalPlayer(const Instance* context);
			static bool clientIsPresent(const Instance* context, bool testInDatamodel);
			static bool serverIsPresent(const Instance*, bool);
			static bool frontendProcessing(const Instance* context, bool testInDatamodel);
			static bool backendProcessing(const Instance*, bool);
		};
	}
}
