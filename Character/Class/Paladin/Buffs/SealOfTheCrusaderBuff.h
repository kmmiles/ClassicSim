#ifndef SEALOFTHECRUSADERBUFF_H
#define SEALOFTHECRUSADERBUFF_H

#include "Buff.h"
#include "TalentRequirer.h"

#include <QVector>

class Paladin;

class SealOfTheCrusaderBuff: public Buff, public TalentRequirer {
public:
    SealOfTheCrusaderBuff(Paladin* pchar);
    ~SealOfTheCrusaderBuff() override = default;

private:
    Paladin* paladin;

    unsigned sotc_ap_bonus;
    const unsigned sotc_ap_base {306};
    const QVector<double> improved_sotc_ranks {1.0, 1.05, 1.10, 1.15};

    void buff_effect_when_applied() override;
    void buff_effect_when_removed() override;
    void increase_talent_rank_effect(const QString& talent_name, const int curr) override;
    void decrease_talent_rank_effect(const QString& talent_name, const int curr) override;
};

#endif // SEALOFTHECRUSADERBUFF_H