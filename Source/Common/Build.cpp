#include "Precompiled.hpp"
#include "Common/Build.hpp"
#include "Common/Utility.hpp"
#include "Config/BuildInfo.hpp"

namespace
{
    std::string WorkingDir;
    std::string EngineDir;
}

void Build::Initialize()
{
    // Retrieve directory path.
    WorkingDir = Utility::GetTextFileContent("WorkingDir.txt");
    EngineDir = Utility::GetTextFileContent("EngineDir.txt");

    // Print retrieved build info.
    PrintInfo();
}

void Build::PrintInfo()
{
    LOG_INFO() << "Printing build info..." << LOG_INDENT();
    LOG_INFO() << "Working directory: \"" << (Build::GetWorkingDir().empty() ? "default" : Build::GetWorkingDir()) << "\"";

    LOG_INFO() << "Engine repository: "
        << Build::GetEngineChangeNumber() << "-"
        << Build::GetEngineChangeHash()   << "-"
        << Build::GetEngineBranchName()   << " ("
        << Build::GetEngineChangeDate()   << ")";

    LOG_INFO() << "Project repository: "
        << Build::GetProjectChangeNumber() << "-"
        << Build::GetProjectChangeHash()   << "-"
        << Build::GetProjectBranchName()   << " ("
        << Build::GetProjectChangeDate()   << ")";
}

std::string Build::GetWorkingDir()
{
    return WorkingDir;
}

std::string Build::GetEngineDir()
{
    return EngineDir;
}

std::string Build::GetEngineChangeNumber()
{
    return BuildInfo::Engine::ChangeNumber;
}

std::string Build::GetEngineChangeHash()
{
    return BuildInfo::Engine::CommitHash;
}

std::string Build::GetEngineChangeDate()
{
    return BuildInfo::Engine::CommitDate;
}

std::string Build::GetEngineBranchName()
{
    return BuildInfo::Engine::BranchName;
}

std::string Build::GetProjectChangeNumber()
{
    return BuildInfo::Project::ChangeNumber;
}

std::string Build::GetProjectChangeHash()
{
    return BuildInfo::Project::CommitHash;
}

std::string Build::GetProjectChangeDate()
{
    return BuildInfo::Project::CommitDate;
}

std::string Build::GetProjectBranchName()
{
    return BuildInfo::Project::BranchName;
}
