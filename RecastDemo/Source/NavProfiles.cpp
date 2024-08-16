#include "NavProfiles.h"

#include <vector>

using std::vector;

vector<NavGameProfile> NavGameProfiles;

unsigned int CurrentGameProfileIndex = 0;

string* CurrentlyModifiedString = nullptr;

int AreaFlagModifier = -1;
int ConnAreaModifier = -1;
int ConnFlagModifier = -1;
int ProfileMeshModifier = -1;
int ProfileModifier = 0;

unsigned int* FlagColorModifier = nullptr;


unsigned int GetCurrentGameProfileIndex()
{
	return CurrentGameProfileIndex;
}

void SetCurrentGameProfileIndex(unsigned int NewIndex)
{
	if (NewIndex >= NavGameProfiles.size())
	{
		CurrentGameProfileIndex = 0;
	}
	else
	{
		CurrentGameProfileIndex = NewIndex;
	}
}

inline unsigned int duRGBA2(int r, int g, int b, int a)
{
	return ((unsigned int)r) | ((unsigned int)g << 8) | ((unsigned int)b << 16) | ((unsigned int)a << 24);
}

NavGameProfile* CreateNewGameProfile()
{
	NavGameProfile NewProfile;
	NewProfile.GameName = "Game";
	
	NavAreaDefinition DefaultNullArea;
	DefaultNullArea.NavAreaIndex = 0;
	DefaultNullArea.AreaName = "Unwalkable";
	DefaultNullArea.TechnicalName = "NAV_AREA_UNWALKABLE";
	DefaultNullArea.AreaId = 60;
	DefaultNullArea.FlagIndex = 0;
	DefaultNullArea.bCustom = false;
	DefaultNullArea.R = 10.0f;
	DefaultNullArea.G = 10.0f;
	DefaultNullArea.B = 10.0f;
	DefaultNullArea.DebugColor = duRGBA2(10, 10, 10, 255);

	NavAreaDefinition DefaultWalkArea;
	DefaultWalkArea.NavAreaIndex = 1;
	DefaultWalkArea.AreaName = "Walk";
	DefaultWalkArea.TechnicalName = "NAV_AREA_WALK";
	DefaultWalkArea.AreaId = 1;
	DefaultWalkArea.FlagIndex = 1;
	DefaultWalkArea.bCustom = false;
	DefaultWalkArea.R = 0.0f;
	DefaultWalkArea.G = 192.0f;
	DefaultWalkArea.B = 255.0f;
	DefaultWalkArea.DebugColor = duRGBA2(0, 192, 255, 255);

	NavAreaDefinition DefaultCrouchArea;
	DefaultCrouchArea.NavAreaIndex = 2;
	DefaultCrouchArea.AreaName = "Crouch";
	DefaultCrouchArea.TechnicalName = "NAV_AREA_CROUCH";
	DefaultCrouchArea.AreaId = 2;
	DefaultCrouchArea.FlagIndex = 2;
	DefaultCrouchArea.bCustom = false;
	DefaultCrouchArea.R = 9.0f;
	DefaultCrouchArea.G = 130.0f;
	DefaultCrouchArea.B = 150.0f;
	DefaultCrouchArea.DebugColor = duRGBA2(9, 130, 150, 255);

	NavAreaDefinition DefaultHazardArea;
	DefaultHazardArea.NavAreaIndex = 3;
	DefaultHazardArea.AreaName = "Hazard";
	DefaultHazardArea.TechnicalName = "NAV_AREA_HAZARD";
	DefaultHazardArea.AreaId = 3;
	DefaultHazardArea.FlagIndex = 1;
	DefaultHazardArea.bCustom = false;
	DefaultHazardArea.R = 192.0f;
	DefaultHazardArea.G = 32.0f;
	DefaultHazardArea.B = 32.0f;
	DefaultHazardArea.DebugColor = duRGBA2(192, 32, 32, 255);

	NewProfile.AreaDefinitions.push_back(DefaultNullArea);
	NewProfile.AreaDefinitions.push_back(DefaultWalkArea);
	NewProfile.AreaDefinitions.push_back(DefaultCrouchArea);
	NewProfile.AreaDefinitions.push_back(DefaultHazardArea);

	NavFlagDefinition DisabledFlagDef;
	DisabledFlagDef.NavFlagIndex = 0;
	DisabledFlagDef.FlagName = "Disabled";
	DisabledFlagDef.TechnicalName = "NAV_FLAG_DISABLED";
	DisabledFlagDef.FlagId = 1 << 15;
	DisabledFlagDef.bCustom = false;
	DisabledFlagDef.R = 8.0f;
	DisabledFlagDef.G = 8.0f;
	DisabledFlagDef.B = 8.0f;
	DisabledFlagDef.DebugColor = duRGBA2(8, 8, 8, 255);

	NavFlagDefinition WalkFlagDef;
	WalkFlagDef.NavFlagIndex = 1;
	WalkFlagDef.FlagName = "Walk";
	WalkFlagDef.TechnicalName = "NAV_FLAG_WALK";
	WalkFlagDef.FlagId = 1 << 0;
	WalkFlagDef.bCustom = false;
	WalkFlagDef.R = 0.0f;
	WalkFlagDef.G = 192.0f;
	WalkFlagDef.B = 255.0f;
	WalkFlagDef.DebugColor = duRGBA2(0, 192, 255, 255);

	NavFlagDefinition CrouchFlagDef;
	CrouchFlagDef.NavFlagIndex = 2;
	CrouchFlagDef.FlagName = "Crouch";
	CrouchFlagDef.TechnicalName = "NAV_FLAG_CROUCH";
	CrouchFlagDef.FlagId = 1 << 1;
	CrouchFlagDef.bCustom = false;
	CrouchFlagDef.R = 9.0f;
	CrouchFlagDef.G = 130.0f;
	CrouchFlagDef.B = 150.0f;
	CrouchFlagDef.DebugColor = duRGBA2(9, 130, 150, 255);

	NavFlagDefinition JumpFlagDef;
	JumpFlagDef.NavFlagIndex = 3;
	JumpFlagDef.FlagName = "Jump";
	JumpFlagDef.TechnicalName = "NAV_FLAG_JUMP";
	JumpFlagDef.FlagId = 1 << 2;
	JumpFlagDef.bCustom = false;
	JumpFlagDef.R = 200.0f;
	JumpFlagDef.G = 200.0f;
	JumpFlagDef.B = 0.0f;
	JumpFlagDef.DebugColor = duRGBA2(200, 200, 0, 255);

	NavFlagDefinition LadderFlagDef;
	LadderFlagDef.NavFlagIndex = 4;
	LadderFlagDef.FlagName = "Ladder";
	LadderFlagDef.TechnicalName = "NAV_FLAG_LADDER";
	LadderFlagDef.FlagId = 1 << 3;
	LadderFlagDef.bCustom = false;
	LadderFlagDef.R = 64.0f;
	LadderFlagDef.G = 64.0f;
	LadderFlagDef.B = 255.0f;
	LadderFlagDef.DebugColor = duRGBA2(64, 64, 255, 255);

	NavFlagDefinition FallFlagDef;
	FallFlagDef.NavFlagIndex = 5;
	FallFlagDef.FlagName = "Fall";
	FallFlagDef.TechnicalName = "NAV_FLAG_FALL";
	FallFlagDef.FlagId = 1 << 4;
	FallFlagDef.bCustom = false;
	FallFlagDef.R = 255.0f;
	FallFlagDef.G = 64.0f;
	FallFlagDef.B = 64.0f;
	FallFlagDef.DebugColor = duRGBA2(255, 64, 64, 255);

	NewProfile.FlagDefinitions.push_back(DisabledFlagDef);
	NewProfile.FlagDefinitions.push_back(WalkFlagDef);
	NewProfile.FlagDefinitions.push_back(CrouchFlagDef);
	NewProfile.FlagDefinitions.push_back(JumpFlagDef);
	NewProfile.FlagDefinitions.push_back(LadderFlagDef);
	NewProfile.FlagDefinitions.push_back(FallFlagDef);

	NavOffMeshConnectionDefinition WalkMoveDef;
	WalkMoveDef.ConnIndex = 0;
	WalkMoveDef.ConnName = "Walk";
	WalkMoveDef.AreaIndex = 1;
	WalkMoveDef.FlagIndex = 1;
	WalkMoveDef.bCustom = false;

	NavOffMeshConnectionDefinition CrouchMoveDef;
	CrouchMoveDef.ConnIndex = 1;
	CrouchMoveDef.ConnName = "Crouch";
	CrouchMoveDef.AreaIndex = 2;
	CrouchMoveDef.FlagIndex = 2;
	CrouchMoveDef.bCustom = false;

	NavOffMeshConnectionDefinition JumpMoveDef;
	JumpMoveDef.ConnIndex = 2;
	JumpMoveDef.ConnName = "Jump";
	JumpMoveDef.AreaIndex = 1;
	JumpMoveDef.FlagIndex = 3;
	JumpMoveDef.bCustom = false;

	NavOffMeshConnectionDefinition LadderMoveDef;
	LadderMoveDef.ConnIndex = 3;
	LadderMoveDef.ConnName = "Ladder";
	LadderMoveDef.AreaIndex = 1;
	LadderMoveDef.FlagIndex = 4;
	LadderMoveDef.bCustom = false;

	NavOffMeshConnectionDefinition FallMoveDef;
	FallMoveDef.ConnIndex = 4;
	FallMoveDef.ConnName = "Fall";
	FallMoveDef.AreaIndex = 1;
	FallMoveDef.FlagIndex = 5;
	FallMoveDef.bCustom = false;

	NewProfile.ConnectionDefinitions.push_back(WalkMoveDef);
	NewProfile.ConnectionDefinitions.push_back(CrouchMoveDef);
	NewProfile.ConnectionDefinitions.push_back(JumpMoveDef);
	NewProfile.ConnectionDefinitions.push_back(LadderMoveDef);
	NewProfile.ConnectionDefinitions.push_back(FallMoveDef);
		
	NavMeshDefinition DefaultMesh;
	NewProfile.MeshDefinitions.push_back(DefaultMesh);

	NavAgentProfile DefaultProfile;
	DefaultProfile.ProfileName = "Base Player";
	DefaultProfile.MovementFlags |= WalkFlagDef.FlagId;
	DefaultProfile.MovementFlags |= CrouchFlagDef.FlagId;
	DefaultProfile.MovementFlags |= JumpFlagDef.FlagId;
	DefaultProfile.MovementFlags |= FallFlagDef.FlagId;
	DefaultProfile.MovementFlags |= LadderFlagDef.FlagId;
	
	for (int i = 0; i < 32; i++)
	{
		DefaultProfile.AreaCosts[i] = 1.0f;
	}

	DefaultProfile.AreaCosts[2] = 2.0f;
	DefaultProfile.AreaCosts[3] = 10.0f;

	NewProfile.ProfileDefinitions.push_back(DefaultProfile);

	NavGameProfiles.push_back(NewProfile);

	return &(*(prev(NavGameProfiles.end())));
}

