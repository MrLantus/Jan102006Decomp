#pragma once
#include "Network/Players.h"
#include "humanoid/Humanoid.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/PlayerHopper.h"
#include "v8datamodel/Workspace.h"

namespace RBX
{
	namespace Network
	{
		struct CharacterAdded
		{
		public:
			const boost::shared_ptr<Instance> character;
		  
		public:
			CharacterAdded(Instance* character)
				: character(shared_from(character))
			{
			}
		};

		struct CharacterRemoving
		{
		public:
			const boost::shared_ptr<Instance> character;
		  
		public:
			CharacterRemoving(Instance* character)
				: character(shared_from(character))
			{
			}
		};

		extern const char* sPlayer;
		class Player : public DescribedCreatable<Player, Instance, &sPlayer>,
					   public Notifier<Player, CharacterAdded>,
					   public Notifier<Player, CharacterRemoving>
		{
		private:
			boost::shared_ptr<ModelInstance> character;
			boost::signals::scoped_connection characterDiedConnection;
			std::string characterAppearance;
			int userId;
			G3D::RealTime lastActivityTime;

		public:
			static Reflection::BoundProp<int, 1> prop_userId;
			static Reflection::SignalDesc<Player, void(std::string, boost::shared_ptr<Instance>)> event_Chatted;

		private:
			virtual bool askAddChild(const Instance*) const;
			virtual void onServiceProvider(const ServiceProvider* Provider);
			void onCharacterChangedFrontend();
			void registerLocalPlayerNotIdle();
		public:
			Player();
			virtual ~Player();
		public:
			virtual XmlElement* write();
			virtual void setName(const std::string& value);
			ModelInstance* getCharacter() const
			{
				return character.get();
			}
			void setCharacterAppearance(const std::string& value);
			int getUserID() const
			{
				return userId;
			}
			void rebuildPlayerHopper();
			PlayerHopper* getLocalPlayerHopper() const;
			void loadCharacter();
			void removeCharacter();
			void removeCharacterAppearance();
			void loadCharacterAppearance();
		private:
			void doPeriodicIdleCheck();

		public:
			static void onLocalPlayerNotIdle(ServiceProvider*);
		};
	}
}
