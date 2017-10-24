#include "gameFallout4.h"

#include "fallout4dataarchives.h"
#include "fallout4scriptextender.h"
#include "fallout4savegameinfo.h"
#include "fallout4gameplugins.h"
#include "fallout4unmanagedmods.h"

#include <pluginsetting.h>
#include "iplugingame.h"
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include "versioninfo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

using namespace MOBase;

GameFallout4::GameFallout4()
{
}

bool GameFallout4::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new Fallout4ScriptExtender(this));
  registerFeature<DataArchives>(new Fallout4DataArchives());
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "Fallout4.ini"));
  registerFeature<SaveGameInfo>(new Fallout4SaveGameInfo(this));
  registerFeature<GamePlugins>(new Fallout4GamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new Fallout4UnmangedMods(this));

  return true;
}

QString GameFallout4::gameName() const
{
  return "Fallout 4";
}

QList<ExecutableInfo> GameFallout4::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("F4SE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      << ExecutableInfo("Fallout 4", findInGameFolder(binaryName()))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
      << ExecutableInfo("LOOT", getLootPath())
         ;
}

QString GameFallout4::name() const
{
  return "Fallout4 Support Plugin";
}

QString GameFallout4::author() const
{
  return "Tannin";
}

QString GameFallout4::description() const
{
  return tr("Adds support for the game Fallout 4.\n"
            "Splash by %1").arg("nekoyoubi");
}

MOBase::VersionInfo GameFallout4::version() const
{
  return VersionInfo(0, 3, 0, VersionInfo::RELEASE_BETA);
}

bool GameFallout4::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFallout4::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout4::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Fallout4", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Fallout4", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/fallout4.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "fallout4_default.ini", "fallout4.ini");
    } else {
      copyToProfile(myGamesPath(), path, "fallout4.ini");
    }

    copyToProfile(myGamesPath(), path, "fallout4prefs.ini");
	copyToProfile(myGamesPath(), path, "fallout4custom.ini");
  }
}

QString GameFallout4::savegameExtension() const
{
  return "fos";
}

QString GameFallout4::steamAPPId() const
{
  return "377160";
}

QStringList GameFallout4::primaryPlugins() const {
  return {"fallout4.esm", "dlcrobot.esm", "dlcworkshop01.esm", "dlccoast.esm", "dlcworkshop02.esm", "dlcworkshop03.esm", "dlcnukaworld.esm",
          "ccbgsfo4001-pipboy(black).esl", "ccbgsfo4002-pipboy(blue).esl", "ccbgsfo4003-pipboy(camo01).esl",
          "ccbgsfo4004-pipboy(camo02).esl", "ccbgsfo4006-pipboy(chrome).esl", "ccbgsfo4012-pipboy(red).esl",
          "ccbgsfo4014-pipboy(white).esl", "ccbgsfo4016-prey.esl", "ccbgsfo4017-mauler.esl", "ccbgsfo4018-gaussrifleprototype.esl",
          "ccbgsfo4019-chinesestealtharmor.esl", "ccbgsfo4020-powerarmorskin(black).esl", "ccbgsfo4022-powerarmorskin(camo01).esl",
          "ccbgsfo4023-powerarmorskin(camo02).esl", "ccbgsfo4025-powerarmorskin(chrome).esl", "ccbgsfo4038-horsearmor.esl",
          "ccbgsfo4039-tunnelsnakes.esl", "ccbgsfo4041-doommarinearmor.esl", "ccbgsfo4042-bfg.esl",
          "ccbgsfo4043-doomchainsaw.esl", "ccbgsfo4044-hellfirepowerarmor.esl", "ccfsvfo4001-modularmilitarybackpack.esl",
          "ccfsvfo4002-midcenturymodern.esl", "ccfrsfo4001-handmadeshotgun.esl", "cceejfo4001-decorationpack.esl"};
}

QStringList GameFallout4::gameVariants() const
{
  return { "Regular" };
}

QString GameFallout4::gameShortName() const
{
  return "Fallout4";
}

QString GameFallout4::gameNexusName() const
{
  return "Fallout4";
}

QStringList GameFallout4::iniFiles() const
{
    return { "fallout4.ini", "fallout4prefs.ini", "fallout4custom.ini" };
}

QStringList GameFallout4::DLCPlugins() const
{
  return {"dlcrobot.esm", "dlcworkshop01.esm", "dlccoast.esm", "dlcworkshop02.esm", "dlcworkshop03.esm", "dlcnukaworld.esm",
          "ccbgsfo4001-pipboy(black).esl", "ccbgsfo4002-pipboy(blue).esl", "ccbgsfo4003-pipboy(camo01).esl",
          "ccbgsfo4004-pipboy(camo02).esl", "ccbgsfo4006-pipboy(chrome).esl", "ccbgsfo4012-pipboy(red).esl",
          "ccbgsfo4014-pipboy(white).esl", "ccbgsfo4016-prey.esl", "ccbgsfo4017-mauler.esl", "ccbgsfo4018-gaussrifleprototype.esl",
          "ccbgsfo4019-chinesestealtharmor.esl", "ccbgsfo4020-powerarmorskin(black).esl", "ccbgsfo4022-powerarmorskin(camo01).esl",
          "ccbgsfo4023-powerarmorskin(camo02).esl", "ccbgsfo4025-powerarmorskin(chrome).esl", "ccbgsfo4038-horsearmor.esl",
          "ccbgsfo4039-tunnelsnakes.esl", "ccbgsfo4041-doommarinearmor.esl", "ccbgsfo4042-bfg.esl",
          "ccbgsfo4043-doomchainsaw.esl", "ccbgsfo4044-hellfirepowerarmor.esl", "ccfsvfo4001-modularmilitarybackpack.esl",
          "ccfsvfo4002-midcenturymodern.esl", "ccfrsfo4001-handmadeshotgun.esl", "cceejfo4001-decorationpack.esl"};
}

IPluginGame::LoadOrderMechanism GameFallout4::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameFallout4::nexusModOrganizerID() const
{
  return 0; //...
}

int GameFallout4::nexusGameID() const
{
  return 1151;
}
