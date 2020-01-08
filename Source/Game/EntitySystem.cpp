#include "Precompiled.hpp"
#include "Game/EntitySystem.hpp"
using namespace Game;

namespace
{
    // Constant variables.
    const EntityHandle::ValueType MaximumIdentifier = std::numeric_limits<EntityHandle::ValueType>::max();
    const EntityHandle::ValueType MaximumVersion = std::numeric_limits<EntityHandle::ValueType>::max();
    const EntityHandle::ValueType InvalidIdentifier = 0;
    const EntityHandle::ValueType StartingIdentifier = 1;
}

EntitySystem::Events::Events()
{
}

EntitySystem::HandleEntry::HandleEntry(EntityHandle::ValueType identifier) :
    handle(),
    flags(HandleFlags::Unused)
{
    handle.identifier = identifier;
}

EntitySystem::EntitySystem() :
    m_entityCount(0)
{
}

EntitySystem::~EntitySystem()
{
    // Destroy all remaining entities.
    this->DestroyAllEntities();

    // Empty the processing queue.
    this->ProcessCommands();
}

EntityHandle EntitySystem::CreateEntity()
{
    // Check if we have reached the numerical limits.
    VERIFY(m_handleEntries.size() != MaximumIdentifier, "Entity identifier limit has been reached!");

    // Check if the next entity entry on the free list is valid.
    while(!m_freeHandles.empty())
    {
        // Retrieve first free handle from the free list.
        HandleEntry* handleEntry = m_freeHandles.front();

        // Ensure that the handle entry is free.
        ASSERT(handleEntry->flags == HandleFlags::Unused, "Handle on the free list is being used!");

        // Check if entity entry has exhausted its possible versions.
        if(handleEntry->handle.version == MaximumVersion)
        {
            // Do not use this handle anymore.
            // Discarding a handle will waste memory.
            // #todo: We should maintain a larger space of available handles 
            // at once to prevent entities in front from expiring too fast.
            m_freeHandles.pop();

            // Attempt to find another candidate for a new entity handle.
            continue;
        }

        // Found a good candidate for a new entity handle.
        break;
    }

    // Create a new handle if the free list queue is empty.
    if(m_freeHandles.empty())
    {
        // Calculate next unused identifier.
        EntityHandle::ValueType identifier = StartingIdentifier + m_handleEntries.size();

        // Create a new handle entry.
        m_handleEntries.emplace_back(identifier);

        // Add new handle entry to the free list queue.
        m_freeHandles.emplace(&m_handleEntries.back());
    }

    // Retrieve an unused handle from the free list.
    HandleEntry* handleEntry = m_freeHandles.front();
    m_freeHandles.pop();

    // Mark handle as created.
    handleEntry->flags |= HandleFlags::Created;

    // Notify about a created entity.
    EntityCommand command;
    command.type = EntityCommands::Created;
    command.handle = handleEntry->handle;
    m_commands.push(command);

    // Return the entity handle.
    return handleEntry->handle;
}

void EntitySystem::DestroyEntity(const EntityHandle& entity)
{
    // Check if the handle is valid.
    if(!IsHandleValid(entity))
        return;

    // Locate the handle entry.
    int handleIndex = entity.identifier - StartingIdentifier;
    HandleEntry& handleEntry = m_handleEntries[handleIndex];

    // Check if entity is already marked to be destroyed.
    if(handleEntry.flags & HandleFlags::Destroy)
        return;

    // Set the handle destroy flag.
    handleEntry.flags |= HandleFlags::Destroy;

    // Add a destroy entity command.
    EntityCommand command;
    command.type = EntityCommands::Destroy;
    command.handle = handleEntry.handle;
    m_commands.push(command);
}

void EntitySystem::DestroyAllEntities()
{
    // Process outstanding entity commands.
    ProcessCommands();

    // Invalidate all handle entries.
    for(HandleEntry& handleEntry : m_handleEntries)
    {
        if(handleEntry.flags & HandleFlags::Created)
        {
            DestroyEntity(handleEntry.handle);
        }
    }

    // Process all queued destroy commands.
    ProcessCommands();
}

void EntitySystem::ProcessCommands()
{
    // Process entity commands.
    while(!m_commands.empty())
    {
        // Get the command from the queue.
        EntityCommand& command = m_commands.front();

        // Process entity command.
        switch(command.type)
        {
            case EntityCommands::Created:
            {
                // Locate the handle entry.
                int handleIndex = command.handle.identifier - StartingIdentifier;
                HandleEntry& handleEntry = m_handleEntries[handleIndex];
                ASSERT(command.handle == handleEntry.handle);

                // Inform that a  new entity was created
                // since last time commands were processed.
                this->events.entityCreated(handleEntry.handle);

                // Increment the counter of active entities.
                m_entityCount += 1;
            }
            break;

            case EntityCommands::Destroy:
            {
                // Locate the handle entry.
                int handleIndex = command.handle.identifier - StartingIdentifier;
                HandleEntry& handleEntry = m_handleEntries[handleIndex];
                ASSERT(command.handle == handleEntry.handle);

                // Inform about an entity being destroyed
                // since last time commands were processed.
                this->events.entityDestroyed(handleEntry.handle);

                // Decrement the counter of active entities.
                m_entityCount -= 1;

                // Free the entity handle and return it to the pool.
                ASSERT(handleEntry.flags & HandleFlags::Destroy);
                this->FreeHandle(handleIndex, handleEntry);
            }
            break;
        }

        // Remove command from the queue.
        m_commands.pop();
    }
}

void EntitySystem::FreeHandle(int handleIndex, HandleEntry& handleEntry)
{
    // Make sure we got the matching index.
    ASSERT(&m_handleEntries[handleIndex] == &handleEntry);

    // Make sure that flags are correct.
    ASSERT(handleEntry.flags & HandleFlags::Created);

    // Increment the handle version to invalidate it.
    handleEntry.handle.version += 1;

    // Mark the handle as free.
    handleEntry.flags = HandleFlags::Unused;

    // Add the handle entry to the free list queue.
    m_freeHandles.emplace(&handleEntry);
}

bool EntitySystem::IsHandleValid(const EntityHandle& entity) const
{
    // Calculate entity handle entry index.
    int handleIndex = entity.identifier - StartingIdentifier;

    // Make sure handle index is withing the expected range.
    ASSERT(handleIndex >= 0, "Invalid entity handle identifier!");
    ASSERT(handleIndex < (int)m_handleEntries.size(), "Invalid entity handle identifier!");

    // Retrieve the handle entry.
    const HandleEntry& handleEntry = m_handleEntries[handleIndex];

    // Make sure queried handle is valid.
    ASSERT(handleEntry.flags & HandleFlags::Created, "Queried entity handle is not marked as created!");
    
    // Check if handle versions match.
    if(handleEntry.handle.version != entity.version)
        return false;

    return true;
}
