#include "Precompiled.hpp"
#include "System/ResourceManager.hpp"
using namespace System;

ResourceManager::ResourceManager() :
    m_initialized(false)
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager::ResourceManager(ResourceManager&& other) :
    ResourceManager()
{
    // Call the move assignment.
    *this = std::move(other);
}

ResourceManager& ResourceManager::operator=(ResourceManager&& other)
{
    // Swap class members.
    std::swap(m_pools, other.m_pools);
    std::swap(m_initialized, other.m_initialized);

    return *this;
}

bool ResourceManager::Initialize()
{
    LOG() << "Initializing resource manager..." << LOG_INDENT();

    // Check if resource manager has already been initialized.
    ASSERT(!m_initialized, "Resource manager has already been initialized!");

    // Success!
    return m_initialized = true;
}

void ResourceManager::ReleaseUnused()
{
    ASSERT(m_initialized, "Resource manager has not been initialized!");

    // Release all unused resources.
    for(auto& pair : m_pools)
    {
        ASSERT(pair.second != nullptr, "Resource pool is null!");

        // Release unused resources from each pool.
        auto& pool = pair.second;
        pool->ReleaseUnused();
    }
}