NavAreaDefinition* CreateNewAreaDefinition()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	int NewAreaIndex = GetIndexForNewArea();

	NavAreaDefinition NewAreaDef;
	NewAreaDef.AreaName = "New Area";
	NewAreaDef.TechnicalName = "NAV_AREA_CUSTOM";
	NewAreaDef.AreaId = NewAreaIndex;
	NewAreaDef.NavAreaIndex = NewAreaIndex;
	NewAreaDef.bCustom = true;

	CurrentProfile->AreaDefinitions.push_back(NewAreaDef);

	return &(*(prev(CurrentProfile->AreaDefinitions.end())));
}

int GetIndexForNewFlag()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return 0; }

	int Result = 0;
	bool bFound = false;

	while (!bFound)
	{
		bFound = true;
		for (auto it = CurrentProfile->FlagDefinitions.begin(); it != CurrentProfile->FlagDefinitions.end(); it++)
		{
			if (it->NavFlagIndex == Result)
			{
				bFound = false;
				Result++;
				break;
			}
		}
	}

	return Result;
}

int GetIndexForNewConnectionType()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return 0; }

	int Result = 0;
	bool bFound = false;

	while (!bFound)
	{
		bFound = true;
		for (auto it = CurrentProfile->ConnectionDefinitions.begin(); it != CurrentProfile->ConnectionDefinitions.end(); it++)
		{
			if (it->ConnIndex == Result)
			{
				bFound = false;
				Result++;
				break;
			}
		}
	}

	return Result;
}


