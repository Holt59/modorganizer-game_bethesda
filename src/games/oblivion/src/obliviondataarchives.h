#ifndef OBLIVIONDATAARCHIVES_H
#define OBLIVIONDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class OblivionDataArchives : public GamebryoDataArchives
{

public:
  OblivionDataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // OBLIVIONDATAARCHIVES_H
