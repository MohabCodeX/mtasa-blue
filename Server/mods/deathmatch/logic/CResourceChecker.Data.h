/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Server/mods/deathmatch/logic/CResourceChecker.Data.h
 *  PURPOSE:     Resource file content checker/validator/upgrader
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

namespace
{
    //
    // Minimum version requirements for functions/events
    //

#if 0 // Activate the counterpart in CResourceChecker::CheckVersionRequirements when you add items to these lists
    struct SVersionItem
    {
        SString functionName;
        SString minMtaVersion;
    };

    SVersionItem clientFunctionInitList[] = {
        // Features added in 1.7.0
        // {"name", "1.7.0-9.bbbbb"},
    };

    SVersionItem serverFunctionInitList[] = {
        // Features added in 1.7.0
        // {"name", "1.7.0-9.bbbbb"},
    };
#endif

    //
    // Deprecated function/events
    //

    struct SDeprecatedItem
    {
        // bRemoved does not mean:
        //     "has this function been removed yet?"
        // bRemoved actually means:
        //     "is not rename?" (you can't rename removed functions)
        bool bRemoved;

        SString strOldName;
        SString strNewName;
        SString strVersion;
    };

    SDeprecatedItem clientDeprecatedList[] = {
        // Client functions
        {false, "getPlayerRotation", "getPedRotation"},
        {false, "canPlayerBeKnockedOffBike", "canPedBeKnockedOffBike"},
        {false, "getPlayerContactElement", "getPedContactElement"},
        {false, "isPlayerInVehicle", "isPedInVehicle"},
        {false, "doesPlayerHaveJetPack", "doesPedHaveJetPack"},
        {false, "isPlayerInWater", "isElementInWater"},
        {false, "isPedInWater", "isElementInWater"},
        {false, "isPlayerOnGround", "isPedOnGround"},
        {false, "getPlayerTask", "getPedTask"},
        {false, "getPlayerSimplestTask", "getPedSimplestTask"},
        {false, "isPlayerDoingTask", "isPedDoingTask"},
        {false, "getPlayerTarget", "getPedTarget"},
        {false, "getPlayerTargetStart", "getPedTargetStart"},
        {false, "getPlayerTargetEnd", "getPedTargetEnd"},
        {false, "getPlayerTargetCollision", "getPedTargetCollision"},
        {false, "getPlayerWeaponSlot", "getPedWeaponSlot"},
        {false, "getPlayerWeapon", "getPedWeapon"},
        {false, "getPlayerAmmoInClip", "getPedAmmoInClip"},
        {false, "getPlayerTotalAmmo", "getPedTotalAmmo"},
        {false, "getPlayerOccupiedVehicle", "getPedOccupiedVehicle"},
        {false, "getPlayerArmor", "getPedArmor"},
        {false, "getPlayerSkin", "getElementModel"},
        {false, "isPlayerChoking", "isPedChoking"},
        {false, "isPlayerDucked", "isPedDucked"},
        {false, "getPlayerStat", "getPedStat"},
        {false, "setPlayerWeaponSlot", "setPedWeaponSlot"},
        {false, "setPlayerSkin", "setElementModel"},
        {false, "setPlayerRotation", "setPedRotation"},
        {false, "setPlayerCanBeKnockedOffBike", "setPedCanBeKnockedOffBike"},
        {false, "setVehicleModel", "setElementModel"},
        {false, "getVehicleModel", "getElementModel"},
        {false, "getPedSkin", "getElementModel"},
        {false, "setPedSkin", "setElementModel"},
        {false, "getObjectRotation", "getElementRotation"},
        {false, "setObjectRotation", "setElementRotation"},
        {false, "getVehicleIDFromName", "getVehicleModelFromName"},
        {false, "getVehicleID", "getElementModel"},
        {false, "getVehicleRotation", "getElementRotation"},
        {false, "getVehicleNameFromID", "getVehicleNameFromModel"},
        {false, "setVehicleRotation", "setElementRotation"},
        {false, "attachElementToElement", "attachElements"},
        {false, "detachElementFromElement", "detachElements"},
        {false, "xmlFindSubNode", "xmlFindChild"},
        {false, "xmlNodeGetSubNodes", "xmlNodeGetChildren"},
        {false, "xmlNodeFindSubNode", "xmlFindChild"},
        {false, "xmlCreateSubNode", "xmlCreateChild"},
        {false, "isPedFrozen", "isElementFrozen"},
        {false, "isVehicleFrozen", "isElementFrozen"},
        {false, "isObjectStatic", "isElementFrozen"},
        {false, "setPedFrozen", "setElementFrozen"},
        {false, "setVehicleFrozen", "setElementFrozen"},
        {false, "setObjectStatic", "setElementFrozen"},
        {false, "isPlayerDead", "isPedDead"},
        {false, "showPlayerHudComponent", "setPlayerHudComponentVisible"},
        {false, "setControlState", "setPedControlState"},
        {false, "getControlState", "getPedControlState"},
        {false, "getVehicleTurnVelocity", "getElementAngularVelocity"},
        {false, "setVehicleTurnVelocity", "setElementAngularVelocity"},
        {false, "getCameraShakeLevel", "getCameraDrunkLevel"},
        {false, "setCameraShakeLevel", "setCameraDrunkLevel"},
        // Edit
        {false, "guiEditSetCaratIndex", "guiEditSetCaretIndex"},
        {false, "guiMemoSetCaratIndex", "guiMemoSetCaretIndex"},
        // XML
        {false, "xmlNodeFindChild", "xmlFindChild"},

        {false, "getComponentPosition", "will return 3 floats instead of a Vector3", "1.5.5-9.11710"},
        {false, "getComponentRotation", "will return 3 floats instead of a Vector3", "1.5.5-9.11710"},

        {false, "getBoundingBox", "will return 6 floats instead of 2 Vector3", "1.5.5-9.13999"},

        // Ped jetpacks
        //{false, "doesPedHaveJetPack", "isPedWearingJetpack"},

        // Base Encoding & Decoding
        {true, "base64Encode", "Please manually change this to encodeString (different syntax). Refer to the wiki for details"},
        {true, "base64Decode", "Please manually change this to decodeString (different syntax). Refer to the wiki for details"},

        {false, "setHelicopterRotorSpeed", "setVehicleRotorSpeed"},
        {false, "getHelicopterRotorSpeed", "getVehicleRotorSpeed"},

        {false, "setPedOnFire", "setElementOnFire"},
        {false, "isPedOnFire", "isElementOnFire"},

        {false, "removeAllGameBuildings", "removeGameWorld"},
        {false, "restoreAllGameBuildings", "restoreGameWorld"},
    };