int GetIndexForNewArea()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return 0; }

	int Result = 0;
	bool bFound = false;

	while (!bFound)
	{
		bFound = true;
		for (auto it = CurrentProfile->AreaDefinitions.begin(); it != CurrentProfile->AreaDefinitions.end(); it++)
		{
			if (it->NavAreaIndex == Result)
			{
				bFound = false;
				Result++;
				break;
			}
		}
	}

	return Result;
}

NavFlagDefinition* GetFlagAtIndex(unsigned int Index)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	if (Index >= CurrentProfile->FlagDefinitions.size()) { return nullptr; }

	return &CurrentProfile->FlagDefinitions[Index];
}

NavAreaDefinition* GetAreaAtIndex(unsigned int Index)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	if (Index < 0 || Index >= CurrentProfile->AreaDefinitions.size()) { return nullptr; }

	return &CurrentProfile->AreaDefinitions[Index];
}

NavAgentProfile* GetAgentProfileAtIndex(unsigned int Index)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	if (Index < 0 || Index >= CurrentProfile->ProfileDefinitions.size()) { return nullptr; }

	return &CurrentProfile->ProfileDefinitions[Index];
}

NavMeshDefinition* GetMeshAtIndex(unsigned int Index)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	if (Index < 0 || Index >= CurrentProfile->MeshDefinitions.size()) { return nullptr; }

	return &CurrentProfile->MeshDefinitions[Index];
}

