#ifndef GAMEGAMEBRYO_H
#define GAMEGAMEBRYO_H

#include "iplugingame.h"

class ScriptExtender;
class DataArchives;
class SaveGameInfo;
class BSAInvalidation;

#include <QObject>
#include <QString>
class QDir;
class QFileInfo;

#include <memory>

class GameGamebryo : public MOBase::IPluginGame
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginGame)

  friend class GamebryoScriptExtender;
  friend class GamebryoSaveGameInfo;
  friend class GamebryoSaveGameInfoWidget;

public:

  GameGamebryo();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  //getName
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
  virtual QString getBinaryName() const override;
  //getGameShortName
  //getIniFiles
  //getDLCPlugins
  virtual LoadOrderMechanism getLoadOrderMechanism() const override;
  //getNexusModOrganizerID
  //getNexusGameID
  virtual bool looksValid(QDir const &) const override;
  virtual QString getGameVersion() const override;

protected:

  virtual QString getLauncherName() const;

  QFileInfo findInGameFolder(const QString &relativePath) const;
  QString myGamesPath() const;
  QString selectedVariant() const;
  QString getVersion(QString const &program) const;

  static QString localAppFolder();
  //Arguably this shouldn't really be here but every gamebryo program seems to use it
  static QString getLootPath();

  //This function is not terribly well name as it copies exactly where it's told
  //to, irrespective of whether it's in the profile...
  static void copyToProfile(const QString &sourcePath,
                            const QDir &destinationDirectory,
                            const QString &sourceFileName);

  static void copyToProfile(const QString &sourcePath,
                            const QDir &destinationDirectory,
                            const QString &sourceFileName,
                            const QString &destinationFileName);

protected:

  std::map<std::type_index, boost::any> featureList() const;

  //These should be implemented by anything that uses gamebryo (I think)
  //(and if they don't, it'll be a null pointer and won't look implemented,
  //so that's fine too).
  std::shared_ptr<ScriptExtender> m_ScriptExtender { nullptr };
  std::shared_ptr<DataArchives> m_DataArchives { nullptr };
  std::shared_ptr<BSAInvalidation> m_BSAInvalidation { nullptr };
  std::shared_ptr<SaveGameInfo> m_SaveGameInfo { nullptr };

private:

  QString identifyGamePath() const;

private:

  QString m_GamePath;
  QString m_MyGamesPath;

  QString m_GameVariant;

  MOBase::IOrganizer *m_Organizer;

};

#endif // GAMEGAMEBRYO_H
