#ifndef GAMEGAMEBRYO_H
#define GAMEGAMEBRYO_H

#include "iplugingame.h"

class ScriptExtender;
class DataArchives;
class SaveGameInfo;
class BSAInvalidation;
class LocalSavegames;

#include <QString>
#include <iplugingame.h>
#include <ipluginfilemapper.h>
#include <memory>
#include <ShlObj.h>


class GameGamebryo : public MOBase::IPluginGame,
                     public MOBase::IPluginFileMapper
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginGame MOBase::IPluginFileMapper)

public:

  GameGamebryo();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  //initializeProfile
  //savegameExtension
  virtual bool isInstalled() const override;
  virtual QIcon gameIcon() const override;
  virtual QDir gameDirectory() const override;
  virtual QDir dataDirectory() const override;
  virtual void setGamePath(const QString &path) override;
  virtual QDir documentsDirectory() const override;
  virtual QDir savesDirectory() const override;
  //executables
  //steamAPPId
  //getPrimaryPlugins
  virtual QStringList gameVariants() const override;
  virtual void setGameVariant(const QString &variant) override;
  virtual QString binaryName() const override;
  //getGameShortName
  //getIniFiles
  //getDLCPlugins
  virtual LoadOrderMechanism loadOrderMechanism() const override;
  //getNexusModOrganizerID
  //getNexusGameID
  virtual bool looksValid(QDir const &) const override;
  virtual QString gameVersion() const override;

public: // IPluginFileMapper interface

  virtual MappingType mappings() const;

protected:

  std::unique_ptr<BYTE[]> getRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD flags, LPDWORD type = nullptr) const;
  QString findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value) const;
  QFileInfo findInGameFolder(const QString &relativePath) const;
  QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const;
  QString getSpecialPath(const QString &name) const;
  QString localAppFolder() const;
  QString myGamesPath() const;
  //Arguably this shouldn't really be here but every gamebryo program seems to use it
  QString getLootPath() const;
  QString selectedVariant() const;
  virtual QString getLauncherName() const;
  friend class GamebryoScriptExtender;
  QString getVersion(QString const &program) const;

protected:

  std::map<std::type_index, boost::any> featureList() const;

  //These should be implemented by anything that uses gamebro (I think)
  //(and if they don't, it'll be a null pointer and won't look implemented,
  //so that's fine too).
  std::shared_ptr<ScriptExtender> m_ScriptExtender { nullptr };
  std::shared_ptr<DataArchives> m_DataArchives { nullptr };
  std::shared_ptr<BSAInvalidation> m_BSAInvalidation { nullptr };
  std::shared_ptr<LocalSavegames> m_LocalSavegames { nullptr };
  std::shared_ptr<SaveGameInfo> m_SaveGameInfo { nullptr };

private:

  QString determineMyGamesPath(const QString &gameName);

  virtual QString myGamesFolderName() const = 0;
  virtual QString identifyGamePath() const = 0;

private:

  QString m_GamePath;
  QString m_MyGamesPath;

  QString m_GameVariant;

  MOBase::IOrganizer *m_Organizer;

};

#endif // GAMEGAMEBRYO_H