int GetNumNavMeshes()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return 0; }

	return CurrentProfile->MeshDefinitions.size();
}

NavOffMeshConnectionDefinition* GetConnectionAtIndex(unsigned int Index)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	if (Index < 0 || Index >= CurrentProfile->ConnectionDefinitions.size()) { return nullptr; }

	return &CurrentProfile->ConnectionDefinitions[Index];
}

vector<NavAreaDefinition> GetAllNavAreaDefinitions()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return vector<NavAreaDefinition>(); }

	return CurrentProfile->AreaDefinitions;
}

vector<NavFlagDefinition> GetAllNavFlagDefinitions()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return vector<NavFlagDefinition>(); }

	return CurrentProfile->FlagDefinitions;
}

vector<NavOffMeshConnectionDefinition> GetAllConnectionDefinitions()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return vector<NavOffMeshConnectionDefinition>(); }

	return CurrentProfile->ConnectionDefinitions;
}

vector<NavMeshDefinition> GetAllMeshDefinitions()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return vector<NavMeshDefinition>(); }

	return CurrentProfile->MeshDefinitions;
}

vector<NavAgentProfile> GetAllAgentProfileDefinitions()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return vector<NavAgentProfile>(); }

	return CurrentProfile->ProfileDefinitions;
}

NavFlagDefinition* CreateNewMovementFlag()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	int NewFlagIndex = GetIndexForNewFlag();

	NavFlagDefinition NewFlagDef;
	NewFlagDef.NavFlagIndex = NewFlagIndex;
	NewFlagDef.FlagName = "New Flag";
	NewFlagDef.TechnicalName = "NAV_FLAG_CUSTOM";
	NewFlagDef.FlagId = 1 << NewFlagIndex;
	NewFlagDef.bCustom = true;

	CurrentProfile->FlagDefinitions.push_back(NewFlagDef);

	return &(*(prev(CurrentProfile->FlagDefinitions.end())));
}