    SDeprecatedItem serverDeprecatedList[] = {
        // Server events
        {false, "onClientLogin", "onPlayerLogin"},
        {false, "onClientLogout", "onPlayerLogout"},
        {false, "onClientChangeNick", "onPlayerChangeNick"},
        // Server functions
        {false, "getPlayerSkin", "getElementModel"},
        {false, "setPlayerSkin", "setElementModel"},
        {false, "getVehicleModel", "getElementModel"},
        {false, "setVehicleModel", "setElementModel"},
        {false, "getObjectModel", "getElementModel"},
        {false, "setObjectModel", "setElementModel"},
        {false, "getVehicleID", "getElementModel"},
        {false, "getVehicleIDFromName", "getVehicleModelFromName"},
        {false, "getVehicleNameFromID", "getVehicleNameFromModel"},
        {false, "getPlayerWeaponSlot", "getPedWeaponSlot"},
        {false, "getPlayerWeapon", "getPedWeapon"},
        {false, "getPlayerTotalAmmo", "getPedTotalAmmo"},
        {false, "getPlayerAmmoInClip", "getPedAmmoInClip"},
        {false, "getPlayerArmor", "getPedArmor"},
        {false, "getPlayerRotation", "getPedRotation"},
        {false, "isPlayerChoking", "isPedChoking"},
        {false, "isPlayerDead", "isPedDead"},
        {false, "isPlayerDucked", "isPedDucked"},
        {false, "getPlayerStat", "getPedStat"},
        {false, "getPlayerTarget", "getPedTarget"},
        {false, "getPlayerClothes", "getPedClothes"},
        {false, "doesPlayerHaveJetPack", "doesPedHaveJetPack"},
        {false, "isPlayerInWater", "isElementInWater"},
        {false, "isPedInWater", "isElementInWater"},
        {false, "isPlayerOnGround", "isPedOnGround"},
        {false, "getPlayerFightingStyle", "getPedFightingStyle"},
        {false, "getPlayerGravity", "getPedGravity"},
        {false, "getPlayerContactElement", "getPedContactElement"},
        {false, "setPlayerArmor", "setPedArmor"},
        {false, "setPlayerWeaponSlot", "setPedWeaponSlot"},
        {false, "killPlayer", "killPed"},
        {false, "setPlayerRotation", "setPedRotation"},
        {false, "setPlayerStat", "setPedStat"},
        {false, "addPlayerClothes", "addPedClothes"},
        {false, "removePlayerClothes", "removePedClothes"},
        {false, "setPlayerFightingStyle", "setPedFightingStyle"},
        {false, "setPlayerGravity", "setPedGravity"},
        {false, "setPlayerChoking", "setPedChoking"},
        {false, "warpPlayerIntoVehicle", "warpPedIntoVehicle"},
        {false, "removePlayerFromVehicle", "removePedFromVehicle"},
        {false, "attachElementToElement", "attachElements"},
        {false, "detachElementFromElement", "detachElements"},
        {false, "showPlayerHudComponent", "setPlayerHudComponentVisible"},
        {false, "getVehicleTurnVelocity", "getElementAngularVelocity"},
        {false, "setVehicleTurnVelocity", "setElementAngularVelocity"},

        // Server ped jetpack
        {true, "givePlayerJetPack", "Replaced with setPedWearingJetpack. Refer to the wiki for details"},
        {true, "removePlayerJetPack", "Replaced with setPedWearingJetpack. Refer to the wiki for details"},
        {true, "givePedJetPack", "Replaced with setPedWearingJetpack. Refer to the wiki for details"},
        {true, "removePedJetPack", "Replaced with setPedWearingJetpack. Refer to the wiki for details"},
        {false, "doesPedHaveJetPack", "isPedWearingJetpack"},

        // XML
        {false, "xmlNodeGetSubNodes", "xmlNodeGetChildren"},
        {false, "xmlCreateSubNode", "xmlCreateChild"},
        {false, "xmlFindSubNode", "xmlFindChild"},
        // Frozen
        {false, "isPedFrozen", "isElementFrozen"},
        {false, "isVehicleFrozen", "isElementFrozen"},
        {false, "setPedFrozen", "setElementFrozen"},
        {false, "setVehicleFrozen", "setElementFrozen"},
        // Player
        {false, "getPlayerOccupiedVehicle", "getPedOccupiedVehicle"},
        {false, "getPlayerOccupiedVehicleSeat", "getPedOccupiedVehicleSeat"},
        {false, "isPlayerInVehicle", "isPedInVehicle"},
        {false, "getPlayerFromNick", "getPlayerFromName"},
      
        // Client
        {false, "getClientName", "getPlayerName"},
        {false, "getClientIP", "getPlayerIP"},
        {false, "getClientAccount", "getPlayerAccount"},
        {true, "getAccountClient", "getAccountPlayer"},
        {false, "setClientName", "setPlayerName"},
        // Utility
        {true, "randFloat", "math.random"},
        {true, "randInt", "math.random"},
        // Weapon
        {false, "giveWeaponAmmo", "giveWeapon"},
        {false, "takeWeaponAmmo", "takeWeapon"},

        // Admin
        {true, "banIP", "Please manually update this.  Refer to the wiki for details"},
        {true, "banSerial", "Please manually update this.  Refer to the wiki for details"},
        {true, "unbanIP", "Please manually update this.  Refer to the wiki for details"},
        {true, "unbanSerial", "Please manually update this.  Refer to the wiki for details"},
        {true, "getBansXML", "Please manually update this.  Refer to the wiki for details"},
        {true, "canPlayerUseFunction", "Please manually update this.  Refer to the wiki for details"},

        // Old Discord implementation (see #2499)
        {true, "setPlayerDiscordJoinParams", "See GitHub PR #2499 for more details"},

        // Base Encoding & Decoding
        {true, "base64Encode", "Please manually change this to encodeString (different syntax). Refer to the wiki for details"},
        {true, "base64Decode", "Please manually change this to decodeString (different syntax). Refer to the wiki for details"},
    
        // Ped
        {false, "setPedOnFire", "setElementOnFire"},
        {false, "isPedOnFire", "isElementOnFire"}
    };
}            // namespace