bool CanRemoveMovementArea(unsigned int RemoveIndex)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return false; }

	if (RemoveIndex >= CurrentProfile->AreaDefinitions.size()) { return false; }

	return CurrentProfile->AreaDefinitions[RemoveIndex].bCustom;
}

bool CanRemoveMovementFlag(unsigned int RemoveIndex)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return false; }

	if (RemoveIndex >= CurrentProfile->FlagDefinitions.size()) { return false; }

	return CurrentProfile->FlagDefinitions[RemoveIndex].bCustom;
}

bool CanRemoveConnectionType(unsigned int RemoveIndex)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return false; }

	if (RemoveIndex >= CurrentProfile->ConnectionDefinitions.size()) { return false; }

	return CurrentProfile->ConnectionDefinitions[RemoveIndex].bCustom;
}

void RemoveMovementArea(unsigned int RemoveIndex)
{
	if (!CanRemoveMovementArea(RemoveIndex)) { return; }

	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return; }

	CurrentProfile->AreaDefinitions.erase((CurrentProfile->AreaDefinitions.begin() + RemoveIndex));
}

void RemoveMovementFlag(unsigned int RemoveIndex)
{
	if (!CanRemoveMovementFlag(RemoveIndex)) { return; }

	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return; }

	CurrentProfile->FlagDefinitions.erase((CurrentProfile->FlagDefinitions.begin() + RemoveIndex));
}

void RemoveAgentProfile(unsigned int RemoveIndex)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile || CurrentProfile->ProfileDefinitions.size() <= 1 || RemoveIndex >= CurrentProfile->ProfileDefinitions.size()) { return; }

	CurrentProfile->ProfileDefinitions.erase((CurrentProfile->ProfileDefinitions.begin() + RemoveIndex));
}

void RemoveMeshDefinition(unsigned int RemoveIndex)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile || CurrentProfile->MeshDefinitions.size() <= 1 || RemoveIndex >= CurrentProfile->MeshDefinitions.size()) { return; }

	CurrentProfile->MeshDefinitions.erase((CurrentProfile->MeshDefinitions.begin() + RemoveIndex));
}

void RemoveConnectionType(unsigned int RemoveIndex)
{
	if (!CanRemoveConnectionType(RemoveIndex)) { return; }

	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return; }

	CurrentProfile->ConnectionDefinitions.erase((CurrentProfile->ConnectionDefinitions.begin() + RemoveIndex));
}

NavMeshDefinition* CreateNewNavMesh()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	NavMeshDefinition NewMeshDef;

	CurrentProfile->MeshDefinitions.push_back(NewMeshDef);

	return &(*(prev(CurrentProfile->MeshDefinitions.end())));
}

NavAgentProfile* CreateNewAgentProfile()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	NavAgentProfile NewAgentProfile;

	vector<NavFlagDefinition> AllFlags = GetAllNavFlagDefinitions();

	for (auto it = AllFlags.begin(); it != AllFlags.end(); it++)
	{
		if (!it->bCustom)
		{
			NewAgentProfile.MovementFlags |= it->FlagId;
		}
	}

	for (int i = 0; i < 32; i++)
	{
		NewAgentProfile.AreaCosts[i] = 1.0f;
	}

	CurrentProfile->ProfileDefinitions.push_back(NewAgentProfile);

	return &(*(prev(CurrentProfile->ProfileDefinitions.end())));
}

NavOffMeshConnectionDefinition* CreateConnectionType()
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	int NewConnectionIndex = GetIndexForNewConnectionType();

	NavOffMeshConnectionDefinition NewMoveDef;
	NewMoveDef.ConnIndex = NewConnectionIndex;
	NewMoveDef.ConnName = "New Connection Type";
	NewMoveDef.AreaIndex = 0;
	NewMoveDef.FlagIndex = 0;
	NewMoveDef.bCustom = true;

	CurrentProfile->ConnectionDefinitions.push_back(NewMoveDef);

	return &(*(prev(CurrentProfile->ConnectionDefinitions.end())));
}

NavGameProfile* GetCurrentGameProfile()
{
	if (NavGameProfiles.size() == 0)
	{
		CurrentGameProfileIndex = 0;
		return nullptr;
	}

	if (CurrentGameProfileIndex > NavGameProfiles.size() - 1)
	{
		CurrentGameProfileIndex = NavGameProfiles.size() - 1;
	}

	return &(*(NavGameProfiles.begin() + CurrentGameProfileIndex));
}

vector<NavGameProfile> GetAllGameProfiles()
{
	return NavGameProfiles;
}

void DeleteGameProfile(unsigned int ProfileIndex)
{
	if (ProfileIndex >= NavGameProfiles.size()) { return; }

	NavGameProfiles.erase(NavGameProfiles.begin() + ProfileIndex);

	if (CurrentGameProfileIndex == ProfileIndex)
	{
		if (CurrentGameProfileIndex > 0)
		{
			CurrentGameProfileIndex--;
		}
	}
}

string* GetCurrentlyModifiedString()
{
	return CurrentlyModifiedString;
}

void SetCurrentlyModifiedString(string* NewModifiedString)
{
	CurrentlyModifiedString = NewModifiedString;
}

void EndModifyString()
{
	CurrentlyModifiedString = nullptr;
}

void SetProfileModifier(int NewIndex)
{
	ResetProfileMenus();
	ProfileModifier = NewIndex;
}

int GetProfileModifier()
{
	return ProfileModifier;
}

void SetAreaFlagModifier(int NewIndex)
{
	ResetProfileMenus();
	AreaFlagModifier = NewIndex;
}

void SetConnAreaModifier(int NewIndex)
{
	ResetProfileMenus();
	ConnAreaModifier = NewIndex;
}

void SetConnFlagModifier(int NewIndex)
{
	ResetProfileMenus();
	ConnFlagModifier = NewIndex;
}

void SetProfileMeshModifier(int NewIndex)
{
	ResetProfileMenus();
	ProfileMeshModifier = NewIndex;
}

bool GetShowFlagList()
{
	return AreaFlagModifier > -1;
}

bool GetShowConnAreaList()
{
	return ConnAreaModifier > -1;
}

int GetAreaFlagModifier()
{
	return AreaFlagModifier;
}

int GetConnAreaModifier()
{
	return ConnAreaModifier;
}

int GetConnFlagModifier()
{
	return ConnFlagModifier;
}

int GetProfileMeshModifier()
{
	return ProfileMeshModifier;
}

unsigned int* GetFlagColorModifier()
{
	return FlagColorModifier;
}

void ResetProfileMenus()
{
	EndModifyString();
	AreaFlagModifier = -1;
	ConnAreaModifier = -1;
	ConnFlagModifier = -1;

	FlagColorModifier = nullptr;
}

NavAreaDefinition* GetNavAreaById(unsigned short Id)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	for (auto it = CurrentProfile->AreaDefinitions.begin(); it != CurrentProfile->AreaDefinitions.end(); it++)
	{
		if (it->AreaId == Id)
		{
			return &(*it);
		}
	}

	return nullptr;
}

NavFlagDefinition* GetFlagByFlagId(unsigned int Id)
{
	NavGameProfile* CurrentProfile = GetCurrentGameProfile();

	if (!CurrentProfile) { return nullptr; }

	for (auto it = CurrentProfile->FlagDefinitions.begin(); it != CurrentProfile->FlagDefinitions.end(); it++)
	{
		if (it->FlagId == Id)
		{
			return &(*it);
		}
	}

	return nullptr;
}

void SetFlagColorModifier(unsigned int* NewColorIndex)
{
	FlagColorModifier = NewColorIndex;
}

void OutputProfileConfig(NavGameProfile* Profile)
{
	if (!Profile) { return; }

	FILE* fp = fopen("Profiles/Profiles.yaml", "w+");

	if (!fp) { return; }

	fprintf(fp, "Hello");

	fflush(fp);
	fclose(fp);
}